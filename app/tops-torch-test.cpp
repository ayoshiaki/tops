#include <torch/torch.h>
//#include <torch/data/dataloader.h>
//#include <torch/script.h>

#include <boost/program_options.hpp>

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <iomanip>
#include <chrono>

#include "ProbabilisticModel.hpp"
#include "NeuralNetworkModel.hpp"

#include "ProbabilisticModelCreatorClient.hpp"
#include "version.hpp"

#define INPUTS 1
#define SEQUENCE 3
#define BATCH 1
#define LAYERS 3
#define HIDDEN 2
#define DIRECTIONS 2
#define OUTPUTS 1

using namespace std;
using namespace tops;
using namespace boost::program_options;

//CNN struct (layers according SplicerOver project)
struct NetImpl : torch::nn::Module {
  NetImpl() {
    //Construct and register CNN submodules.
    conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(1, 6, 4).stride(2).padding(1).bias(false)));//torch::nn::Conv2d(/*input_shape*/1, 70, /*kernel_size=*/(9, 4)));//, /*stride=*/1, /*padding=*/1));
    pool1 = register_module("pool1", torch::nn::MaxPool2d((3, 1)));
    conv2 = register_module("conv2", torch::nn::Conv2d(70, 100, /*kernel_size=*/(7, 1)));
    conv3 = register_module("conv3", torch::nn::Conv2d(100, 100, /*kernel_size=*/1));
    conv4 = register_module("conv4", torch::nn::Conv2d(100, 200, /*kernel_size=*/(7, 1)));
    pool2 = register_module("pool2", torch::nn::MaxPool2d((4,1)));
    conv5 = register_module("conv5", torch::nn::Conv2d(200, 250, /*kernel_size=*/(7, 1)));
    pool3 = register_module("pool3", torch::nn::MaxPool2d((4,1)));
    dense1 = register_module("dense1", torch::nn::Linear(250, 512));

    bias = register_parameter("bias", torch::randn(10));
  }
  
  //Implement the Net's algorithm.
  torch::Tensor forward(torch::Tensor x) {
    //Use one of the many tensor manipulation functions.
    double p_dropout = 0.2;
    bool train_dropout = is_training();
    x = torch::dropout(conv1(x), /*p=*/p_dropout, /*train=*/train_dropout); // conv1 -> dropout1
    x = torch::dropout(conv2(x), /*p=*/p_dropout, /*train=*/train_dropout); // conv2 -> dropout2
    x = torch::dropout(pool1(conv3(x)), /*p=*/p_dropout, /*train=*/train_dropout); // conv3 -> pool1 -> dropout3
    x = torch::dropout(pool2(conv4(x)), /*p=*/p_dropout, /*train=*/train_dropout); // conv4 -> pool2 -> dropout4
    x = torch::dropout(pool3(conv5(x)), /*p=*/p_dropout, /*train=*/train_dropout); // conv5 -> pool3 -> dropout5
    x = torch::dropout(dense1(x), /*p=*/p_dropout, /*train=*/train_dropout); // dense1 -> dropout6
    x = torch::softmax(x, /*dim=*/2);
    return x;
  }

  //Modules of the spliceRover CNN
  torch::nn::Conv2d conv1{nullptr}, conv2{nullptr}, conv3{nullptr}, conv4{nullptr}, conv5{nullptr};
  torch::nn::MaxPool2d pool1{nullptr}, pool2{nullptr}, pool3{nullptr};
  torch::nn::Linear dense1{nullptr};

  torch::Tensor bias;
};

TORCH_MODULE(Net);


