/*
 *       NeuralNetworkModelCreator.cpp
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

#include "NeuralNetworkModelCreator.hpp"
#include "ConfigurationReader.hpp"
//#include "NeuralNetworkModel.hpp"
#include "Alphabet.hpp"
namespace tops {

  ProbabilisticModelPtr NeuralNetworkModelCreator::create(ProbabilisticModelParameters & parameters) const {
    
    //ProbabilisticModelParameterValuePtr weight = parameters.getMandatoryParameterValue("weight");
    //ProbabilisticModelParameterValuePtr bias = parameters.getMandatoryParameterValue("bias");
    ProbabilisticModelParameterValuePtr layers = parameters.getMandatoryParameterValue("layers");
    if(layers == NULL) {
        std::cerr << help();
    }
    NeuralNetworkModelPtr model = NeuralNetworkModelPtr(new NeuralNetworkModel());
    model->initialize(parameters);
    return model;
  }
}
