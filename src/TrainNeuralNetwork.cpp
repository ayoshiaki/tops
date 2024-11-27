/*
 *       TrainVariableLengthMarkovChain.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
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

#include "util.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "NeuralNetworkModel.hpp"
#include "TrainNeuralNetwork.hpp"

#include <iostream>
#include <fstream>
#include <set>

namespace tops {


ProbabilisticModelPtr TrainNeuralNetwork::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr init_model_param = parameters.getOptionalParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr initspecificationpar = parameters.getOptionalParameterValue("initial_specification");

    ProbabilisticModelParameterValuePtr train_set_param = parameters.getMandatoryParameterValue("training_set");
    
    ProbabilisticModelParameterValuePtr epochs_param = parameters.getOptionalParameterValue("epochs");
    ProbabilisticModelParameterValuePtr batch_size_param = parameters.getOptionalParameterValue("batch_size");
    ProbabilisticModelParameterValuePtr learning_rate_param = parameters.getOptionalParameterValue("learning_rate");

    if(initspecificationpar != NULL) 
	    init_model_param = initspecificationpar;
    if((initspecificationpar == NULL) && (init_model_param == NULL)) 
	    std::cerr << "ERROR: initial_specification is a mandatory paramenter\n" << std::endl;
    
    int epochs = 100;
    if(epochs_param != NULL)
      epochs = epochs_param->getInt();

    int batch_size = 64;
    if(batch_size_param != NULL)
      batch_size = batch_size_param->getInt();
    
    double learning_rate = 0.01;
    if(learning_rate_param != NULL)
      learning_rate = learning_rate_param->getDouble();

    ProbabilisticModelCreatorClient creator;
    std::string name = init_model_param->getString();
    ProbabilisticModelPtr m = creator.create(name);
    
    AlphabetPtr alphabet = m->alphabet();

    // how to train sequences ??????
    SequenceEntryList sample_set;    
    readSequencesFromFile(sample_set, alphabet, train_set_param->getString());
    SequenceList seqs;
    for(int i = 0; i < (int)sample_set.size(); i++)
      seqs.push_back(sample_set[i]->getSequence());
    // ??????

    double loss_value = m->trainSGDAlgorithm(seqs, epochs, batch_size, learning_rate);
    std::cout << loss_value << std::endl;

    return m;
}
/*
//! Provides a help
std::string TrainNeuralNetwork::help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\ntraining_set" << std::endl;
      out << "\talphabet" << std::endl;      
      out << "Example: " << std::endl;
      out << "\ttraining_algorithm=\"SGDAlgorithm\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\ttraining_set= \"input.seq" << std::endl;      
      return out.str();
}*/

}
;