struct SequentialImple : torch::nn::Module {
  SequentialImple() {
    //Construct and register CNN submodules.
    conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(1, 1, 1).stride(1).padding(1).bias(false)));//torch::nn::Conv2d(/*input_shape*/1, 70, /*kernel_size=*/(9, 4)));//, /*stride=*/1, /*padding=*/1));
    conv2 = register_module("conv2", torch::nn::Conv2d(1, 1, /*kernel_size=*/(1, 1)));
  }
  
  //Implement the Net's algorithm.
  torch::Tensor forward(torch::Tensor x) {
    //Use one of the many tensor manipulation functions.
    double p_dropout = 0.2;
    bool train_dropout = is_training();
    x = torch::dropout(conv1(x), /*p=*/p_dropout, /*train=*/train_dropout); // conv1 -> dropout1
    x = torch::dropout(conv2(x), /*p=*/p_dropout, /*train=*/train_dropout); // conv2 -> dropout2
    return x;
  }

  //Modules of the spliceRover CNN
  torch::nn::Conv2d conv1{nullptr}, conv2{nullptr};
};

//TORCH_MODULE(Sequential);

//Bidirectional LSTM NN
struct BLSTM_Model : torch::nn::Module {
  BLSTM_Model(uint64_t layers, uint64_t hidden, uint64_t inputs){
    lstm         = register_module("lstm", torch::nn::LSTM(torch::nn::LSTMOptions(inputs, hidden).num_layers(layers)));
    reverse_lstm = register_module("rlstm", torch::nn::LSTM(torch::nn::LSTMOptions(inputs, hidden).num_layers(layers)));
    linear       = register_module("linear", torch::nn::Linear(hidden*DIRECTIONS, OUTPUTS));
  }

  torch::Tensor forward(torch::Tensor x) {
    //Reverse and feed into LSTM + Reversed LSTM
    auto lstm1 = lstm->forward(x.view({ x.size(0), BATCH, -1 }));
    //                         [SEQUENCE,BATCH,FEATURE]
    auto lstm2 = reverse_lstm->forward(torch::flip(x, 0).view({ x.size(0), BATCH, -1 }));
    //Reverse Output from Reversed LSTM + Combine Outputs into one Tensor
    auto cat = torch::empty({ DIRECTIONS, BATCH, x.size(0), HIDDEN });
    //                       [DIRECTIONS, BATCH, SEQUENCE, FEATURE]
    //std::cout << "lstm1: " << std::get<0>(lstm1) << std::endl;
    //std::cout << "lstm2: " << std::get<0>(lstm2) << std::endl;
    //TODO: lstm is tuple<tensor tuple<tensor, tensor>>, so what is actually the output? test with a bidirectional lstm of libtorch
    cat[0] = std::get<1>(std::get<1>(lstm1)).view({ BATCH, x.size(0), HIDDEN });
    cat[1] = torch::flip(std::get<1>(std::get<1>(lstm2)).view({ BATCH, x.size(0), HIDDEN }), 1);
    //Feed into Linear Layer
    auto out = torch::sigmoid(linear->forward(cat.view({ BATCH, x.size(0), HIDDEN*DIRECTIONS })));
    //                                                [BATCH, SEQUENCE, FEATURE]
    return out;
  }

  torch::nn::LSTM lstm{ nullptr };
  torch::nn::LSTM reverse_lstm{ nullptr };
  torch::nn::Linear linear{ nullptr };
};


class Rede {
  public:
    Rede() {

    };
    Rede(torch::nn::Module _architecture){
      architecture = _architecture;
      auto net = std::make_shared<torch::nn::Module>(architecture);
      // Access and print the convolutional layer's parameters
      for (const auto& param : net->named_modules()) {
          std::cout << "Parameter Name: " << param.key() << "\tShape: " << param.value().get() << std::endl;
      }
    };
  private:
    torch::nn::Module architecture;
};

class EmptyModule {

  struct Network : torch::nn::Module {
      Network() {
      }

      void add_layers(torch::nn::Module module_aux) {
        auto net = std::make_shared<torch::nn::Module>(module_aux);
        for(auto& module_i : net->named_modules())            
          this->register_module(module_i.key(), module_i.value());
      }
      
