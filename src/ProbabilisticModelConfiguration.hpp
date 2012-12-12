#ifndef PROBABILISTIC_MODEL_CONFIGURATION_HPP
#define PROBABILISTIC_MODEL_CONFIGURATION_HPP

#include <string>
#include <map>

#include "ProbabilisticModelParameterValue2.hpp"

namespace tops {

  class ProbabilisticModelConfiguration: public ProbabilisticModelParameterValue2 {
  public:
    ProbabilisticModelConfiguration(ProbabilisticModelParameterMap parameters):_parameters(parameters) {}

    virtual std::string parameterType();
  private:
    ProbabilisticModelParameterMap _parameters;
  };

  typedef boost::shared_ptr <ProbabilisticModelConfiguration> ProbabilisticModelConfigurationPtr;

}

#endif
