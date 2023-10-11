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

    NeuralNetworkModel::NeuralNetworkModel(torch::nn::Module module_nn){
        _module_nn = module_nn;
    }

    std::string NeuralNetworkModel::str () const {
        std::stringstream out;
        out << "model_name = \"NeuralNetworkModel\"\n" ;
        out << "module = " << _module_nn << "\n";
        return out.str();
    }

    void NeuralNetworkModel::setParameters(torch::nn::Module module_nn) {
        _module_nn = module_nn;
    }

    ProbabilisticModelCreatorPtr NeuralNetworkModel::getFactory () const{
        return NeuralNetworkModelCreatorPtr(new NeuralNetworkModelCreator());
    }

    void NeuralNetworkModel::initialize(const ProbabilisticModelParameters & p) {
        //ProbabilisticModelParameterValuePtr weight = p.getMandatoryParameterValue("weight");
        //ProbabilisticModelParameterValuePtr bias = p.getMandatoryParameterValue("bias");
        ProbabilisticModelParameterValuePtr module_nn = p.getMandatoryParameterValue("module_nn");
        
        setParameters(module_nn->getModule());
    }

    ProbabilisticModelParameters NeuralNetworkModel::parameters () const {
        
        ProbabilisticModelParameters par;
        par.add("model_name", StringParameterValuePtr(new StringParameterValue("NeuralNetworkModel")));
        torch::nn::Module module_nn;
        par.add("module_nn", ModuleParameterValuePtr(new ModuleParameterValue(module_nn)));
        return par;
    }

    
    double NeuralNetworkModel::choose() const {
        //std::cerr << "Not implemented" << std::endl;
        return 0.0;
    }

    Sequence & NeuralNetworkModel::choose(Sequence & s, int size ) const {
        return s;
    }

}


