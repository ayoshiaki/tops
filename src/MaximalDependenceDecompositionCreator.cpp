#include "MaximalDependenceDecompositionCreator.hpp"
#include "MaximalDependenceDecomposition.hpp"

namespace tops {
  ProbabilisticModelPtr MaximalDependenceDecompositionCreator::create(ProbabilisticModelParameters & parameters) const {
    MaximalDependenceDecompositionPtr model = MaximalDependenceDecompositionPtr(new MaximalDependenceDecomposition());
    ProbabilisticModelParameterValuePtr symbols = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr concensus = parameters.getMandatoryParameterValue("consensus");
    ProbabilisticModelParameterValuePtr concensus_model = parameters.getMandatoryParameterValue("consensus_model");
    ProbabilisticModelParameterValuePtr tree = parameters.getMandatoryParameterValue("tree");
    if((symbols == NULL) || (concensus == NULL) || (concensus_model == NULL) || (tree == NULL))
    {
      std::cerr << help() << std::endl;
    }
    model->initialize(parameters);
    return model;
  }
}