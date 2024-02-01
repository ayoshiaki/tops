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
    add_prob(ConfigurationReader *c) : _c(c){};
    void operator()(double n) const
    {
      (((_c->getCurrentParameterValue()->getDoubleMap()).find(_c->getAuxString()))->second) = n;
      //      std::cerr << "ADD_PROB_ELEMENT: "  << n  << std::endl;
    }
  private:
    ConfigurationReader * _c;
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

  struct create_Convolution_layer
  {
    create_Convolution_layer(ConfigurationReader *c) : _c(c){};
    template<typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
      if(_c->getAuxLayer() == "Conv1d"){
        //_c->showParameters();
        torch::nn::Conv1dOptions conv_options = torch::nn::Conv1dOptions(
        /*in_channels=*/_c->getValueParametersLayer("in_channels"),  
        /*out_channels=*/_c->getValueParametersLayer("out_channels"),
        /*kernel_size=*/_c->getVectorValuesParametersLayer<1>("kernel_size"))
        .stride(_c->getVectorValuesParametersLayer<1>("stride"))
        .padding(_c->getVectorValuesParametersLayer<1>("padding"))
        .dilation(_c->getVectorValuesParametersLayer<1>("dilation"))
        .groups(_c->getValueParametersLayer("groups"))
        .bias(_c->getValueParametersLayer("bias"))
        .padding_mode(torch::kCircular);
        //.output_padding(_c->getValueParametersLayer("output_padding"));
        //.transposed(_c->getValueParametersLayer("transposed")); 

        //(_c->getCurrentParameterValue()->getModule()).register_module("conv" + std::to_string(_c->getCurrentLayer()), torch::nn::Conv1d(conv_options));
        _c->getAuxModuleLayers()->push_back(torch::nn::Conv1d(conv_options));        
        
        _c->IncCurrentLayer();
      }
      else if(_c->getAuxLayer() == "Conv2d"){
        torch::nn::Conv2dOptions conv_options = torch::nn::Conv2dOptions(
        /*in_channels=*/_c->getValueParametersLayer("in_channels"),  
        /*out_channels=*/_c->getValueParametersLayer("out_channels"),
        /*kernel_size=*/_c->getVectorValuesParametersLayer<2>("kernel_size"))
        .stride(_c->getVectorValuesParametersLayer<2>("stride"))
        .padding(_c->getVectorValuesParametersLayer<2>("padding"))
        .dilation(_c->getVectorValuesParametersLayer<2>("dilation"))
        .groups(_c->getValueParametersLayer("groups"))
        .bias(_c->getValueParametersLayer("bias"))
        .padding_mode(torch::kCircular);
        //.output_padding(_c->getValueParametersLayer("output_padding"));
        //.transposed(_c->getValueParametersLayer("transposed"));

        //(_c->getCurrentParameterValue()->getModule()).register_module("conv" + std::to_string(_c->getCurrentLayer()), torch::nn::Conv2d(conv_options));
        _c->getAuxModuleLayers()->push_back(torch::nn::Conv2d(conv_options));
        
        _c->IncCurrentLayer();
      }
    }
  private:
    ConfigurationReader * _c;
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
      std::cerr << "Cant open file " << filename << std::endl;
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
      pooling_layer,
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
      >> real_p [add_prob(this)]

      >> *( ch_p(';')
            >> '"'
            >> (+ word_p)  [set_first_word(this)]
            >> '"'
            >>
            (  (ch_p('|')  >> '"' >> ( *word_p )[set_second_word(this)] >> '"' >> ':' )
               |
               ch_p(':')  )  [create_transition_entry(this)]
            >> real_p [add_prob(this)] )
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
                >> (int_p [push_back_a(_parameters_layer["kernel_size"])]
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

    /* e.g. MaxPool1d(100, 200, 4); MaxPool1d(100, 200, (4, 5)) */
    pooling_layer
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
        )
      >> ch_p('(')
                >> int_p /* in_channels */ >> ','
                >> int_p /* out_channels */ >> ','
                >> (int_p | tuple_p) /* kernel_size */
      >> ')'
      ;

    layer_p /* kind of layer */
      = convolutional_layer[create_Convolution_layer(this)]
      | pooling_layer
      //| activation_layer
      //| normalization_layer
      //| recurrent_layer
      //| linear_layer
      //| dropout_layer
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

  void ConfigurationReader::showParameters(){
    std::cerr << "--------------------------\n";
    for (auto const& entry : _parameters_layer){
      std::cerr << "\n" << entry.first << ": \n";
      for (size_t i = 0; i < (entry.second).size(); i++){
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
    setParametersLayer();
  }
};