      //Implement the Net's algorithm.
      torch::Tensor forward(torch::Tensor x) {
        for (auto& layer : this->children()){
          //torch::nn::AnyModule layer_aux = *layer;
          //x = layer(x);
        }
          
        return x;
      }
    };

  public:
    EmptyModule() {
    };
    
    torch::nn::Module getModule() {
      return *ptr_mymodule;
    };

    void RegisterModule(std::string name){
      ptr_mymodule->register_module(name, torch::nn::Conv2d(3, 64, 3));
    }

    void ShowModule(){
      for (const auto& param : ptr_mymodule->named_modules()) {
        std::cout << "Parameter Name: " << param.key() << "\tShape: " << param.value().get() << std::endl;
      }
    }
    

  private:
    Network mymodule;
    std::shared_ptr<Network> ptr_mymodule;
};


template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}


class MyModuleImpl : public torch::nn::Module {
public:
    MyModuleImpl() {
        // Define your model's architecture in the constructor
        linear1 = register_module("linear1", torch::nn::Linear(10, 5));
        linear2 = register_module("linear2", torch::nn::Linear(5, 2));
    }

    torch::Tensor forward(torch::Tensor input) {
        // Define the computation graph within the forward method
        return linear2->forward(torch::relu(linear1->forward(input)));
    }

