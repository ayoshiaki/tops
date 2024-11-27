/*
 *       ConfigurationReader.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bonadio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
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

#include "ConfigurationReader.hpp"
#include "ProbabilisticModelParameter.hpp"
#include <boost/spirit/include/classic_core.hpp>
#include <fstream>
#include "util.hpp"

using namespace boost::spirit::classic;
namespace tops {

  struct store_parameter
  {
    store_parameter(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->add_parameter();
      _c->reset();
    }
  private:
    ConfigurationReader * _c;
  };

  struct set_first_word
  {
    set_first_word(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setAuxString2(str);
    }
  private:
    ConfigurationReader * _c;
  };

  struct set_second_word
  {
    set_second_word(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setAuxString3(str);
    }
  private:
    ConfigurationReader * _c;
  };



  struct set_parameter_name
  {
    set_parameter_name(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setCurrentParameterName(str);

      //std::cerr << "PARAMETER NAME: "  << str << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };

  struct set_parameter_value_word
  {
    set_parameter_value_word(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      StringParameterValuePtr value = StringParameterValuePtr (new StringParameterValue(str));
      _c->setCurrentParameterValue(value);
      //      std::cerr << "STRING: "  << str << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };


  struct set_parameter_value_string
  {
    set_parameter_value_string(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      StringParameterValuePtr value = StringParameterValuePtr (new StringParameterValue(str));
      _c->setCurrentParameterValue(value);
      //      std::cerr << "STRING: "  << str << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };

  struct set_parameter_value_int
  {
    set_parameter_value_int(ConfigurationReader *c) : _c(c){};
    void operator()(int num) const
    {
      IntParameterValuePtr value = IntParameterValuePtr (new IntParameterValue(num));
      _c->setCurrentParameterValue(value);
      //      std::cerr << "INT: "  << num << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };


  struct set_parameter_value_double
  {
    set_parameter_value_double(ConfigurationReader *c) : _c(c){};
    void operator()(double num) const
    {
      DoubleParameterValuePtr value = DoubleParameterValuePtr (new DoubleParameterValue(num));
      _c->setCurrentParameterValue(value);
      //      std::cerr << "DOUBLE: "  << num << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };


  struct create_double_vector
  {
    create_double_vector(ConfigurationReader *c) : _c(c){};
    void operator()(double n) const
    {
      DoubleVectorParameterValuePtr v = DoubleVectorParameterValuePtr(new DoubleVectorParameterValue());
      _c->setCurrentParameterValue(v);
      (v->getDoubleVector()).push_back(n);
      //      std::cerr << "DOUBLE_VECTOR_FIRST: " << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };
  struct add_value_to_double_vector
  {
    add_value_to_double_vector(ConfigurationReader *c) : _c(c){};
    void operator()(double n) const
    {
      (_c->getCurrentParameterValue()->getDoubleVector()).push_back(n);
      //      std::cerr << "ADD_DOUBLE_VECTOR: "  << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };

  struct create_int_vector
  {
    create_int_vector(ConfigurationReader *c) : _c(c){};
    void operator()(int n) const
    {
      IntVectorParameterValuePtr v = IntVectorParameterValuePtr(new IntVectorParameterValue());
      _c->setCurrentParameterValue(v);
      (v->getIntVector()).push_back(n);
      //      std::cerr << "CREATE_INT_VECTOR: " << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };
  struct add_value_to_int_vector
  {
    add_value_to_int_vector(ConfigurationReader *c) : _c(c){};
    void operator()(int n) const
    {
      (_c->getCurrentParameterValue()->getIntVector()).push_back(n);
      //      std::cerr << "ADD_INT_VECTOR: "  << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };

  struct create_string_vector
  {
    create_string_vector(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      StringVectorParameterValuePtr v = StringVectorParameterValuePtr(new StringVectorParameterValue());
      _c->setCurrentParameterValue(v);
      (v->getStringVector()).push_back(str);
      //      std::cerr << "CREATE_STRING_VECTOR: " << str  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };
  struct add_value_to_string_vector
  {
    add_value_to_string_vector(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      (_c->getCurrentParameterValue()->getStringVector()).push_back(str);
      //      std::cerr << "ADD_STRING_VECTOR: "  << str  << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };


  struct create_prob_table_entry
  {
    create_prob_table_entry(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-2, std::back_inserter(str));

      double v = 0.0;
      if( (_c->getCurrentParameterValue()->getDoubleMap()).find(str) == (_c->getCurrentParameterValue()->getDoubleMap()).end()) {
        (_c->getCurrentParameterValue()->getDoubleMap())[str] = v;
      }
      _c->setAuxString(str);
    }
  private:
    ConfigurationReader * _c;
  };


  struct create_prob_table
  {
    create_prob_table(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      DoubleMapParameterValuePtr probTable =
        DoubleMapParameterValuePtr(new DoubleMapParameterValue());
      std::string str;
      std::copy(first+1, last-2, std::back_inserter(str));

      double v =0.0;
      if( (probTable->getDoubleMap()).find(str) == (probTable->getDoubleMap()).end()) {
        (probTable->getDoubleMap())[str] = v;
      }
      _c->setAuxString(str);
      _c->setCurrentParameterValue(probTable);
      //      std::cerr << "ADD_TABLE_ENTRY: "  << str << std::endl;
    }
  private:
    ConfigurationReader * _c;
  };


  struct add_prob
  {
    add_prob(ConfigurationReader *c, bool _islayer = false) : _c(c){ islayer = _islayer; };
    void operator()(double n) const
    {
      if(!islayer)
        (((_c->getCurrentParameterValue()->getDoubleMap()).find(_c->getAuxString()))->second) = n;
      else
        _c->setProbabilityValue(n);
      //      std::cerr << "ADD_PROB_ELEMENT: "  << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
    bool islayer;
  };



  struct create_string_map
  {
    create_string_map(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      StringMapParameterValuePtr str_map =
        StringMapParameterValuePtr(new StringMapParameterValue());
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      std::string v;
      if( (str_map->getStringMap()).find(str) == (str_map->getStringMap()).end()) {
        (str_map->getStringMap())[str] = v;
      }
      _c->setAuxString(str);
      _c->setCurrentParameterValue(str_map);
    }
  private:
    ConfigurationReader * _c;
  };

  struct add_str_map
  {
    add_str_map(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      (_c->getCurrentParameterValue()->getStringMap() )[_c->getAuxString()] = str;
    }
  private:
    ConfigurationReader * _c;
  };



  struct add_new_map
  {
    add_new_map(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first+1, last-1, std::back_inserter(str));
      std::string v;
      if( (_c->getCurrentParameterValue()->getStringMap()).find(str) == (_c->getCurrentParameterValue()->getStringMap()).end()) {
        (_c->getCurrentParameterValue()->getStringMap())[str] = v;
      }
      _c->setAuxString(str);
    }
  private:
    ConfigurationReader * _c;
  };



  struct create_transition
  {
    create_transition(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      DoubleMapParameterValuePtr m =
        DoubleMapParameterValuePtr(new DoubleMapParameterValue());

      std::string str;
      std::copy(first, last-1, std::back_inserter(str));
      double v = 0.0;

      std::stringstream out;
      std::string symbol(_c->getAuxString2());
      std::string context(_c->getAuxString3());
      tops::trim_spaces(symbol);
      tops::trim_spaces(context);
      if(context.size() == 0)
        out << symbol ;
      else
        out << symbol << "|" << context ;
      str = out.str();



      if( (m->getDoubleMap()).find(str) == (m->getDoubleMap()).end()) {
        (m->getDoubleMap())[str] = v;
      }
      _c->setAuxString(str);
      _c->setCurrentParameterValue(m);
    }
  private:
    ConfigurationReader * _c;
  };

  struct create_transition_entry
  {
    create_transition_entry(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last-1, std::back_inserter(str));
      double v = 0.0;

      std::stringstream out;
      std::string symbol(_c->getAuxString2());
      std::string context(_c->getAuxString3());
      tops::trim_spaces(symbol);
      tops::trim_spaces(context);
      if(context.size() == 0)
        out << symbol ;
      else
        out << symbol << "|" << context ;
      str = out.str();

      if( (_c->getCurrentParameterValue()->getDoubleMap()).find(str) == (_c->getCurrentParameterValue()->getDoubleMap()).end()) {
        (_c->getCurrentParameterValue()->getDoubleMap())[str] = v;
      }
      _c->setAuxString(str);
      //      std::cerr << "CREATING_TRANSITION_ENTRY: " << str << std::endl;

    }
  private:
    ConfigurationReader * _c;
  };



  struct add_transition_prob
  {
    add_transition_prob(ConfigurationReader *c) : _c(c){};
    void operator()(double n) const
    {
      (((_c->getCurrentParameterValue()->getDoubleMap()).find(_c->getAuxString()))->second) = n;
    }
  private:
    ConfigurationReader * _c;
  };


  /* Deep layer struct parsings */

  struct create_sequential_architecture
  {
    create_sequential_architecture(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      //ModuleParameterValue aux_module = new ModuleParameterValue();
      ModuleParameterValuePtr architecture = ModuleParameterValuePtr(new ModuleParameterValue(std::make_shared<torch::nn::Sequential>(_c->getAuxModuleLayers())));
      _c->setCurrentParameterValue(architecture);

      //std::cerr << "Architecture created\n";
    }
    private:
    ConfigurationReader * _c;
  };

  struct set_optional_parameter_name {
    set_optional_parameter_name(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setAuxParameterName(str);
      //std::cerr << ">> " << _aux_parameter_name << endl;
    }

    private:
      ConfigurationReader * _c;
  };

  struct add_value_to_tuple{
    add_value_to_tuple(ConfigurationReader *c) : _c(c){};
    void operator()(int value) const {
      _c->addValueAuxParametersValues(value);
    }
    private:
      ConfigurationReader * _c;
  };

  struct set_int_layer_optional_parameter{
    set_int_layer_optional_parameter(ConfigurationReader *c) : _c(c){};
    void operator()(int num) const {
      _c->resetAuxParametersValues();
      _c->addValueAuxParametersValues(num);
      _c->UpdateParametersLayer();
    }
  private:
    ConfigurationReader * _c;
  };

  struct set_parameter_tuple{
    set_parameter_tuple(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::string str;
      std::copy(first, last, std::back_inserter(str));

      if(_c->getAuxParameterName() == "") _c->setAuxParameterName("kernel_size");
      //std::cerr << "parameter -> " << _c->getAuxParameterName() << ": ";
      //std::cerr << "<<< " << str << " >>>" << endl;

      _c->UpdateParametersLayer();

      _c->resetAuxParametersValues();
      _c->setAuxParameterName("");
    }
    private:
      ConfigurationReader * _c;
  };

  struct create_optional_layer_parameter
  {
    create_optional_layer_parameter(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setAuxParameterName(str);
      _c->setNewOptionalParameterLayer(str);
    }
    private:
      ConfigurationReader * _c;
  };

  struct start_new_layer {
    start_new_layer(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      _c->setAuxLayer(str); // layer type
      _c->setParametersLayer(); //layer parameters
      _c->setAuxParameterName("");
      _c->resetAuxParametersValues();
    }
    private:
      ConfigurationReader * _c;
  };

  /*
    CREATE AND ADD CONVOLUTIONAL LAYERS
  */
  struct create_Convolution_layer {
    create_Convolution_layer(ConfigurationReader *c) : _c(c){};

    template <typename OptionsType, int Dim>
    OptionsType configureConvOptions() const {
        try {
            OptionsType conv_options = OptionsType(
                _c->getValueParametersLayer("in_channels"),
                _c->getValueParametersLayer("out_channels"),
                _c->getVectorValuesParametersLayer<Dim>("kernel_size"))
                .stride(_c->getVectorValuesParametersLayer<Dim>("stride"))
                .padding(_c->getVectorValuesParametersLayer<Dim>("padding"))
                .dilation(_c->getVectorValuesParametersLayer<Dim>("dilation"))
                .groups(_c->getValueParametersLayer("groups"))
                .bias(_c->getValueParametersLayer("bias"))
                .padding_mode(torch::kCircular);
            return conv_options;
        } catch (const std::exception& e) {
            throw std::runtime_error("Error configuring convolutional options: " + std::string(e.what()));
        }
    }
    void createConv1dLayer() const {
      if (_c->getAuxLayer() == "Conv1d") {
          auto conv_options = configureConvOptions<torch::nn::Conv1dOptions, 1>();
          auto conv_layer = torch::nn::Conv1d(conv_options);
          _c->getAuxModuleLayers()->push_back(conv_layer);
          _c->IncCurrentLayer();
      }
    }

    void createConv2dLayer() const {
        if (_c->getAuxLayer() == "Conv2d") {
            auto conv_options = configureConvOptions<torch::nn::Conv2dOptions, 2>();
            auto conv_layer = torch::nn::Conv2d(conv_options);
            _c->getAuxModuleLayers()->push_back(conv_layer);
            _c->IncCurrentLayer();
        }
    }

    void createConv3dLayer() const {
        if (_c->getAuxLayer() == "Conv3d") {
            auto conv_options = configureConvOptions<torch::nn::Conv3dOptions, 3>();
            auto conv_layer = torch::nn::Conv3d(conv_options);
            _c->getAuxModuleLayers()->push_back(conv_layer);
            _c->IncCurrentLayer();
        }
    }

    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      try {
        const std::string& aux_layer = _c->getAuxLayer();
        if (aux_layer == "Conv1d") {
            createConv1dLayer();
        } else if (aux_layer == "Conv2d") {
            createConv2dLayer();
        } else if (aux_layer == "Conv3d") {
            createConv3dLayer();
        } else {
            throw std::runtime_error("Unsupported convolutional layer type: " + aux_layer);
        }
    } catch (const std::exception& e) {
        // Handle exceptions appropriately
        std::cerr << "Error creating layer: " << e.what() << std::endl;
    }
    }
  private:
    ConfigurationReader * _c;
  };

