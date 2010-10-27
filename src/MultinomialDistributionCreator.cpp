#include "MultinomialDistributionCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "Alphabet.hpp"
namespace tops {

  ProbabilisticModelPtr MultinomialDistributionCreator::create(ProbabilisticModelParameters & parameters) const {
    return createMultinomialDistribution(parameters);
  }

  MultinomialDistributionPtr MultinomialDistributionCreator::createMultinomialDistribution(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue("probabilities");
    if(probs == NULL) 
      {
	std::cerr << help();
      }
    MultinomialDistributionPtr model = MultinomialDistributionPtr(new MultinomialDistribution());
    model->initialize(parameters);
    return model;
  }
}
