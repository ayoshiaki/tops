/*
 *       TrainPHMMBaumWelch.cpp
 *
 *       Copyright 2011 Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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
#include "TrainPHMMBaumWelch.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "PairHiddenMarkovModel.hpp"
namespace tops {

  ProbabilisticModelPtr TrainPHMMBaumWelch::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr trainpar1 = parameters.getMandatoryParameterValue("training_set1");
    ProbabilisticModelParameterValuePtr trainpar2 = parameters.getMandatoryParameterValue("training_set2");
    ProbabilisticModelParameterValuePtr thrpar = parameters.getOptionalParameterValue("threshold");
    ProbabilisticModelParameterValuePtr maxiterpar = parameters.getOptionalParameterValue("maxiter");
    double threshold = 1e-5;
    if(thrpar != NULL)
      threshold = thrpar->getDouble();
    int maxiter = 500;
    if(maxiterpar != NULL)
      maxiter = maxiterpar->getInt();

    ProbabilisticModelCreatorClient creator;
    std::string name = initmodelpar->getString();
    ProbabilisticModelPtr m = creator.create(name);
    SequenceEntryList sample_set1, sample_set2;
    AlphabetPtr alphabet = m->alphabet();
    readSequencesFromFile(sample_set1, alphabet, trainpar1->getString());
    readSequencesFromFile(sample_set2, alphabet, trainpar2->getString());
    SequenceList seqs1, seqs2;
    for(int i = 0; i < (int)sample_set1.size(); i++)
      seqs1.push_back(sample_set1[i]->getSequence());
    for(int i = 0; i < (int)sample_set2.size(); i++)
      seqs2.push_back(sample_set2[i]->getSequence());
    m->pairDecodable()->trainBaumWelch(seqs1, seqs2, maxiter, threshold);
    return m;
  }
};
