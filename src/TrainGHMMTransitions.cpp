#include "TrainGHMMTransitions.hpp"
#include "FiniteDiscreteDistributionCreator.hpp"
#include "FiniteDiscreteDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "GeneralizedHiddenMarkovModel.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "ProbabilisticModelParameter.hpp"

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
    ProbabilisticModelPtr markovChain = creator.create(trainFixedMarkovChain);
    

    ProbabilisticModelParameters ghmmParameters = ghmm->parameters();
    ProbabilisticModelParameters markovChainParameters = markovChain->parameters();
    
    ProbabilisticModelParameterValuePtr probabilities_par = markovChainParameters.getOptionalParameterValue("probabilities");

    ghmmParameters.set("transitions", probabilities_par);
    
    ProbabilisticModelPtr m= creator.create(ghmmParameters);
    return m;
  }
}
