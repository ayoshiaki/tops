#include "HiddenMarkovModelCreator.hpp"
#include "FiniteDiscreteDistributionCreator.hpp"
#include "FiniteDiscreteDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "HiddenMarkovModel.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
namespace tops {
  ProbabilisticModelPtr HiddenMarkovModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr initial_probabilities = parameters.getMandatoryParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");

    if((state_names == NULL)||
       (observation_symbols == NULL)||
       (initial_probabilities == NULL) || 
       (transitions == NULL) || 
       (emissions == NULL)) 
      {
	std::cerr << help() << std::endl;
      }
    ProbabilisticModelPtr model = HiddenMarkovModelPtr(new HiddenMarkovModel());
    model->initialize(parameters);
    return model;
  }
}