    double trainSGDAlgorithm(torch::Tensor input, int epochs) {
        
        // Hyperparameters
        double loss_value = HUGE;
        int batch_size = 64;
        double learning_rate = 0.01;

        // Specify the batch size
        int num_batches = input.size(0) / batch_size;        
              
        // Define the loss function and optimizer
        torch::nn::MSELoss loss_fn;
        // Instantiate an SGD optimization algorithm to update our net's parameters.
        torch::optim::SGD optimizer(this->parameters(), torch::optim::SGDOptions(learning_rate));


        for (size_t epoch = 1; epoch <= epochs; ++epoch) {
            size_t batch_index = 0;
            // Iterate the input data to yield batches from the dataset.
            for (int i = 0; i < num_batches; ++i) {

                // Reset gradients.
                //optimizer.zero_grad();
                this->zero_grad();

                // Get i batch data
                torch::Tensor batch_data = input.slice(0, i * batch_size, (i + 1) * batch_size);
                
                // Execute the model (forward) on the input batch data.
                torch::Tensor predictions = this->forward(batch_data);

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

private:
    torch::nn::Linear linear1{nullptr}, linear2{nullptr};
};

TORCH_MODULE(MyModule); // This is needed to register the module as a TorchScript module


class CNNSplice_Model1 : public torch::nn::Module {
public:
    CNNSplice_Model1(int Length)
        : conv1(torch::nn::Conv1dOptions(4, 50, 9).stride(1).padding(4)),
          pool1(torch::nn::AvgPool1dOptions(2).stride(1)),
          conv2(torch::nn::Conv1dOptions(50, 50, 9).stride(1).padding(4)),
          pool2(torch::nn::AvgPool1dOptions(2).stride(1)),
          conv3(torch::nn::Conv1dOptions(50, 50, 9).stride(1).padding(4)),
          pool3(torch::nn::AvgPool1dOptions(2).stride(1)),
          fc1((50 * Length) - 150, 100),
          fc2(100, 2),
          relu(torch::nn::ReLU()),
          dropout(torch::nn::Dropout(0.3)) {
        register_module("conv1", conv1);
        register_module("pool1", pool1);
        register_module("conv2", conv2);
        register_module("pool2", pool2);
        register_module("conv3", conv3);
        register_module("pool3", pool3);
        register_module("fc1", fc1);
        register_module("fc2", fc2);
        register_module("relu", relu);
        register_module("dropout", dropout);
    }

    torch::Tensor forward(torch::Tensor x) {
        x = torch::relu(conv1(x.permute({0, 2, 1})));
        x = pool1(x);
        x = torch::relu(conv2(x));
        x = pool2(x);
        x = torch::relu(conv3(x));
        x = pool3(x);
        x = x.view({x.size(0), -1});
        x = torch::relu(fc1(x));
        x = dropout(x);
        x = fc2(x);
        return x;
    }

private:
    torch::nn::Conv1d conv1, conv2, conv3;
    torch::nn::AvgPool1d pool1, pool2, pool3;
    torch::nn::Linear fc1, fc2;
    torch::nn::ReLU relu;
    torch::nn::Dropout dropout;
};


/*
*
*
*
*/

// Constants
const int64_t Length = 400;  // Length of window
const int64_t BatchSize = 64;  // Batch size

// Custom dataset class
class CustomDataset : public torch::data::datasets::Dataset<CustomDataset> {
public:
    CustomDataset(const std::string& data_path, const std::string& labels_path) {
        data_ = load_data(data_path);
        targets_ = loadTensor(labels_path);
        // Print the shapes of the resulting tensors
        std::cout << "data shape: " << data_.sizes() << std::endl;
        std::cout << "target shape: " << targets_.sizes() << std::endl;

        //data_ = sliceTensor(data_);        
        //std::cout << "data sliced shape: " << data_.sizes() << std::endl;
        //std::cout << "target sliced shape: " << targets_.sizes() << std::endl;

        //data_ = data_.view({-1, Length, 4});
        targets_ = targets_.view({-1});
        targets_ = targets_.to(torch::kLong);
        std::cout << "data view: " << data_.sizes() << std::endl;
        std::cout << "target view: " << targets_.sizes() << std::endl;
    }

    torch::data::Example<> get(size_t index) override {
        return {data_[index], targets_[index]};
    }

    torch::optional<size_t> size() const override {
        return data_.size(0);
    }

private:
// Function to slice the tensor
    torch::Tensor sliceTensor(const torch::Tensor& tensor) {
        int start_col = (400 - Length) * 2;
        int end_col = 1600 - (400 - Length) * 2;

        return tensor.index({"...", torch::indexing::Slice(start_col, end_col)});
    }
    torch::Tensor loadTensor(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<float>> data;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<float> row((std::istream_iterator<float>(iss)), std::istream_iterator<float>());
        data.push_back(row);
    }

    if (data.empty()) {
        throw std::runtime_error("File is empty or invalid format.");
    }

    auto options = torch::TensorOptions().dtype(torch::kFloat32);
    torch::Tensor tensor = torch::empty({static_cast<long>(data.size()), static_cast<long>(data[0].size())}, options);

    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            tensor[i][j] = data[i][j];
        }
    }

    return tensor;
}
    torch::Tensor load_data(const std::string& path) {
        std::ifstream file(path);
        std::string line;
        std::vector<float> values;
        int rows = 0;
        int columns = 0;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            float value;
            while (iss >> value) {
                values.push_back(value);
            }
            ++rows;
        }

        std::cout << "rows " << path << ": " << rows << std::endl;

        return torch::from_blob(values.data(), {rows, Length, 4}).clone();
    }

    torch::Tensor data_, targets_;
};



// Function to split the dataset
  std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> trainTestSplit(const torch::Tensor& data, const torch::Tensor& labels, double test_size) {
      auto dataset_size = data.size(0);
      auto test_size_long = static_cast<long>(dataset_size * test_size);

      // Create indices and shuffle them
      torch::Tensor indices = torch::randperm(dataset_size, torch::kLong);

      auto train_indices = indices.slice(0, 0, dataset_size - test_size_long);
      auto test_indices = indices.slice(0, dataset_size - test_size_long, dataset_size);

      auto x_train = data.index_select(0, train_indices);
      auto y_train = labels.index_select(0, train_indices);
      auto x_test = data.index_select(0, test_indices);
      auto y_test = labels.index_select(0, test_indices);

      return std::make_tuple(x_train, y_train, x_test, y_test);
  }


