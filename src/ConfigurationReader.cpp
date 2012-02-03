/*
 *       ConfigurationReader.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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
#include  <boost/spirit/core.hpp>
#include <fstream>
#include "util.hpp"
using namespace boost::spirit;
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
      //      std::cerr << "NAME: "  << str << std::endl;
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

  bool  ConfigurationReader::load(std::string & data)
  {

    rule<phrase_scanner_t> config_file, parameter_spec, parameter_value,
      parameter_name,  prob_table, string_vector, double_vector,
      int_vector, word, word_p, string_map, transition_map, nested_configuration, nested_parameter_spec;
    word_p
      = lexeme_d [ +(alnum_p | (ch_p('_') | '.' | '/' | '-' | ' ' | ',' | '+' ))]
      ;
    word
      = ch_p('"')
      >> word_p
      >> ch_p('"')
      ;
    double_vector
      = ch_p('(')
      >> real_p[create_double_vector(this)]
      >> * (',' >>  real_p[add_value_to_double_vector(this)])
      >> ')'
      ;
    string_vector
      = ch_p('(')
      >> word[create_string_vector(this)]
      >> * (',' >>  word[add_value_to_string_vector(this)])
      >> ')'
      ;
    transition_map
      = ch_p('(')
      >> '"'
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
    parameter_name
      = lexeme_d [ alpha_p >> *(alnum_p | (ch_p('_') | '.' | '/'))]
      ;
    parameter_value
      = double_vector
      | parameter_name [set_parameter_value_string(this)]
      | word  [set_parameter_value_word(this)]
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

  void ConfigurationReader::reset() {
    ProbabilisticModelParameterValuePtr a;
    _current_value =a ;
    _current_name = "";
    _aux_string = "";
    _aux_string_2 = "";
    _aux_string_3  = "";

  }
};

