#include "GeneralizedPairHiddenMarkovModelCreator.hpp"
#include "GeneralizedPairHiddenMarkovModel.hpp"
#include "ConfigurationReader.hpp"

namespace tops {
  ProbabilisticModelPtr GeneralizedPairHiddenMarkovModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr number_of_emissions = parameters.getMandatoryParameterValue("number_of_emissions");
    ProbabilisticModelParameterValuePtr initial_probabilities = parameters.getMandatoryParameterValue("initial_probabilities");   
    ProbabilisticModelParameterValuePtr end_probabilities = parameters.getMandatoryParameterValue("end_probabilities");   
    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");
    ProbabilisticModelParameterValuePtr durations = parameters.getOptionalParameterValue("duration_probabilities");

    if((state_names == NULL)||
       (observation_symbols == NULL)||
       (number_of_emissions == NULL) || 
       (initial_probabilities == NULL) || 
       (end_probabilities == NULL) || 
       (transitions == NULL) || 
       (emissions == NULL)) 
      {
	std::cerr << help() << std::endl;
      }
    ProbabilisticModelPtr model = GeneralizedPairHiddenMarkovModelPtr(new GeneralizedPairHiddenMarkovModel());
    model->initialize(parameters);
    return model;
  }
}
