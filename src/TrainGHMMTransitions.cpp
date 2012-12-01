/*
 *       TrainGHMMTransitions.cpp
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

#include "TrainGHMMTransitions.hpp"
#include "DiscreteIIDModelCreator.hpp"
#include "DiscreteIIDModel.hpp"
#include "ConfigurationReader.hpp"
#include "GeneralizedHiddenMarkovModel.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "TrainFixedLengthMarkovChain.hpp"
#include <boost/algorithm/string.hpp>

namespace tops {
  ProbabilisticModelPtr TrainGHMMTransitionsCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr ghmm_model_par = parameters.getMandatoryParameterValue("ghmm_model");
    GeneralizedHiddenMarkovModelPtr result = GeneralizedHiddenMarkovModelPtr(new GeneralizedHiddenMarkovModel());

    if(ghmm_model_par == NULL)
      {
        std::cerr << help() <<std::endl;
        return result;
      }

    ProbabilisticModelCreatorClient creator;
    std::string ghmm_file_name = ghmm_model_par->getString();
    ProbabilisticModelPtr ghmm = creator.create(ghmm_file_name);

    ProbabilisticModelParameters trainFixedMarkovChain;
    trainFixedMarkovChain.add("training_algorithm", StringParameterValuePtr(new StringParameterValue("FixedLengthMarkovChain")));
    trainFixedMarkovChain.add("order", IntParameterValuePtr(new IntParameterValue(1)));
    trainFixedMarkovChain.add("training_set", parameters.getMandatoryParameterValue("training_set"));
    trainFixedMarkovChain.add("pseudo_counts", IntParameterValuePtr(new IntParameterValue(0)));


    ProbabilisticModelParameters ghmmParameters = ghmm->parameters();

    trainFixedMarkovChain.add("alphabet", ghmmParameters.getOptionalParameterValue("state_names"));
    TrainFixedLengthMarkovChainPtr markovChainTraining = TrainFixedLengthMarkovChainPtr(new TrainFixedLengthMarkovChain());
    ProbabilisticModelPtr markovChain  = markovChainTraining->create(trainFixedMarkovChain);
    ProbabilisticModelParameters markovChainParameters = markovChain->parameters();
    AlphabetPtr states = AlphabetPtr(new Alphabet());
    states->initializeFromVector( (ghmmParameters.getOptionalParameterValue("state_names"))->getStringVector() );
    std::map<std::string,double> probs;
    probs = (markovChainParameters.getOptionalParameterValue("probabilities"))->getDoubleMap();
    std::map<std::string,double> trans;


    for(int from = 0; from < states->size(); from++)
      for(int to = 0; to < states->size(); to++){
	stringstream aux;
	aux << states->getSymbol(to)->name() << "|" << states->getSymbol(from)->name();
	if(!close(0, probs[aux.str()], 1e-10)) 
	  trans[aux.str()] = probs[aux.str()];
      }

	
    ProbabilisticModelParameterValuePtr probabilities_par = ProbabilisticModelParameterValuePtr(new DoubleMapParameterValue(trans)); 

    ghmmParameters.set("transitions", probabilities_par);

    ProbabilisticModelPtr m= creator.create(ghmmParameters);
    return m;
  }
}