// Define the CNN model
struct CNNSpliceModel : torch::nn::Module {
    CNNSpliceModel() {
        conv1 = register_module("conv1", torch::nn::Conv1d(torch::nn::Conv1dOptions(4, 50, 9).stride(1).padding(4)));
        pool1 = register_module("pool1", torch::nn::AvgPool1d(torch::nn::AvgPool1dOptions(2).stride(1)));
        conv2 = register_module("conv2", torch::nn::Conv1d(torch::nn::Conv1dOptions(50, 50, 9).stride(1).padding(4)));
        pool2 = register_module("pool2", torch::nn::AvgPool1d(torch::nn::AvgPool1dOptions(2).stride(1)));
        conv3 = register_module("conv3", torch::nn::Conv1d(torch::nn::Conv1dOptions(50, 50, 9).stride(1).padding(4)));
        pool3 = register_module("pool3", torch::nn::AvgPool1d(torch::nn::AvgPool1dOptions(2).stride(1)));
        flatten = register_module("flatten", torch::nn::Flatten());
        fc1 = register_module("fc1", torch::nn::Linear((50 * Length) - 150, 100));
        dropout = register_module("dropout", torch::nn::Dropout(0.3));
        fc2 = register_module("fc2", torch::nn::Linear(100, 2));
    }

    torch::Tensor forward(torch::Tensor x) {
        
        x = x.permute({0, 2, 1});  // Convert (batch_size, length, channels) to (batch_size, channels, length)
        x = torch::relu(conv1->forward(x));
        x = pool1->forward(x);
        x = torch::relu(conv2->forward(x));
        x = pool2->forward(x);
        x = torch::relu(conv3->forward(x));
        x = pool3->forward(x);
        x = flatten->forward(x);
        x = torch::relu(fc1->forward(x));
        x = dropout->forward(x);
        x = fc2->forward(x);
        return x;
    }

    torch::nn::Conv1d conv1{nullptr}, conv2{nullptr}, conv3{nullptr};
    torch::nn::AvgPool1d pool1{nullptr}, pool2{nullptr}, pool3{nullptr};
    torch::nn::Flatten flatten{nullptr};
    torch::nn::Linear fc1{nullptr}, fc2{nullptr};
    torch::nn::Dropout dropout{nullptr};
};

void train_model(std::shared_ptr<CNNSpliceModel>& model, torch::data::StatelessDataLoader<torch::data::datasets::MapDataset<CustomDataset, torch::data::transforms::Stack<torch::data::Example<at::Tensor, at::Tensor> > >, torch::data::samplers::RandomSampler>& dataloader, torch::nn::CrossEntropyLoss& criterion, torch::optim::Adam& optimizer){//, torch::Device device) {
    model->train();    
    double running_loss = 0.0;
    int correct = 0;
    int total = 0;
    
    for (auto& batch : dataloader) {
        
        auto inputs = batch.data;//.to(device);
        auto labels = batch.target;//.to(device).to(torch::kLong);        
        
        optimizer.zero_grad();        
        auto outputs = model->forward(inputs);        
        //std::cout << "labels: " << labels.sizes() << std::endl;
        //std::cout << "outputs: " << outputs.sizes() << std::endl;
        auto loss = criterion(outputs, labels);        
        loss.backward();        
        optimizer.step();        

        running_loss += loss.item<double>() * inputs.size(0);
        auto predicted = outputs.argmax(1);
        total += labels.size(0);
        correct += predicted.eq(labels).sum().item<int>();
    }

    double epoch_loss = running_loss / total;
    double epoch_accuracy = static_cast<double>(correct) / total;

    std::cout << "Train Loss: " << epoch_loss << ", Accuracy: " << epoch_accuracy << std::endl;
}

