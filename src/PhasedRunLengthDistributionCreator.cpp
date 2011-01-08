/*
 *       PhasedRunLengthDistributionCreator.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#include "PhasedRunLengthDistributionCreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "PhasedRunLengthDistribution.hpp"

namespace tops {
  ProbabilisticModelPtr PhasedRunLengthDistributionCreator::create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
    ProbabilisticModelParameterValuePtr iphasepar = parameters.getMandatoryParameterValue("input_phase");
    ProbabilisticModelParameterValuePtr ophasepar = parameters.getMandatoryParameterValue("output_phase");
    ProbabilisticModelParameterValuePtr nphasepar = parameters.getMandatoryParameterValue("number_of_phases");
    ProbabilisticModelParameterValuePtr  deltapar = parameters.getMandatoryParameterValue("delta");
    ProbabilisticModelParameterValuePtr  modelpar = parameters.getMandatoryParameterValue("model");
    if(
       (nphasepar == NULL) ||
       (deltapar == NULL) ||
       (iphasepar == NULL) ||
       (ophasepar == NULL) ||
       (modelpar == NULL) ) {
      exit(-1);
    }
    int iphase = iphasepar -> getInt();
    int ophase = ophasepar -> getInt();
    int nphase = nphasepar -> getInt();
    int delta = deltapar -> getInt();
    std::string modelstr = modelpar->getString();

    
    ConfigurationReader reader;
    ProbabilisticModelPtr m;
    std::map<std::string, ProbabilisticModelPtr>::const_iterator it = models.find(modelstr);
    if(it != models.end())
      m = it->second;
    else{ 
      std::cerr << "Model " << modelstr << " not loaded ! " << std::endl;
      std::exit(-1);
    }
    
    PhasedRunLengthDistributionPtr decorator = PhasedRunLengthDistributionPtr(new PhasedRunLengthDistribution(m));
    AlphabetPtr alpha = m->alphabet();
    decorator->initialize(delta, iphase, ophase, nphase);
    decorator->setAlphabet(m->alphabet());
    decorator->subModelName(modelstr);
    return decorator;
  }

  ProbabilisticModelPtr PhasedRunLengthDistributionCreator::create( ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr iphasepar = parameters.getMandatoryParameterValue("input_phase");
    ProbabilisticModelParameterValuePtr ophasepar = parameters.getMandatoryParameterValue("output_phase");
    ProbabilisticModelParameterValuePtr nphasepar = parameters.getMandatoryParameterValue("number_of_phases");
    ProbabilisticModelParameterValuePtr  deltapar = parameters.getMandatoryParameterValue("delta");
    ProbabilisticModelParameterValuePtr  modelpar = parameters.getMandatoryParameterValue("model");
    if(
       (nphasepar == NULL) ||
       (deltapar == NULL) ||
       (iphasepar == NULL) ||
       (ophasepar == NULL) ||
       (modelpar == NULL) ) {
      exit(-1);
    }
    int iphase = iphasepar -> getInt();
    int ophase = ophasepar -> getInt();
    int nphase = nphasepar -> getInt();
    int delta = deltapar -> getInt();
    std::string modelstr = modelpar->getString();

    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    ProbabilisticModelPtr m ;
    if((modelstr.size() > 0) && (modelstr[0] == '[') ){
      modelstr = modelstr.substr(1, modelstr.size() -2 );
      reader.load(modelstr);
      ProbabilisticModelParametersPtr par = reader.parameters();
      m = creator.create(*par);
    } else  {
	m = creator.create(modelstr) ;
	if(m == NULL) {
	  std::cerr << "Can not load model file " << modelstr<< "!" << std::endl;
	  exit(-1);
	}
      }
    PhasedRunLengthDistributionPtr decorator = PhasedRunLengthDistributionPtr(new PhasedRunLengthDistribution(m));
    AlphabetPtr alpha = m->alphabet();
    decorator->initialize(delta, iphase, ophase, nphase);
    decorator->setAlphabet(m->alphabet());
    return decorator;

  }

}
