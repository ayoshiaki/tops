#include "InhomogeneousMarkovChainCreator.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "Alphabet.hpp"
#include <boost/algorithm/string.hpp>
namespace tops {

  ProbabilisticModelPtr InhomogeneousMarkovChainCreator::create(ProbabilisticModelParameters & parameters) const {
    InhomogeneousMarkovChainPtr model = InhomogeneousMarkovChainPtr(new InhomogeneousMarkovChain());
    ProbabilisticModelParameterValuePtr symbols = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr position_specific_model_names = parameters.getMandatoryParameterValue("position_specific_distribution");
    ProbabilisticModelParameterValuePtr p = parameters.getMandatoryParameterValue("phased");
    if((symbols == NULL) || (position_specific_model_names == NULL) || (p == NULL))
      {
	std::cerr << help() << std::endl;
      }
    model->initialize(parameters);
    return model;
  }


}