void training_process(const std::string& data_path, const std::string& labels_path, const std::string& seq, const std::string& dataorg){//, torch::Device device) {
    
    auto dataset = CustomDataset(data_path, labels_path).map(torch::data::transforms::Stack<>());
    auto data_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(dataset, BatchSize);

    //std::cout << "data loader tipo: (" << type_name<decltype(*data_loader)>() << ')\n';

    auto model = std::make_shared<CNNSpliceModel>();
    //model->to(device);

    torch::nn::CrossEntropyLoss criterion;
    torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(1e-4));

    int num_epochs = 1;
    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        std::cout << "Epoch " << (epoch + 1) << "/" << num_epochs << std::endl;
        train_model(model, *data_loader, criterion, optimizer);//, device);
    }

    //evaluate_model(model, *data_loader, criterion, device);

    // Save model
    torch::save(model, "./../pytorchTests/CNNSplice/modelsTorch/tops_cnnsplice_" + seq + "_" + dataorg + ".pt");
}

/* void evaluate_model(std::shared_ptr<CNNSpliceModel>& model, torch::data::DataLoader<CustomDataset>& dataloader, torch::nn::CrossEntropyLoss& criterion, torch::Device device) {
    model->eval();
    double running_loss = 0.0;
    int correct = 0;
    int total = 0;
    std::vector<int> all_preds, all_labels;

    for (auto& batch : dataloader) {
        auto inputs = batch.data.to(device);
        auto labels = batch.target.to(device).to(torch::kLong);

        auto outputs = model->forward(inputs);
        auto loss = criterion(outputs, labels);

        running_loss += loss.item<double>() * inputs.size(0);
        auto predicted = outputs.argmax(1);
        total += labels.size(0);
        correct += predicted.eq(labels).sum().item<int>();

        all_preds.insert(all_preds.end(), predicted.data<int>(), predicted.data<int>() + predicted.numel());
        all_labels.insert(all_labels.end(), labels.data<int>(), labels.data<int>() + labels.numel());
    }

    double epoch_loss = running_loss / total;
    double epoch_accuracy = static_cast<double>(correct) / total;

    std::cout << "Test Loss: " << epoch_loss << ", Accuracy: " << epoch_accuracy << std::endl;
} */

/*
*
*
*
*/

auto get_cuda_device(const int number) -> std::string
{
     assert(number >= 0 && number <= torch::cuda::device_count());
     return "cuda:" + std::to_string(number);
}

int main (int argc, char* argv[]) {
    
    /*{
      if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <name> <mode>" << std::endl;
        return 1;
      }

      std::string name = argv[1];
      std::string mode = argv[2];

      //auto device_string = get_cuda_device(2);
      //std::cout << "GPU: " << device_string << std::endl;
      //torch::Device device(device_string);

      std::string seq = "acceptor";
      std::vector<std::string> dataorg_list = {"hs"};

      for (const auto& dataorg : dataorg_list) {
          std::string data_path = "./../pytorchTests/CNNSplice/data/" + mode + "/" + dataorg + "/train_" + seq + "_" + dataorg;
          std::string labels_path = "./../pytorchTests/CNNSplice/data/" + mode + "/" + dataorg + "/train_" + seq + "_" + dataorg + "_lbl";
          training_process(data_path, labels_path, seq, dataorg);
      }

      std::cout << "======================" << std::endl;
      std::cout << "Start Donor Convolution" << std::endl;

      seq = "donor";
      for (const auto& dataorg : dataorg_list) {
          std::string data_path = "./../pytorchTests/CNNSplice/data/" + mode + "/" + dataorg + "/train_" + seq + "_" + dataorg;
          std::string labels_path = "./../pytorchTests/CNNSplice/data/" + mode + "/" + dataorg + "/train_" + seq + "_" + dataorg + "_lbl";
          training_process(data_path, labels_path, seq, dataorg);
      }
    }*/
    

    /*
    ------------------------------------------------------------------------------------------
    */

    /* TEST LOAD TORCH JIT MODEL */
    srand(time(NULL));

    {
      std::string filepath = "";
      // /home/waldir/Documents/deeptops/examples/neuralnetwork/acceptor_cnnsplice_hs_jit.pth
      std::cin >> filepath;
      torch::jit::script::Module module;
      try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        std::cout << filepath << " ...\n";
        module = torch::jit::load(filepath);
        std::cout << filepath << " OK\n";
        auto input_data = torch::zeros({1, 400, 4});
        //auto ivalue = torch::jit::IValue(input_data);
        //std::vector<torch::jit::IValue> input;
        //input.push_back(ivalue);

        // Run inference using the loaded TorchScript module
        torch::Tensor output = module.forward({input_data}).toTensor();
        std::cout << output << std::endl;

        for (const auto& pair : module.named_children()) {
          const std::string& name = pair.name;
          const torch::jit::script::Module& child_module = pair.value;
          std::cout << "Child name: " << name << std::endl;
          // You can further inspect or use the child module here
        }

        //std::cout << module.dump_to_str(true, true, true) << std::endl;
      }
      catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        std::cout << e.msg() << std::endl;
        return -1;
      }
      
    }

    /*
    ------------------------------------------------------------------------------------------
    */

    /* VECTOR of AnyModule to call a sequential forward -> WORKS FINE
    
    vector<torch::nn::AnyModule> layers;
    for (size_t i = 0; i < 5; i++)
    {
      torch::nn::AnyModule module_aux(torch::nn::Conv1d(torch::nn::Conv1dOptions(1, 1, 1)));
      layers.push_back(module_aux);
    }
    
    torch::Tensor x = torch::ones({1,1});
    for (size_t i = 0; i < layers.size(); i++)
    {
      x = layers[i].forward(x);
    }
    
    std::cout << x << std::endl;

    */

   /*
    ------------------------------------------------------------------------------------------
    */

    //std::cout << type_name<decltype(architecture)>() << '\n';

