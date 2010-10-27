#include "BernoulliModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "Alphabet.hpp"

namespace tops {
  ProbabilisticModelPtr BernoulliModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue("probability");
    if(probs == NULL)
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    DoubleVector distr;
    distr.push_back(probs->getDouble());
    distr.push_back(1.0 - probs->getDouble());
    ProbabilisticModelPtr model = MultinomialDistributionPtr(new MultinomialDistribution(distr));
    return model;
  }
}
