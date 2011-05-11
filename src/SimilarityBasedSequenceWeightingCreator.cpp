/*
 *       SimilarityBasedSequenceWeightingCreator.cpp
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

#include "SimilarityBasedSequenceWeightingCreator.hpp"
#include "SimilarityBasedSequenceWeighting.hpp"
namespace tops {
  ProbabilisticModelPtr SimilarityBasedSequenceWeightingCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr alphabet = parameters.getOptionalParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr counter = parameters.getOptionalParameterValue("counter");
    ProbabilisticModelParameterValuePtr normalizer = parameters.getOptionalParameterValue("normalizer");


    SimilarityBasedSequenceWeightingPtr model = SimilarityBasedSequenceWeightingPtr(new SimilarityBasedSequenceWeighting());

    if(( alphabet == NULL ) ||
       (counter == NULL) ||
       (normalizer == NULL) )
      {
        return model;
      }

    if (alphabet != NULL)
      {
        AlphabetPtr alpha = AlphabetPtr(new Alphabet());
        alpha->initializeFromVector(alphabet->getStringVector());
        model->setAlphabet(alpha);
      } else
      {
        std::cerr << help() << std::endl;
        exit(-1);
      }
    model->initialize(parameters);
    return model;
  }
}
