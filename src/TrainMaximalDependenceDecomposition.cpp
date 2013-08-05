/*
 *       TrainMaximalDependenceDecomposition.cpp
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

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainMaximalDependenceDecomposition.hpp"
#include "MaximalDependenceDecomposition.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"
#include <boost/algorithm/string.hpp>

namespace tops {

  ProbabilisticModelPtr TrainMaximalDependenceDecomposition::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr alphabet_parameter = parameters.getOptionalParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr consensus_parameter = parameters.getOptionalParameterValue("consensus");
    ProbabilisticModelParameterValuePtr consensus_model_parameter = parameters.getOptionalParameterValue("consensus_model");
    ProbabilisticModelParameterValuePtr training_set_parameter = parameters.getOptionalParameterValue("training_set");

    if(alphabet_parameter == NULL || consensus_parameter == NULL || consensus_model_parameter == NULL || training_set_parameter == NULL) {
      std::cerr << "ERROR: initial_specification is a mandatory paramenter\n" << std::endl;
      return MaximalDependenceDecompositionPtr();
    } else {
      MaximalDependenceDecompositionPtr mdd = MaximalDependenceDecompositionPtr(new MaximalDependenceDecomposition());
      AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
      alphabet->initializeFromVector(alphabet_parameter->getStringVector());
      mdd->setAlphabet(alphabet);

      std::vector<std::string> consensus_symbols = consensus_parameter->getStringVector();
      ConsensusSequence consensus_sequence;
      for (int i = 0; i < consensus_symbols.size(); i++) {
        std::vector<std::string> syms;
        boost::split(syms, consensus_symbols[i], boost::is_any_of(" "));
        vector<int> s;
        for (int j = 0; j < syms.size(); j++) {
          s.push_back(alphabet->getSymbol(syms[j])->id());
        }
        Consensus cons(s);
        consensus_sequence.push_back(cons);
      }
      mdd->setConsensusSequence(consensus_sequence);

      std::string consensus_model_str = consensus_model_parameter->getString();
      consensus_model_str = consensus_model_str.substr(1, consensus_model_str.size() - 2);
      ConfigurationReader consensus_model_reader;
      ProbabilisticModelCreatorClient consensus_model_creator;
      consensus_model_reader.load(consensus_model_str);
      mdd->setConsensusModel(consensus_model_creator.create(*(consensus_model_reader.parameters())));

      SequenceEntryList sample_set;
      readSequencesFromFile(sample_set, alphabet, training_set_parameter->getString());
      
      mdd->train(sample_set, 2);

      return mdd;
    }
  }
};



