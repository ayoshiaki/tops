/*
 *       ReverseComplementDNACreator.cpp
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

#include "ReverseComplementDNACreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ReverseComplementDNA.hpp"

namespace tops {
  ProbabilisticModelPtr ReverseComplementDNACreator::create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
    ConfigurationReader reader;
    std::string modelstr = modelpar->getString();
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr m;
    std::map<std::string, ProbabilisticModelPtr>::const_iterator it = models.find(modelstr);
    bool frommodelname = false;
    if(it != models.end()) {
      m = it->second;
      frommodelname = true;
    }
    else{
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
    }
    ReverseComplementDNAPtr decorator = ReverseComplementDNAPtr(new ReverseComplementDNA(m));
    decorator->setAlphabet(m->alphabet());
    if(frommodelname)
      decorator->subModelName(modelstr);
    return decorator;
  }

  ProbabilisticModelPtr ReverseComplementDNACreator::create( ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
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
    ReverseComplementDNAPtr decorator = ReverseComplementDNAPtr(new ReverseComplementDNA(m));

    decorator->setAlphabet(m->alphabet());

    return decorator;
  }


}