/*
    CREATE AND ADD POOLING LAYERS
*/
  struct create_Pooling_layer {
    create_Pooling_layer(ConfigurationReader* c) : _c(c) {}

    template <typename OptionsType, int Dim>
    OptionsType configurePoolOptions() const {
        try {
            OptionsType pool_options = OptionsType(
                _c->getVectorValuesParametersLayer<Dim>("kernel_size"))
                .stride(_c->getVectorValuesParametersLayer<Dim>("stride"))
                .padding(_c->getVectorValuesParametersLayer<Dim>("padding"))
                //.dilation(_c->getVectorValuesParametersLayer<Dim>("dilation"))
                .ceil_mode(_c->getValueParametersLayer("ceil_mode"));
                //.return_indices(_c->getValueParametersLayer("return_indices"));
            return pool_options;
        } catch (const std::exception& e) {
            throw std::runtime_error("Error configuring pooling options: " + std::string(e.what()));
        }
    }

    void createMaxPool1dLayer() const {
        if (_c->getAuxLayer() == "MaxPool1d") {
            auto pool_options = configurePoolOptions<torch::nn::MaxPool1dOptions, 1>();
            auto pool_layer = torch::nn::MaxPool1d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    void createMaxPool2dLayer() const {
        if (_c->getAuxLayer() == "MaxPool2d") {
            auto pool_options = configurePoolOptions<torch::nn::MaxPool2dOptions, 2>();
            auto pool_layer = torch::nn::MaxPool2d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    void createMaxPool3dLayer() const {
        if (_c->getAuxLayer() == "MaxPool3d") {
            auto pool_options = configurePoolOptions<torch::nn::MaxPool3dOptions, 3>();
            auto pool_layer = torch::nn::MaxPool3d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    void createAvgPool1dLayer() const {
        if (_c->getAuxLayer() == "AvgPool1d") {
            auto pool_options = configurePoolOptions<torch::nn::AvgPool1dOptions, 1>();
            auto pool_layer = torch::nn::AvgPool1d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    void createAvgPool2dLayer() const {
        if (_c->getAuxLayer() == "AvgPool2d") {
            auto pool_options = configurePoolOptions<torch::nn::AvgPool2dOptions, 2>();
            auto pool_layer = torch::nn::AvgPool2d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    void createAvgPool3dLayer() const {
        if (_c->getAuxLayer() == "AvgPool3d") {
            auto pool_options = configurePoolOptions<torch::nn::AvgPool3dOptions, 3>();
            auto pool_layer = torch::nn::AvgPool3d(pool_options);
            _c->getAuxModuleLayers()->push_back(pool_layer);
            _c->IncCurrentLayer();
        }
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
        try {
            const std::string& aux_layer = _c->getAuxLayer();
            if (aux_layer == "MaxPool1d") {
                createMaxPool1dLayer();
            } else if (aux_layer == "MaxPool2d") {
                createMaxPool2dLayer();
            } else if (aux_layer == "MaxPool3d") {
                createMaxPool3dLayer();
            } else if (aux_layer == "AvgPool1d") {
                createAvgPool1dLayer();
            } else if (aux_layer == "AvgPool2d") {
                createAvgPool2dLayer();
            } else if (aux_layer == "AvgPool3d") {
                createAvgPool3dLayer();
            } else {
                throw std::runtime_error("Unsupported pooling layer type: " + aux_layer);
            }
        } catch (const std::exception& e) {
            // Handle exceptions appropriately
            std::cerr << "Error creating pooling layer: " << e.what() << std::endl;
        }
    }

private:
    ConfigurationReader* _c;
};

/*
    CREATE AND ADD ACTIVATION LAYERS
*/
struct create_Activation_layer {
    create_Activation_layer(ConfigurationReader* c) : _c(c) {}

    void createReLULayer() const {
        if (_c->getAuxLayer() == "ReLU()") {
            auto activation_layer = torch::nn::ReLU(torch::nn::ReLUOptions().inplace(true));
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    void createSigmoidLayer() const {
        if (_c->getAuxLayer() == "Sigmoid()") {
            auto activation_layer = torch::nn::Sigmoid();
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    void createTanhLayer() const {
        if (_c->getAuxLayer() == "Tanh()") {
            auto activation_layer = torch::nn::Tanh();
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    void createLeakyReLULayer() const {
        if (_c->getAuxLayer() == "LeakyReLU()") {
            auto activation_layer = torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().inplace(true));
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    void createELULayer() const {
        if (_c->getAuxLayer() == "ELU()") {
            auto activation_layer = torch::nn::ELU(torch::nn::ELUOptions().inplace(true));
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    void createSoftmaxLayer() const {
        if (_c->getAuxLayer() == "Softmax()") {
            auto activation_layer = torch::nn::Softmax(torch::nn::SoftmaxOptions(1));  // Specify the dimension
            _c->getAuxModuleLayers()->push_back(activation_layer);
            _c->IncCurrentLayer();
        }
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
        try {
            const std::string& aux_layer = _c->getAuxLayer();
            if (aux_layer == "ReLU()") {
                createReLULayer();
            } else if (aux_layer == "Sigmoid()") {
                createSigmoidLayer();
            } else if (aux_layer == "Tanh()") {
                createTanhLayer();
            } else if (aux_layer == "LeakyReLU()") {
                createLeakyReLULayer();
            } else if (aux_layer == "ELU()") {
                createELULayer();
            } else if (aux_layer == "Softmax()" || aux_layer == "Softmax(dim=1)") {
                createSoftmaxLayer();
            } else {
                throw std::runtime_error("Unsupported activation layer type: " + aux_layer);
            }
        } catch (const std::exception& e) {
            // Handle exceptions appropriately
            std::cerr << "Error creating activation layer: " << e.what() << std::endl;
        }
    }

private:
    ConfigurationReader* _c;
};

/*
    CREATE AND ADD LINEAR LAYERS
*/
struct create_Linear_layer {
    create_Linear_layer(ConfigurationReader* c) : _c(c) {}

    void createLinearLayer() const {
        if (_c->getAuxLayer() == "Linear") {
            auto linear_layer = torch::nn::Linear(
                _c->getValueParametersLayer("in_channels"),
                _c->getValueParametersLayer("out_channels"));
            _c->getAuxModuleLayers()->push_back(linear_layer);
            _c->IncCurrentLayer();
        }
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
        try {
            const std::string& aux_layer = _c->getAuxLayer();
            if (aux_layer == "Linear") {
                createLinearLayer();
            } else {
                throw std::runtime_error("Unsupported linear layer type: " + aux_layer);
            }
        } catch (const std::exception& e) {
            // Handle exceptions appropriately
            std::cerr << "Error creating linear layer: " << e.what() << std::endl;
        }
    }

private:
    ConfigurationReader* _c;
};

/*
    CREATE AND ADD DROPOUT LAYERS
*/
struct create_Dropout_layer {
    create_Dropout_layer(ConfigurationReader* c) : _c(c) {}

    void createDropoutLayer() const {
        if (_c->getAuxLayer() == "Dropout") {
            double dropout_prob = _c->getProbabilityValue();
            if(dropout_prob < 0.0) dropout_prob = 0.5;
            auto dropout_layer = torch::nn::Dropout(dropout_prob);
            _c->getAuxModuleLayers()->push_back(dropout_layer);
            _c->IncCurrentLayer();
        }
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
        try {
            const std::string& aux_layer = _c->getAuxLayer();
            if (aux_layer == "Dropout") {
                createDropoutLayer();
            } else {
                throw std::runtime_error("Unsupported dropout layer type: " + aux_layer);
            }
        } catch (const std::exception& e) {
            // Handle exceptions appropriately
            std::cerr << "Error creating dropout layer: " << e.what() << std::endl;
        }
    }

private:
    ConfigurationReader* _c;
};

  /* Deep layer struct parsings */



  struct print_context
  {
    print_context() {};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      std::cerr << " CONTEXT : " << std::endl;
      std::cerr << str << std::endl;
    }
  private:
  };

  bool ConfigurationReader::loadFromFile(const std::string & filename)
  {
    std::ifstream input;
    std::string line;
    input.open(filename.c_str());
    if (!input.is_open()) {
      std::cerr << "Cant open file config " << filename << std::endl;
      exit(-1);
    }
    string conf;
    while (!input.eof()) {
      getline(input, line, '\n');
      line += "\n";
      conf.append(line);
    }
    input.close();
    return load( conf);
  }

/* PRINT ???*/
  struct print_partial_result
  {
    print_partial_result(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      std::string str;
      std::copy(first, last, std::back_inserter(str));
      std::cout << "Partial result: " << str << endl;
    }
  private:
    ConfigurationReader * _c;
  };

  struct PrintIntAction {
    template <typename Iterator, typename Context, typename Skipper, typename Attribute>
    void operator()(Iterator& first, Iterator const& last, Context& /*context*/, Skipper const& /*skipper*/, Attribute& attr) const {
        std::cout << "Parsed integer: " << attr << std::endl;
    }
};
struct PrintAction {
    template <typename Iterator, typename Context, typename Skipper, typename Attribute>
    void operator()(Iterator& first, Iterator const& last, Context& context, Skipper const& skipper, Attribute& attr) const {
        std::cout << "Matched content: " << attr << std::endl;
    }
};



  bool  ConfigurationReader::load(std::string & data)
  {

    rule<phrase_scanner_t> config_file, parameter_spec, parameter_value,
      parameter_name,  prob_table, string_vector, double_vector,
      int_vector, word, word_p, string_map, transition_map, nested_configuration, nested_parameter_spec,
      tree_p, tree,

      layer_vector, layer_p, tuple_p,

      convolutional_layer, conv_creator, conv_parameters,
      pooling_layer, pooling_creator, pooling_parameters,
      activation_layer,
      normalization_layer,
      recurrent_layer,
      linear_layer,
      dropout_layer
      ;

    /* recognizes a string with some special characters: "_./- ,+" (e.g. _Aa1 z) */
    word_p
      = lexeme_d [ +(alnum_p | (ch_p('_') | '.' | '/' | '-' | ' ' | ',' | '+' ))]
      ;

    /* similar to word_p but with parenthesis and : characters (e.g. (_Aa1 z:_Aa1 z:))*/
    tree_p
      = lexeme_d [ +(alnum_p | (ch_p('_') | '.' | '/' | '-' | ' ' | ',' | '+' | '(' | ')' | ':' ))]
      ;

    /* recognizes "word_p" */
    word
      = ch_p('"')
      >> word_p
      >> ch_p('"')
      ;

    /* recognizes "{tree}" */
    tree
      = ch_p('{')
      >> tree_p
      >> ch_p('}')
      ;

    /* recognizes a list of doubles (e.g. (1, 2.6, 0.0001)) */
    double_vector
      = ch_p('(')
      >> real_p[create_double_vector(this)]
      >> * (',' >>  real_p[add_value_to_double_vector(this)])
      >> ')'
      ;

    /* recognizes a list of (the rule) word (e.g ("e.g. _Aa1 z", "A", "a+A"))*/
    string_vector
      = ch_p('(')
      >> word[create_string_vector(this)]
      >> * (',' >>  word[add_value_to_string_vector(this)])
      >> ')'
      ;

    /* recognizes a list of transitions or emission probabilities (e.g ("A"|"B":0.4; "A":0.4; "A b"|"":0.4)) */
    transition_map
      = ch_p('(')
      >> '"' /* word is not enough?? */
      >> ( + word_p)  [set_first_word(this)]
      >> '"'
      >>
      (  (ch_p('|')  >> '"' >> ( *word_p ) [set_second_word(this)] >> '"' >> ':' )
         |
         ch_p(':')  ) [create_transition(this)]
      >> real_p [add_prob(this, false)]

      >> *( ch_p(';')
            >> '"'
            >> (+ word_p)  [set_first_word(this)]
            >> '"'
            >>
            (  (ch_p('|')  >> '"' >> ( *word_p )[set_second_word(this)] >> '"' >> ':' )
               |
               ch_p(':')  )  [create_transition_entry(this)]
            >> real_p [add_prob(this, false)] )
      >> !( ch_p(';') ) >> ')'
      ;

    /* recognizes a map of strings "<key>":"<value>" (e.g ("A" : "B"; "A a B b" : "z"; "1 0" : "0 1")) */
    string_map
      = ch_p('(')
      >> ('"'
          >> +word_p
          >> '"' )[create_string_map(this)]
      >>  ch_p(':')
      >> ('"'
          >> +word_p
          >> '"' ) [add_str_map (this)]
      >> *( ';'
            >> (  '"'
                  >> +word_p
                  >>   '"')[add_new_map(this)]
            >>  ':'
            >> ('"'
                >> +word_p
                >> '"' ) [add_str_map(this)] )
      >> !( ch_p(';') ) >> ')'

      ;

    // *** Deep layer rules

    /* e.g. (1, 2); (1, 2, 3) */
    tuple_p
      = ch_p('(')
      >> int_p [add_value_to_tuple(this)]
      >> + ( ',' >> int_p [add_value_to_tuple(this)] )
      >> ')'
      ;

    /*
      CONVOLUTIONAL LAYER
    */
    /* e.g. Conv2d(100, 200, 4); Conv2d(100, 200, (4, 5)) */

    conv_creator
      = ( str_p("Conv1d")
        | str_p("Conv2d")
        | str_p("Conv3d")
        | str_p("ConvTranspose1d")
        | str_p("ConvTranspose2d")
        | str_p("ConvTranspose3d")
        )[start_new_layer(this)]
        ;

    conv_parameters
      = ch_p('(')
                >> int_p [push_back_a(_parameters_layer["in_channels"])] /* in_channels */ >> ','
                >> int_p [push_back_a(_parameters_layer["out_channels"])] /* out_channels */ >> ','
                >> * (str_p("kernel_size=")) >> (int_p [push_back_a(_parameters_layer["kernel_size"])]
                  | tuple_p [set_parameter_tuple(this)]) /* kernel_size */
                >> * (',' >> word_p [set_optional_parameter_name(this)]
                          >> '='
                          >> (int_p [set_int_layer_optional_parameter(this)]
                            | tuple_p [set_parameter_tuple(this)] ) /* optional parameter */ )
      >> ')'
      ;

    convolutional_layer
      = conv_creator
      >> conv_parameters
      ;

    /*
      POOLING LAYER
    */
    /* e.g. MaxPool1d(100, 200, 4); MaxPool2d(100, 200, (4, 5)) */

    pooling_creator
      = ( str_p("MaxPool1d")
        | str_p("MaxPool2d")
        | str_p("MaxPool3d")
        | str_p("MaxUnpool1d")
        | str_p("MaxUnpool2d")
        | str_p("MaxUnpool3d")
        | str_p("AvgPool1d")
        | str_p("AvgPool2d")
        | str_p("AvgPool3d")
        | str_p("AvgUnpool1d")
        | str_p("AvgUnpool2d")
        | str_p("AvgUnpool3d")
        )[start_new_layer(this)]
      ;

    pooling_parameters
      = ch_p('(')
                >> * (str_p("kernel_size=")) >> (int_p [push_back_a(_parameters_layer["kernel_size"])]
                  | tuple_p [set_parameter_tuple(this)]) /* kernel_size */
                >> * (',' >> word_p [set_optional_parameter_name(this)]
                          >> '='
                          >> (int_p [set_int_layer_optional_parameter(this)]
                            | tuple_p [set_parameter_tuple(this)] ) /* optional parameter */ )
      >> ')'
      ;

    pooling_layer
      = pooling_creator
      >> pooling_parameters
      ;

    /*
      ACTIVATION LAYER
    */
    /* e.g. ReLU, Sigmoid, Tanh, LeakyReLU, ELU, and Softmax */

    activation_layer
      = ( str_p("ReLU()")
        | str_p("Sigmoid()")
        | str_p("Tanh()")
        | str_p("LeakyReLU()")
        | str_p("ELU()")
        | str_p("Softmax(") >> *(str_p("dim=1")) >> ')'
        )[start_new_layer(this)]
      ;

    /*
      LINEAR LAYER
    */
    /* e.g. Linear(20, 30) */

    linear_layer
      = (str_p("Linear"))[start_new_layer(this)]
      >> ch_p('(')
                >> * (str_p("in_features=")) >> int_p [push_back_a(_parameters_layer["in_channels"])] /* in_channels */ >> ','
                >> * (str_p("out_features=")) >> int_p [push_back_a(_parameters_layer["out_channels"])] /* out_channels */
                >> * (',' >> word_p [set_optional_parameter_name(this)]
                          >> '='
                          >> (int_p [set_int_layer_optional_parameter(this)]) /* optional parameter (bias) */ )
      >> ')'
      ;

    /*
      LINEAR LAYER
    */
    /* e.g. Linear(20, 30) */

    dropout_layer
      = (str_p("Dropout"))[start_new_layer(this)]
      >> ch_p('(')
                >> * (str_p("p=")) >> real_p [add_prob(this, true)] /* probability */
      >> * (',' >> str_p("inplace=") >> int_p [push_back_a(_parameters_layer["inplace"])]) >> ')'
      ;

    layer_p /* kind of layer */
      = convolutional_layer[create_Convolution_layer(this)]
      | pooling_layer[create_Pooling_layer(this)]
      | activation_layer[create_Activation_layer(this)]
      //| normalization_layer
      //| recurrent_layer
      | linear_layer[create_Linear_layer(this)]
      | dropout_layer[create_Dropout_layer(this)]
      ;

    layer_vector /* list of layers */
      = ch_p('(')
      >> layer_p
      >> * ( ',' >> layer_p )
      >> ')'
      ;

    // *** Deep layer rules


    parameter_name
      = lexeme_d [ alpha_p >> *(alnum_p | (ch_p('_') | '.' | '/'))]
      ;
    parameter_value
      = layer_vector [create_sequential_architecture(this)]
      | double_vector
      | parameter_name [set_parameter_value_string(this)]
      | word  [set_parameter_value_word(this)]
      | tree  [set_parameter_value_word(this)]
      | string_vector
      | transition_map
      | strict_real_p [set_parameter_value_double(this)]
      | int_p [set_parameter_value_int(this)]
      | nested_configuration [set_parameter_value_string(this)]
      | string_map
      ;

    nested_parameter_spec
      = parameter_name >> '=' >> parameter_value
      ;
    parameter_spec
      = parameter_name[set_parameter_name(this)] >> '=' >> parameter_value ;
      ;
    nested_configuration
      = (ch_p('[') >> +nested_parameter_spec >> ch_p(']'))
      ;
    config_file
      =  parameter_spec[store_parameter(this)] >> (config_file | !end_p)
      ;

    parse_info<> info ;
    bool inComment = false;
    int k = 0;
    for(int i = 0; i < (int)data.size(); i++)
      {
        if (data[i] == '#' )
          inComment = true;
        if(data[i] == '\n')
          inComment = false;
        if(!inComment)
          {
            data[k] = data[i];
            k++;
          }

      }
    data.resize(k);

    info = parse(data.c_str(),  config_file, space_p);
    if(info.full)
      {
        return true;
      }
    else
      {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed " << info.stop <<"\n";
        std::cout << "-------------------------\n";
        return false;
      }

  }

    void ConfigurationReader::setCurrentParameterValue(ProbabilisticModelParameterValuePtr value){
      _current_value = value;
    }
    ProbabilisticModelParameterValuePtr ConfigurationReader::getCurrentParameterValue(){
      return _current_value;
    }
    void ConfigurationReader::setCurrentParameterName(const std::string & name) {
      _current_name = name;
    }

    void ConfigurationReader::setAuxString(const std::string & aux) {
      _aux_string = aux;
    }

    std::string ConfigurationReader::getAuxString(){
      return _aux_string;
    }

    std::string ConfigurationReader::getCurrentParameterName(){
      return _current_name;
    }

    void ConfigurationReader::add_parameter() {
      _parameters->add(_current_name, _current_value);
    }

  ProbabilisticModelParametersPtr ConfigurationReader::parameters(){
    return _parameters;
  }

  ConfigurationReader::ConfigurationReader(){
    _parameters = ProbabilisticModelParametersPtr (new ProbabilisticModelParameters());
    reset();
  }

  std::string ConfigurationReader::getAuxString2(){
    return _aux_string_2;
  }


  std::string ConfigurationReader::getAuxString3(){
    return _aux_string_3;
  }

  void ConfigurationReader::setAuxString2(const std::string & aux){
    _aux_string_2 = aux;
  }

  void ConfigurationReader::setAuxString3(const std::string & aux)
  {
    _aux_string_3 = aux;
  }

  int ConfigurationReader::getCurrentLayer(){
    return _currentLayer;
  }

  void ConfigurationReader::IncCurrentLayer(){
    _currentLayer++;
  }

  std::string ConfigurationReader::getAuxLayer(){
    //std::cout << "AuxLayer Name: " << _aux_layer << std::endl;
    return _aux_layer;
  }

  void ConfigurationReader::setAuxLayer(const std::string & aux){
    _aux_layer = aux;
  }

  void ConfigurationReader::setParametersLayer(){
    _parameters_layer["in_channels"] = {}; //conv, convtrans
    _parameters_layer["out_channels"] = {}; //conv, convtrans
    _parameters_layer["kernel_size"] = {}; //conv, convtrans, pool

    _parameters_layer["stride"] = {1}; //conv, convtrans, pool

    _parameters_layer["padding"] = {0}; //conv, convtrans, pool
    _parameters_layer["padding_mode"] = {0}; //conv (0='zeros', 1='reflect', 2='replicate', 3='circular')
    _parameters_layer["output_padding"] = {0}; //convtrans

    _parameters_layer["dilation"] = {1}; //conv, convtrans, pool
    _parameters_layer["groups"] = {1}; //conv, convtrans
    _parameters_layer["bias"] = {1}; //conv, convtrans
    _parameters_layer["return_indices"] = {0}; //pool, AdaptiveMaxPool1d
    _parameters_layer["ceil_mode"] = {0}; //pool

    _parameters_layer["output_size"] = {0}; //fold, fracMaxPool, AdaptiveMaxPool1d, AdaptiveAvgPool1d

    _parameters_layer["output_ratio"] = {1}; //fracMaxPool (can be a float)??

    _parameters_layer["count_include_pad"] = {0}; //avgpool

    _parameters_layer["power"] = {1}; //LPPool

    //non-linear activations
    _parameters_layer["alpha"] = {1}; //ELU (can be a float)??
    _parameters_layer["inplace"] = {0}; //ELU, RELU
  }

  int ConfigurationReader::getValueParametersLayer(const std::string & parameter){
    return (_parameters_layer[parameter])[0];
  }

  void ConfigurationReader::setProbabilityValue(double value){
    probability_aux = value;
  }
  double ConfigurationReader::getProbabilityValue(){
    return probability_aux;
  }

  void ConfigurationReader::setNewOptionalParameterLayer(const std::string & parameter){
    _parameters_layer[parameter] = {};
  }

  template<size_t D>
  torch::ExpandingArray<D> ConfigurationReader::getVectorValuesParametersLayer(const std::string & parameter){
    const int l = _parameters_layer[parameter].size();
    if(l == 1) return torch::ExpandingArray<D>((_parameters_layer[parameter])[0]);
    else if(l == 2) return torch::ExpandingArray<D>({(_parameters_layer[parameter])[0], (_parameters_layer[parameter])[1]});
    return torch::ExpandingArray<D>({(_parameters_layer[parameter])[0], (_parameters_layer[parameter])[1], (_parameters_layer[parameter])[2]});
  }

  void ConfigurationReader::UpdateParametersLayer(){
    _parameters_layer[_aux_parameter_name] = _aux_parameters_values;
  }

  std::string ConfigurationReader::getAuxParameterName(){
    return _aux_parameter_name;
  }

  void ConfigurationReader::setAuxParameterName(const std::string & aux){
    _aux_parameter_name = aux;
  }

  void ConfigurationReader::addValueAuxParametersValues(const int value){
    _aux_parameters_values.push_back(value);
  }
  vector<int> ConfigurationReader::getAuxParametersValues(){
    return _aux_parameters_values;
  }
  void ConfigurationReader::resetAuxParametersValues(){
    _aux_parameters_values = {};
  }

  torch::nn::Sequential ConfigurationReader::getAuxModuleLayers(){
    return _aux_module_layers;
  }

  void ConfigurationReader::showParameters()
  {
    std::cerr << "--------------------------\n";
    for (auto const &entry : _parameters_layer)
    {
      std::cerr << "\n"
                << entry.first << ": \n";
      for (size_t i = 0; i < (entry.second).size(); i++)
      {
        std::cerr << (entry.second)[i] << ", ";
      }
    }
    std::cerr << "--------------------------\n";
  }

  void ConfigurationReader::reset() {
    ProbabilisticModelParameterValuePtr a;
    _current_value = a;
    _current_name = "";
    _aux_string = "";
    _aux_string_2 = "";
    _aux_string_3  = "";

    _currentLayer = 1;
    _aux_layer = "";
    _aux_parameter_name = "";
    _aux_parameters_values = {};
    _ptr_aux_module_layers = std::make_shared<torch::nn::Sequential>(_aux_module_layers);
    probability_aux = -1.0;
    setParametersLayer();
  }
};
