/*
 *       FixedSequenceAtPositionCreator.cpp
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

#include "FixedSequenceAtPositionCreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "FixedSequenceAtPosition.hpp"

namespace tops {
  ProbabilisticModelPtr FixedSequenceAtPositionCreator::create( ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr positionpar = parameters.getMandatoryParameterValue("position");
    ProbabilisticModelParameterValuePtr sequencepar = parameters.getMandatoryParameterValue("sequence");
    ProbabilisticModelParameterValuePtr probabilitypar = parameters.getMandatoryParameterValue("probability");
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
    boost::regex sep(" ");
    std::vector<std::string> seqstr;
    split_regex(sequencepar->getString(), seqstr,  sep);


    int position = positionpar->getInt();
    std::vector<double> probs;
    probs.push_back(probabilitypar->getDouble());
    probs.push_back(1.0 - probabilitypar->getDouble());
    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    std::string modelstr = modelpar->getString();

    ProbabilisticModelPtr m ;
    if((modelstr.size() > 0) && (modelstr[0] == '[') ){
      modelstr = modelstr.substr(1, modelstr.size() -2 );
      reader.load(modelstr);
      ProbabilisticModelParametersPtr par = reader.parameters();
      m = creator.create(*par);
    } else
      {
        m = creator.create(modelstr) ;
        if(m == NULL) {
          std::cerr << "Can not load model file " << modelstr<< "!" << std::endl;
          exit(-1);
        }
      }
    FixedSequenceAtPositionPtr decorator = FixedSequenceAtPositionPtr(new FixedSequenceAtPosition(m));

    DiscreteIIDModelPtr distr = DiscreteIIDModelPtr(new DiscreteIIDModel(probs));
    AlphabetPtr alpha = m->alphabet();
    SequenceFactory factory(alpha);
    Sequence sequence = factory.createSequence(seqstr);
    decorator->initialize(position, sequence, distr);
    decorator->setAlphabet(m->alphabet());
    decorator->subModelName(modelstr);
    return decorator;
  }

}
