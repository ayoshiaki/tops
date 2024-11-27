/*
 *       NeuralNetworkModel.cpp
 *
 *       Copyright 2011 Waldir Caro <waldirc@ime.usp.br>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#include "NeuralNetworkModel.hpp"
#include "NeuralNetworkModelCreator.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {

    NeuralNetworkModel::NeuralNetworkModel(){
        _trained_model_file = "";
        _initialized = false;
    }

    NeuralNetworkModel::NeuralNetworkModel(std::shared_ptr<torch::nn::Sequential> module_nn_ptr){
        _module_nn = *module_nn_ptr;
        _trained_model_file = "";
        _initialized = false;
    }

    NeuralNetworkModel::NeuralNetworkModel(std::string trained_model_file){
        _trained_model_file = trained_model_file;
        _initialized = false;
    }

    // ToDo: UPDATE to be the same as the config file.
    std::string NeuralNetworkModel::str () const {
        std::stringstream out;
        out << "model_name = \"NeuralNetworkModel\"\n" ;
        out << "layers = ";
        
        ModuleParameterValuePtr architecture = ModuleParameterValuePtr(new ModuleParameterValue(std::make_shared<torch::nn::Sequential>(_module_nn)));
        out << architecture->str();        

        out << "upstream_length = " << _upstream_length << "\n";
        out << "downstream_length = " << _downstream_length << "\n";
        out << "trained_model_file = \"" << _trained_model_file << "\"\n";        
        
        return out.str();
    }

    std::string NeuralNetworkModel::print_graph () const {
        return this->str();
    }

    void NeuralNetworkModel::setParameters(std::shared_ptr<torch::nn::Sequential> module_nn_ptr, std::string trained_model_file, int upstream_length, int downstream_length) {
        _module_nn = *module_nn_ptr;
        _trained_model_file = trained_model_file;
        _upstream_length = upstream_length;
        _downstream_length = downstream_length;
        _sequence_length = _upstream_length + _downstream_length;
        _initialized = false;
        
        if (_trained_model_file != ""){ //there is a trained model in jit format
            try {
                // Deserialize the ScriptModule from a file using torch::jit::load()                
                _trained_module_nn = torch::jit::load(_trained_model_file);
                _trained_module_nn.eval();
                non_const_jit_module = const_cast<torch::jit::script::Module*>(&_trained_module_nn);
                
                torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
        
                //std::cout << "1. Initialize prefix NN " << (torch::cuda::is_available() ? "torch::kCUDA\n" : "torch::kCPU\n");
                non_const_jit_module->to(device);
                non_const_jit_module->eval();
            }
            catch (const c10::Error& e) {
                std::cerr << "error loading the trained model (" << e.what() << ")\n";
            }
        }
        //std::cout << this->str();
    }

    ProbabilisticModelCreatorPtr NeuralNetworkModel::getFactory () const{
        return NeuralNetworkModelCreatorPtr(new NeuralNetworkModelCreator());
    }

    void NeuralNetworkModel::initialize(const ProbabilisticModelParameters & p) {
        //ProbabilisticModelParameterValuePtr weight = p.getMandatoryParameterValue("weight");
        //ProbabilisticModelParameterValuePtr bias = p.getMandatoryParameterValue("bias");
        ProbabilisticModelParameterValuePtr module_nn_ptr = p.getMandatoryParameterValue("layers");
        ProbabilisticModelParameterValuePtr upstream_length_p = p.getMandatoryParameterValue("upstream_length");
        ProbabilisticModelParameterValuePtr downstream_length_p = p.getMandatoryParameterValue("downstream_length");
        ProbabilisticModelParameterValuePtr model_file = p.getOptionalParameterValue("trained_model_file");
        ProbabilisticModelParameterValuePtr symbols = p.getOptionalParameterValue("alphabet");

        // load a trained model if it is present                
        std::shared_ptr<torch::nn::Sequential> module_nn = module_nn_ptr->getModule();
        
        int upstream_length = upstream_length_p->getInt();
        int downstream_length = downstream_length_p->getInt();
        std::string trained_model_file = "";
        if (model_file != NULL) trained_model_file = model_file->getString();
        
        AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
        alphabet->initializeFromVector(symbols->getStringVector());
        setAlphabet(alphabet);

        setParameters(module_nn, trained_model_file, upstream_length, downstream_length);
    }

    ProbabilisticModelParameters NeuralNetworkModel::parameters () const {
        
        ProbabilisticModelParameters par;
        par.add("model_name", StringParameterValuePtr(new StringParameterValue("NeuralNetworkModel")));
        auto module_nn = std::make_shared<torch::nn::Sequential>(_module_nn);
        par.add("layers", ModuleParameterValuePtr(new ModuleParameterValue(module_nn)));
        par.add("upstream_length", IntParameterValuePtr(new IntParameterValue(_upstream_length)));
        par.add("downstream_length", IntParameterValuePtr(new IntParameterValue(_downstream_length)));
        if (_trained_model_file != "") par.add("trained_model_file", StringParameterValuePtr(new StringParameterValue(_trained_model_file)));
        return par;
    }

    // Transform input_data (vector<vector<int>>) in tensor_data (Tensor) one-hot encoded
    torch::Tensor NeuralNetworkModel::sequences_to_Tensor(SequenceList & input_data) const {
        
        // Step 1: Convert the vector of vectors to a flat vector
        std::vector<int> flat_data;
        for (const auto& row : input_data) {
            flat_data.insert(flat_data.end(), row.begin(), row.end());
        }

        // Step 2: Convert the flat vector to a torch::Tensor
        torch::Tensor tensor_data = torch::from_blob(flat_data.data(), {input_data.size(), input_data[0].size()}, torch::kInt);
        tensor_data = tensor_data.to(torch::kLong);

        // Step 3: Find the maximum value in the tensor to determine the number of classes        
        //int num_classes = tensor_data.max().item<int>() + 1; // Assuming classes are zero-indexed
        // It can be also the length of the Alphabet
        int num_classes = alphabet()->size();

        // Step 4: Convert the tensor to a one-hot encoded tensor
        torch::Tensor one_hot_tensor = torch::one_hot(tensor_data, num_classes);
	    std::cerr << "[DEBUG] one hot tensor: " << one_hot_tensor.sizes() << " | " << tensor_data.sizes() << "\n";

        // Step 5: Convert the one-hot tensor to the desired data type (e.g., float)
        one_hot_tensor = one_hot_tensor.to(torch::kFloat32);
        /*for(int i=0;i<10;i++){
            std::cerr << "[DEBUG] tensor type: " << tensor_data[0][i] << " -> " << one_hot_tensor[0][i] << std::endl;
        }*/
        
        // CREATE TENSOR PADDING WITH ZEROS UPSTREAM AND DOWNSTREAM
        #if 0
            // Create tensors of zeros for the beginning and the end
            auto zeros_begin = torch::zeros({1, 200, 4});
            auto zeros_end = torch::zeros({1, 200, 4});

            // Concatenate the tensors
            one_hot_tensor = torch::cat({zeros_begin, one_hot_tensor, zeros_end}, 1);
            std::cerr << "[DEBUG] one hot tensor: " << one_hot_tensor.sizes() << " | " << tensor_data.sizes() << "\n";
        #endif

        return one_hot_tensor;
    }

    // forward method using the sequential architecture neural network
    double NeuralNetworkModel::trainSGDAlgorithm(SequenceList & training_set, int epochs, int batch_size, double learning_rate) {
        
        // Hyperparameters
        double loss_value = HUGE;
        // int batch_size = 64;
        // double learning_rate = 0.01;

        // Transform the training set into Tensors
        torch::Tensor input = sequences_to_Tensor(training_set);

        // Specify the batch size
        int num_batches = input.size(0) / batch_size;        
              
        // Define the loss function and optimizer
        torch::nn::MSELoss loss_fn;
        // Instantiate an SGD optimization algorithm to update our net's parameters.
        torch::optim::SGD optimizer(_module_nn->parameters(), torch::optim::SGDOptions(learning_rate));


        for (size_t epoch = 1; epoch <= epochs; ++epoch) {
            size_t batch_index = 0;
            // Iterate the input data to yield batches from the dataset.
            for (int i = 0; i < num_batches; ++i) {

                // Reset gradients.
                //optimizer.zero_grad();
                _module_nn->zero_grad();

                // Get i batch data
                torch::Tensor batch_data = input.slice(0, i * batch_size, (i + 1) * batch_size);
                
                // Execute the model (forward) on the input batch data.
                torch::Tensor predictions = _module_nn->forward(batch_data);

                // Compute a loss value to judge the prediction of our model.
                torch::Tensor loss = loss_fn(predictions, /*batch_data.target*/batch_data.slice(1, batch_data.size(1) - 1, batch_data.size(1)));
                
                // Compute gradients of the loss w.r.t. the parameters of our model.
                loss.backward();
                
                // Update the parameters based on the calculated gradients.
                optimizer.step();
                
                // Output the loss and checkpoint every 10 batches.
                if (++batch_index % 100 == 0) {
                    std::cerr << "Epoch: " << epoch << " | Batch: " << batch_index
                    << " | Loss: " << loss.item<float>() << std::endl;
                }
                loss_value = loss.item<double>();
            }
        }
        
        return loss_value;
    }

    // size of \@s should be bigger or equal than sequence_length
    double NeuralNetworkModel::evaluatePosition(const Sequence & s, unsigned int i) const {        
        if(s.size() >= _sequence_length){
            try {
            
                torch::Tensor input_t;
                if(!_initialized){
                    // Create (copy) the subsequence
                    Sequence subsequence(s.begin() + (i - _upstream_length), s.begin() + (i + _downstream_length));                    

                    // Create a SequenceList from the subsequence
                    SequenceList sample = {subsequence};

                    // Convert the SequenceList to a torch::Tensor using sequences_to_Tensor
                    input_t = sequences_to_Tensor(sample);
                }
                else{
                    // Extract the specified column range (inclusive)
                    input_t = _last_sequence.slice(1, i - _upstream_length, i + _downstream_length);
                }

                // Make sure the module is not null and has been loaded correctly
                // assert(_trained_module_nn);                
                torch::Tensor output = non_const_jit_module->forward({input_t}).toTensor();
                
                torch::Tensor max_value_tensor = output.max();                
                torch::Tensor probs = torch::softmax(output, 1);
                std::cerr << "evaluate_position" << i << "\n\toutput: (" << output << ")\n\tprobability: (" << probs << ")" << std::endl;
                return log((probs.select(1, 1)).item<double>());
            }
            catch (const c10::Error& e) {
                std::cerr << "error evaluating the position of the sequence in the model\n";
                return log(0.0);
            }
        }
        else {            
            std::cerr << "Invalid sequence length evaluating" << std::endl;
            return 0.0;
        }
    }

    // \@begin should be 0 and @end should be the same as sequence_length (where sequence_length is the sum of upstream and downstream)
    // ?? CHECK: evaluate is the sequence likelihood given this model ??
    double NeuralNetworkModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {        
	   
        if((end - begin) == _sequence_length){
            try {
            
                torch::Tensor input_t;
                if(!_initialized){
                    // Create (copy) the subsequence
                    Sequence subsequence(s.begin() + begin, s.begin() + end);

                    // Create a SequenceList from the subsequence
                    SequenceList sample = {subsequence};

                    // Convert the SequenceList to a torch::Tensor using sequences_to_Tensor
                    input_t = sequences_to_Tensor(sample);
                }
                else{
                    // Extract the specified column range (inclusive)
                    input_t = _last_sequence.slice(1, begin, end);
                }

                // Make sure the module is not null and has been loaded correctly
                // assert(_trained_module_nn);                
                torch::Tensor output = non_const_jit_module->forward({input_t}).toTensor();
                
                torch::Tensor max_value_tensor = output.max();                
                torch::Tensor probs = torch::softmax(output, 1);
		        auto probs_max = probs.select(1,1);
                //std::cerr << "[DEBUG]\toutput: (" << probs << ")\n\tprobability: (" << probs_max.item<double>() << ")" << std::endl;
                return log(probs_max.item<double>());
                
            }
            catch (const c10::Error& e) {
                std::cerr << "error evaluating the sequence in the model\n";
                return 0.0;
            }
        }
        else {
            std::cerr << "[ERROR] Invalid sequence length evaluating " << begin <<"\t"<<end<<"\t"<<_sequence_length << std::endl;
            return 0.0;
        }
    }

    double NeuralNetworkModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end, unsigned int phase) const {
    	return evaluate(s, begin, end);
    }

    std::vector<torch::Tensor> split_sequence(const torch::Tensor& one_hot_sequence, int subseq_size, int stride) {
        std::vector<torch::Tensor> subsequences;

        // Get the length of the sequence from the second dimension
        int len = one_hot_sequence.size(0);

        for (int i = 0; i <= len - subseq_size; i += stride) {
            // Slice the tensor to get the subsequence
            torch::Tensor subseq = one_hot_sequence.slice(0, i, i + subseq_size);
            subsequences.push_back(subseq);
        }

        return subsequences;
    }

    // Function to stack the subsequences into a single tensor
    torch::Tensor stack_subsequences(const std::vector<torch::Tensor>& subsequences) {
        // Stack the subsequences into a single tensor
        return torch::stack(subsequences);
    }

    // Classify each subsequence in batches and measure time
    std::vector<int> NeuralNetworkModel::classify_subsequences_in_batches(const torch::Tensor& stacked_subsequences, int batch_size, double threshold, torch::Device device) {
        /**
         * @brief A vector to store the predictions made by the neural network model.
         * 
         * This vector holds integer values representing the predicted classes or outcomes
         * generated by the neural network during the inference phase. Each element in the 
         * vector corresponds to a prediction for a given input sample.
         */
        std::vector<int> predictions;
        auto start = std::chrono::high_resolution_clock::now();

        int num_batches = (stacked_subsequences.size(0) + batch_size - 1) / batch_size;
        for (int batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
            int64_t start_idx = batch_idx * batch_size;
            int64_t end_idx = std::min(start_idx + static_cast<int64_t>(batch_size), stacked_subsequences.size(0));

            torch::Tensor batch_tensor = stacked_subsequences.slice(0, start_idx, end_idx).to(device);	        

            // Perform the forward pass of the trained model on the batch tensor
            auto probs_aux = _trained_module_nn.forward({batch_tensor}).toTensor();            
            
            // Apply softmax to the outputs to get the probabilities of each class
            torch::Tensor probs = torch::softmax(probs_aux, 1);
            
            // Store the predictions and scores in the respective vectors
            for (int i = 0; i < probs.size(0); ++i) {
		        // MUST I SAVE THE MAX PROBABILTY AND CLASS PREDICTED OR THE CLASS 1 WITH THE PROBABILITY OF 1 (to ensure low probs in viterbi)?????????
                auto probs_max = probs[i][1].item<double>();
                if(probs_max < threshold) probs_max = 0; // treat as ZERO to avoid FP ???
                auto predicted = probs_max < threshold ? 0:1;
                predictions.push_back(predicted);
                _scores.push_back(log(probs_max));
                
                //std::cerr << probs[i][1].item<double>() << "\n";
                // DEBUG print the splice sites predicted above the threshold
                //if (_scores.size()+199>1775 && _scores.size()+199<1785)
	                //std::cerr << "[INFO] NN splice site \t#:" << _scores.size()+199 << "\tp:" << probs[i][1].item<double>() << "\tlog(p):" << log(probs[i][1].item<double>()) << "\n";
            }            
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> total_time = end - start;
        //std::cerr << "Total time for predictions: " << total_time.count() << " seconds" << std::endl;

        return predictions;
    }

    bool NeuralNetworkModel::initialize_prefix_sum_array(const Sequence & s){
        //when we evaluate more than one input, it is more efficient to send all the inputs as a "batch"
        //so we need to override the initialize_prefix_sum_array
        //instead of usint the superclass method that calls evaluate() for each position of the sequence
        //we one-hot encode the whole sequence and generate all sequences from the sliding window as a batch
        //evaluation will be much faster as it will fully use pythorch's paralellism        
        if(_initialized)
            return _initialized;

        _scores.resize(s.size());
        if(s.size() < _sequence_length){ // Do not initialize if the sequence is shorter than the input of the network
            std::cerr << "[ERROR] Invalid sequence length initializing\n";
            _initialized = false;
            return false;
        }
        
        _scores = {};

        // initialize a Tensor for the whole sequence        
        SequenceList sample = {s};
        _last_sequence = sequences_to_Tensor(sample);
        // Remove extra dimension (of size 1)
        auto tensor_squeezed = _last_sequence.squeeze(0);
	
	    //std::cerr << "[DEBUG] last sequence: " << _last_sequence.sizes() << std::endl;
        //std::cerr << "[DEBUG] last sequence squeezed: " << tensor_squeezed.sizes() << std::endl; 
        
        auto _subsequences = split_sequence(tensor_squeezed, _sequence_length, 1);
        // Stack the subsequences into a single tensor
        torch::Tensor stacked_subsequences = stack_subsequences(_subsequences);
       
        //std::cerr << "[DEBUG] split sequences size: " << _subsequences.size() << std::endl;
        //std::cerr << "[DEBUG] stacked subseq size: " << stacked_subsequences.sizes() << std::endl;
        
        // Classify each subsequence in batches
        // ???? cuda operations are not working
        //torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
        torch::Device device(torch::kCPU);
        int batch_size = 1024;
	    double threshold_p = 0.00;
        std::vector<int> predictions = classify_subsequences_in_batches(stacked_subsequences, batch_size, threshold_p, device);
        //_scores.push_back(predictions);
        
        
        //std::cout << "predictions:\n";
        //for (int i=0; i<predictions.size(); i++) std::cout << predictions[i] << "\n";        
        //std::cout << "scores:\n";        
        //std::cout << "[INFO] " << _trained_model_file << " found:\n";

	// test splice site predictions
#if 0
	int ss_count = 1;
        for (int i=0; i<_scores.size(); i++){
            if (predictions[i]){
                std::cerr << "#" << ss_count++ << "\t" << i + _upstream_length << "\t" << exp(_scores[i]) << "\t";
                int step = -6;
                for(; step < -1; step++){
                    std::cerr << alphabet()->getSymbol(s[i+_upstream_length+step])->name();
                }
                std::cerr << "[" << alphabet()->getSymbol(s[i+_upstream_length-1])->name() << alphabet()->getSymbol(s[i+_upstream_length])->name() << "]";
                for(step=1; step <= 5; step++){
                    std::cerr << alphabet()->getSymbol(s[i+_upstream_length+step])->name();
                }
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
#endif
        
        _initialized = true;
        return _initialized;
    }

    double NeuralNetworkModel::prefix_sum_array_compute(int begin , int end) {
        //return evaluate(_last, begin, end);
        if ((begin < (int) _scores.size()) && (begin >= 0) && _initialized)
            return _scores[begin];
        return -HUGE;
    }

    double NeuralNetworkModel::choose() const {
        //std::cerr << "Not implemented" << std::endl;
        return 0.0;
    }

    Sequence & NeuralNetworkModel::choose(Sequence & s, int size ) const {
        //std::cerr << "Not implemented" << std::endl;
        return s;
    }

    DoubleVector NeuralNetworkModel::getScores() const {
        
	/*std::cerr << "[DEBUG] Scores: ";
        for (const auto& score : _scores) {
            std::cerr << score << " ";
        }
        std::cerr << std::endl;*/
	return _scores;
    }

}


