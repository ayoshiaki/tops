#include <torch/torch.h>
#include <boost/program_options.hpp>

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

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
struct Net : torch::nn::Module {
  Net() {
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
  public:
    EmptyModule() {
        ptr_mymodule = std::make_shared<torch::nn::Module>(mymodule);      
        ptr_mymodule->register_module("conv_layer1", torch::nn::Conv2d(3, 64, 3));
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
    torch::nn::Module mymodule;
    std::shared_ptr<torch::nn::Module> ptr_mymodule;
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

int main (int argc, char ** argv) {
    
    srand(time(NULL));

    //torch::Tensor tensor = torch::rand({2, 3, 1});
    //std::cout << tensor << std::endl;

    EmptyModule emptymodule;
    emptymodule.RegisterModule("conv_layer2");
    emptymodule.RegisterModule("conv_layer3");
    
    emptymodule.ShowModule();

    torch::nn::Module architecture = emptymodule.getModule();
    
    Rede rede = Rede(architecture);

    //std::cout << type_name<decltype(architecture)>() << '\n';



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

    /* TEST Neural Network Creator */
    /*options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
      ("length,l", value<int> (), "length of the sequences")
      ("numseq,n", value<int> (), "number of sequences")
      ("output,o", value<string>(), "file to store  the sequences")
      ("hidden_states,h", "outputs the hidden state sequences")
      ("fasta,F",  "use fasta format");
    try
    {      
      variables_map vm;
      store(parse_command_line(argc, argv, desc), vm);
      notify(vm);

      int nseq =  vm["numseq"].as<int>();
      int length = vm["length"].as<int>();
      string model_file = vm["model"].as<string>();
      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr model = creator.create(model_file);

      std::cout << model->str() << std::endl;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }*/
    
    return 0;

}
