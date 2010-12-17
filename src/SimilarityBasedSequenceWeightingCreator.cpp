#include "SimilarityBasedSequenceWeightingCreator.hpp"
#include "SimilarityBasedSequenceWeighting.hpp"
namespace tops {
  ProbabilisticModelPtr SimilarityBasedSequenceWeightingCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr alphabet = parameters.getOptionalParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr counter = parameters.getOptionalParameterValue("counter");
    ProbabilisticModelParameterValuePtr normalizer = parameters.getOptionalParameterValue("normalizer");


    SimilarityBasedSequenceWeightingPtr model = SimilarityBasedSequenceWeightingPtr(new SimilarityBasedSequenceWeighting());

    if(( alphabet == NULL ) ||
       (counter == NULL) ||
       (normalizer == NULL) )
      {
	return model;
      }

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
    model->initialize(parameters);
    return model;
  }
}
