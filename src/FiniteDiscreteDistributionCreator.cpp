#include "FiniteDiscreteDistributionCreator.hpp"
#include "ConfigurationReader.hpp"
#include "FiniteDiscreteDistribution.hpp"
#include "Alphabet.hpp"
namespace tops {

  ProbabilisticModelPtr FiniteDiscreteDistributionCreator::create(ProbabilisticModelParameters & parameters) const {
    return createFiniteDiscreteDistribution(parameters);
  }

  FiniteDiscreteDistributionPtr FiniteDiscreteDistributionCreator::createFiniteDiscreteDistribution(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue("probabilities");
    if(probs == NULL) 
      {
	std::cerr << help();
      }
    FiniteDiscreteDistributionPtr model = FiniteDiscreteDistributionPtr(new FiniteDiscreteDistribution());
    model->initialize(parameters);
    return model;
  }
}
