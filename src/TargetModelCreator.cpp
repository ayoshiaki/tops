#include "TargetModelCreator.hpp"
#include "TargetModel.hpp"
namespace tops {
  ProbabilisticModelPtr TargetModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr alphabet = parameters.getOptionalParameterValue("alphabet");
    TargetModelPtr model = TargetModelPtr(new TargetModel());
    if (alphabet != NULL)
      {
	AlphabetPtr alpha = AlphabetPtr(new Alphabet());
	alpha->initializeFromVector(alphabet->getStringVector());
	model->setAlphabet(alpha);
      } else 
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    return model;

  }
}