/*
    ------------------------------------------------------------------------------------------
    */

    /* TEST Net STRUCTURE
    auto net = std::make_shared<Net>();

    for (auto& param : net->named_modules()){
    	std::cout << "Module Name: " << param.key() << std::endl;
      std::cout << "Module Instance: " << param.value().get() << std::endl;
    }

    string model_path = "net.csv";
    torch::serialize::OutputArchive output_archive;
    net->save(output_archive);
    output_archive.save_to(model_path);*/



    /*auto lstm = torch::nn::LSTM(3, 3);
    torch::Tensor inputs[] = torch::randn({1, 3, 5});
    torch::Tensor hidden[2] = { torch::randn(1,1,3), torch::randn(1,1,3) };

    for (auto& i : inputs) {
      auto out
    }*/

    /*
    ------------------------------------------------------------------------------------------
    */

    /*
      TEST BLSTM_Model STRUCTURE
    BLSTM_Model model = BLSTM_Model(LAYERS, HIDDEN, INPUTS);
    torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.0001));
    //Input
    torch::Tensor input = torch::empty({ SEQUENCE, INPUTS });
    auto input_acc = input.accessor<float, 2>();
    size_t count = 0;
    for (float i = 0.1; i < 0.4; i+=0.1) {
      input_acc[count][0] = i;
      count++;
    }
    //Target
    torch::Tensor target = torch::empty({ SEQUENCE, OUTPUTS });
    auto target_acc = target.accessor<float, 2>();
    count = 0;
    for (float i = 0.4; i < 0.7; i+=0.1) {
      target_acc[count][0] = i;
      count++;
    }
    //Train
    for (size_t i = 0; i < 6000; i++) {
      torch::Tensor output = model.forward(input);
      auto loss = torch::mse_loss(output.view({ SEQUENCE, OUTPUTS }), target);
      std::cout << "Loss " << i << " : " << loss.item<float>() << std::endl;
      loss.backward();
      optimizer.step();
    }
    //Test: Response should be about (0.4, 0.5, 0.6)
    torch::Tensor output = model.forward(input);
    std::cout << output << std::endl;
    */

    /*
    ------------------------------------------------------------------------------------------
    */

    
    
    return 0;

}
