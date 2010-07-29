#include "GeneralizedHiddenMarkovModelCreator.hpp"
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
  ProbabilisticModelPtr GeneralizedHiddenMarkovModelCreator::create(
								    ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr state_names_par =
      parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr initial_probabilities_par =
      parameters.getMandatoryParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr transitions_par =
      parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr observation_symbols_par =
      parameters.getMandatoryParameterValue("observation_symbols");
    
    GeneralizedHiddenMarkovModelPtr m = GeneralizedHiddenMarkovModelPtr(
									new GeneralizedHiddenMarkovModel());
    
    if ((state_names_par == NULL) || (initial_probabilities_par == NULL)
	|| (transitions_par == NULL) || (observation_symbols_par == NULL)) {
      std::cerr << help() << std::endl;
      return m;
    }
    
    m->initialize(parameters);
    return m;
  }
  
}
