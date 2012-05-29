/*
 *       TargetModel.cpp
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

#include "TargetModel.hpp"
//#include "TargetModelCreator.hpp"
#include "TrainDiscreteIIDModel.hpp"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {

  std::string TargetModel::str() const
  {
 {
      std::stringstream out;
      out << "model_name=\"TargetModel\""<< std::endl;
      out << ProbabilisticModel::alphabet()->str() << std::endl;
      return out.str();
    }

  }

    //! Calculates the sequence likelihood given this model
  double TargetModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
    if (end >= s.size())
      return -HUGE;
    if(begin < 0)
      return -HUGE;
    Sequence subseq(end - begin + 1);
    int k = 0;
    for(int i = begin ; i <= (int)end; i++){
      subseq[k] = s[i];
      k++;
    }
    SequenceEntryList samples;
    SequenceEntryPtr entry = SequenceEntryPtr(new SequenceEntry(alphabet()));
    entry->setSequence(subseq);
    samples.push_back(entry);
    TrainDiscreteIIDModelPtr trainingAlgorithm = TrainDiscreteIIDModelPtr(new TrainDiscreteIIDModel());
    ProbabilisticModelPtr m = trainingAlgorithm->train(samples, alphabet());
    return m->evaluate(s, begin, end);
  }

  void TargetModel::initialize (const ProbabilisticModelParameters & p )
  {
    ProbabilisticModelParameterValuePtr alphabet = p.getOptionalParameterValue("alphabet");
    if (alphabet != NULL)
      {
        AlphabetPtr alpha = AlphabetPtr(new Alphabet());
        alpha->initializeFromVector(alphabet->getStringVector());
        setAlphabet(alpha);
      }
  }
  ProbabilisticModelParameters TargetModel::parameters() const
  {
    ProbabilisticModelParameters p;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue("TargetModel")));
    p.add("alphabet", alphabet()->getParameterValue());
    return p;
  }

}
