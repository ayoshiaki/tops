/*
 *       RemoveSequenceFromModel.cpp
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

#include "RemoveSequenceFromModel.hpp"
#include "SequenceFactory.hpp"
#include <map>
#include <vector>

namespace tops
{
  ProbabilisticModelPtr RemoveSequenceFromModel::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr seqlistpar = parameters.getMandatoryParameterValue("sequence_list");
    ProbabilisticModelParameterValuePtr alphabetpar = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr phasepar = parameters.getOptionalParameterValue("phase");

    if((seqlistpar == NULL) || (alphabetpar == NULL))
      {
        std::cerr << help() << std::endl;
        exit(-1);
      }
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromVector(alphabetpar->getStringVector());

    std::vector<std::string> seqs = seqlistpar->getStringVector();

    int phase = 0;
    if(phasepar != NULL)
      phase = phasepar->getInt();


    ProbabilisticModelPtr result = _creator->create(parameters);
    for(int i = 0; i < (int)seqs.size() ; i ++ )
      {
        SequenceFactory factory(alphabet);
        boost::regex sep(" ");
        std::vector<std::string> seqstr;
        split_regex(seqs[i], seqstr,  sep);
        Sequence s = factory.createSequence(seqstr);
        result->removeSequenceFromModel(s, phase);
      }

    return result;
  }
}
