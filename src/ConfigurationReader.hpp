/*
 *       ConfigurationReader.hpp
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

#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include <boost/shared_ptr.hpp>

#include "crossplatform.hpp"

#include <iostream>
#include <string>

#include <torch/torch.h>
#include <ATen/ATen.h>

using namespace std;


namespace tops {

  class DLLEXPORT Alphabet;
  class DLLEXPORT ProbabilisticModelParameters;

  class DLLEXPORT ProbabilisticModelParameterValue;
  typedef boost::shared_ptr<ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  typedef boost::shared_ptr<ProbabilisticModelParameters> ProbabilisticModelParametersPtr;

  //! This class  reads a configuration file
  class DLLEXPORT ConfigurationReader {
  public:

    ConfigurationReader();
    //! Loads the configuration from the data string
    bool load( std::string & data);

    //! Loads the configuration from a file
    bool loadFromFile( const std::string & filename);

    void setCurrentParameterValue(ProbabilisticModelParameterValuePtr value);

    ProbabilisticModelParameterValuePtr getCurrentParameterValue();

    std::string getCurrentParameterName();
    void setCurrentParameterName(const std::string & name);

    ProbabilisticModelParametersPtr parameters();
    void add_parameter();

    std::string getAuxString();
    std::string getAuxString2();
    std::string getAuxString3();

    void setAuxString(const std::string & aux) ;
    void setAuxString2(const std::string & aux);
    void setAuxString3(const std::string & aux);

    int getCurrentLayer();
    void IncCurrentLayer();
    std::string getAuxLayer();
    void setAuxLayer(const std::string & aux);
    
    void setParametersLayer();
    int getValueParametersLayer(const std::string & parameter);
    
    template<size_t D>
    torch::ExpandingArray<D> getVectorValuesParametersLayer(const std::string & parameter);
    
    void UpdateParametersLayer();
    
    void setNewOptionalParameterLayer(const std::string & parameter);
    
    std::string getAuxParameterName();
    void setAuxParameterName(const std::string & aux);
    
    void addValueAuxParametersValues(const int value);
    vector<int> getAuxParametersValues();
    void resetAuxParametersValues();
    
    std::shared_ptr<torch::nn::Module> getAuxModuleLayers();

    void showParameters();

    void reset();

  private:
    ProbabilisticModelParametersPtr _parameters;
    ProbabilisticModelParameterValuePtr _current_value;
    std::string  _current_name;
    std::string _aux_string;
    std::string _aux_string_2;
    std::string _aux_string_3;
    
    int _currentLayer;
    std::string _aux_layer; // name to register the layer e.g. Conv1d, Conv2d
    std::string _aux_parameter_name; // parameter's name of a layer e.g. kernel_size, padding, stride
    vector<int> _aux_parameters_values; // list of values of an optional parameter
    torch::nn::Module _aux_module_layers;
    std::shared_ptr<torch::nn::Module> _ptr_aux_module_layers;
    map<std::string, vector<int>> _parameters_layer;
  };

}

#endif
