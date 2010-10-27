#include "VariableLengthMarkovChain.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "Symbol.hpp"
#include "Alphabet.hpp"
#include <boost/algorithm/string.hpp>
namespace tops {

  ProbabilisticModelPtr VariableLengthMarkovChainCreator::create(ProbabilisticModelParameters & parameters) const {
    VariableLengthMarkovChainPtr mc = createVLMC(parameters);
    return mc;
  }


  VariableLengthMarkovChainPtr VariableLengthMarkovChainCreator::createVLMC(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue("probabilities");
    ProbabilisticModelParameterValuePtr symbols = parameters.getMandatoryParameterValue("alphabet");
    if((probs == NULL) || (symbols == NULL))
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    VariableLengthMarkovChainPtr model =
      VariableLengthMarkovChainPtr(new VariableLengthMarkovChain());
    model->initialize(parameters);
    return model;
  }


}
