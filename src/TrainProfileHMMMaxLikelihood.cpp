/*
 *       TrainProfileHMMMaxLikelihood.cpp
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

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainProfileHMMMaxLikelihood.hpp"
#include "ProfileHiddenMarkovModel.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
namespace tops {

  ProbabilisticModelPtr TrainProfileHMMMaxLikelihood::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr statespar = parameters.getMandatoryParameterValue("states_training_set");
    ProbabilisticModelParameterValuePtr emisspar = parameters.getMandatoryParameterValue("emissions_training_set");
    ProbabilisticModelParameterValuePtr pseudopar = parameters.getOptionalParameterValue("pseudocounts");

    int pseudocounts = 1;
    if(pseudopar != NULL)
      pseudocounts = pseudopar->getInt();

    ProbabilisticModelCreatorClient creator;
    std::string name = initmodelpar->getString();
    ProbabilisticModelPtr m = creator.create(name);
    SequenceEntryList states_sample_set, emiss_sample_set;
    AlphabetPtr emiss_alphabet = m->alphabet();
    AlphabetPtr states_alphabet = m->getStateNames();
    readSequencesFromFile(states_sample_set, states_alphabet, statespar->getString());
    readSequencesFromFile(emiss_sample_set, emiss_alphabet, emisspar->getString());


    SequenceList seqs1, seqs2;
    for(int i = 0; i < (int)states_sample_set.size(); i++)
      seqs1.push_back(states_sample_set[i]->getSequence());
    for(int i = 0; i < (int)emiss_sample_set.size(); i++)
      seqs2.push_back(emiss_sample_set[i]->getSequence());
    m->profileDecodable()->trainMaxLikelihood(seqs1, seqs2, pseudocounts);
    return m;
  }
};



