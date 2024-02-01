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
    }

    NeuralNetworkModel::NeuralNetworkModel(std::shared_ptr<torch::nn::Sequential> module_nn_ptr){
        _module_nn = *module_nn_ptr;
    }

    // ToDo: UPDATE to be the same as the config file.
    std::string NeuralNetworkModel::str () const {
        std::stringstream out;
        out << "model_name = \"NeuralNetworkModel\"\n" ;
        out << "layers = (\n";
        
        //std::cout << "layers size: " << _module_nn->size() << std::endl;

        for(size_t i = 0; i < _module_nn->size(); ++i) {
            const std::shared_ptr<torch::nn::Module>& module_ptr = _module_nn->ptr(i);            
            module_ptr->pretty_print(out);
            out << "\n";
        }
        out << ")\n";
        
        return out.str();
    }

    void NeuralNetworkModel::setParameters(std::shared_ptr<torch::nn::Sequential> module_nn_ptr) {
        _module_nn = *module_nn_ptr;
        std::cout << this->str();
    }

    ProbabilisticModelCreatorPtr NeuralNetworkModel::getFactory () const{
        return NeuralNetworkModelCreatorPtr(new NeuralNetworkModelCreator());
    }

    void NeuralNetworkModel::initialize(const ProbabilisticModelParameters & p) {
        //ProbabilisticModelParameterValuePtr weight = p.getMandatoryParameterValue("weight");
        //ProbabilisticModelParameterValuePtr bias = p.getMandatoryParameterValue("bias");
        ProbabilisticModelParameterValuePtr module_nn_ptr = p.getMandatoryParameterValue("layers");
        std::shared_ptr<torch::nn::Sequential> module_nn = module_nn_ptr->getModule();
        setParameters(module_nn);
    }

    ProbabilisticModelParameters NeuralNetworkModel::parameters () const {
        
        ProbabilisticModelParameters par;
        par.add("model_name", StringParameterValuePtr(new StringParameterValue("NeuralNetworkModel")));
        auto module_nn = std::make_shared<torch::nn::Sequential>(_module_nn);
        par.add("layers", ModuleParameterValuePtr(new ModuleParameterValue(module_nn)));
        return par;
    }

    // Transform input_data (vector<vector<int>>) in tensor_data (Tensor)
    torch::Tensor NeuralNetworkModel::sequences_to_Tensor(SequenceList & input_data){
        
        // Step 1: Convert the vector of vectors to a flat vector
        std::vector<int> flat_data;
        for (const auto& row : input_data) {
            flat_data.insert(flat_data.end(), row.begin(), row.end());
        }

        // Step 2: Convert the flat vector to a torch::Tensor
        torch::Tensor tensor_data = torch::from_blob(flat_data.data(), {input_data.size(), input_data[0].size()}, torch::kInt);

        return tensor_data;
    }

    // forward method using the sequential architecture neural network
    double NeuralNetworkModel::trainSGDAlgorithm(SequenceList & training_set, int epochs) {
        
        // Hyperparameters
        double loss_value = HUGE;        
        int batch_size = 64;        
        double learning_rate = 0.01;

        // Transform the training set into Tensors
        torch::Tensor input = sequences_to_Tensor(training_set);

        // Specify the batch size
        int num_batches = input.size(0) / batch_size;        

        // Create a sequential architecture to train the net
        //torch::nn::Sequential sequential_architecture;//(_module_nn);
        //for (const auto& layer : _module_nn.children())
        //    sequential_architecture->push_back(layer);
        
        // Instantiate an SGD optimization algorithm to update our net's parameters.
        //torch::optim::SGD optimizer(sequential_nn->parameters(), /*lr=*/0.01);
        // Define the loss function and optimizer
        torch::nn::MSELoss loss_fn;
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
                    std::cout << "Epoch: " << epoch << " | Batch: " << batch_index
                    << " | Loss: " << loss.item<float>() << std::endl;
                }
                loss_value = loss.item<double>();
            }
        }
        
        return loss_value;
    }

    
    double NeuralNetworkModel::choose() const {
        //std::cerr << "Not implemented" << std::endl;
        return 0.0;
    }

    Sequence & NeuralNetworkModel::choose(Sequence & s, int size ) const {
        //std::cerr << "Not implemented" << std::endl;
        return s;
    }

}


