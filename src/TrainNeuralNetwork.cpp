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

#include "ContextTree.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "NeuralNetworkModel.hpp"
#include "TrainNeuralNetwork.hpp"

#include <iostream>
#include <fstream>
#include <set>

namespace tops {

ProbabilisticModelPtr TrainNeuralNetwork::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getOptionalParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr initspecificationpar = parameters.getOptionalParameterValue("initial_specification");

    ProbabilisticModelParameterValuePtr trainpar = parameters.getMandatoryParameterValue("training_set");
    //ProbabilisticModelParameterValuePtr thrpar = parameters.getOptionalParameterValue("threshold");
    ProbabilisticModelParameterValuePtr maxiterpar = parameters.getOptionalParameterValue("maxiter");

    if(initspecificationpar != NULL) 
	    initmodelpar = initspecificationpar;
    if((initspecificationpar == NULL) && (initmodelpar == NULL)) 
	    std::cerr << "ERROR: initial_specification is a mandatory paramenter\n" << std::endl;

    double threshold = 1e-5;
    if(thrpar != NULL)
      threshold = thrpar->getDouble();
    int maxiter = 500;
    if(maxiterpar != NULL)
      maxiter = maxiterpar->getInt();

    ProbabilisticModelCreatorClient creator;
    std::string name = initmodelpar->getString();
    ProbabilisticModelPtr m = creator.create(name);
    SequenceEntryList sample_set;
    AlphabetPtr alphabet = m->alphabet();
    readSequencesFromFile(sample_set, alphabet, trainpar->getString());
    SequenceList seqs;
    for(int i = 0; i < (int)sample_set.size(); i++)
      seqs.push_back(sample_set[i]->getSequence());
    m->trainBaumWelch(seqs, maxiter, threshold);
    return m;
}

//! Provides a help
std::string TrainNeuralNetwork::help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\ntraining_set" << std::endl;
      out << "\talphabet" << std::endl;
      out << "\tcut" << std::endl;
      out << "Example: " << std::endl;
      out << "\ttraining_algorithm=\"ContextAlgorithm\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\ttraining_set= \"input.seq" << std::endl;
      out << "\tcut=1" << std::endl;
      return out.str();
}

}
;

