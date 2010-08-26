#include "ProbabilisticModel.hpp"
#include "TrainFixedLengthMarkovChain.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "util.hpp"
namespace tops {

ProbabilisticModelPtr TrainFixedLengthMarkovChain::create(
		ProbabilisticModelParameters & parameters, double & loglikelihood,
		int & sample_size) const {
	ProbabilisticModelParameterValuePtr orderpar =
			parameters.getMandatoryParameterValue("order");
	ProbabilisticModelParameterValuePtr trainpar =
			parameters.getMandatoryParameterValue("training_set");
	ProbabilisticModelParameterValuePtr alphapar =
			parameters.getMandatoryParameterValue("alphabet");
	ProbabilisticModelParameterValuePtr pseudocountspar = parameters.getOptionalParameterValue("pseudo_counts");

	double pseudocounts = 0;
	
	if(pseudocountspar != NULL)
	  pseudocounts = pseudocountspar->getDouble();
	

	if ((trainpar == NULL) || (alphapar == NULL) || (orderpar == NULL)) {
	  std::cerr << help() << std::endl;
	  exit(-1);
	}
	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet ->initializeFromVector(alphapar->getStringVector());
	SequenceEntryList sample_set;
	readSequencesFromFile(sample_set, alphabet, trainpar->getString());
	ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet->size()));
	tree->initializeCounter(sample_set, orderpar->getInt(), pseudocounts);
	tree->normalize();

	VariableLengthMarkovChainPtr m = VariableLengthMarkovChainPtr(
								      new VariableLengthMarkovChain(tree));
	m->setAlphabet(alphabet);
	loglikelihood = 0.0;
	sample_size = 0;
	for (int i = 0; i < (int) sample_set.size(); i++) {
	  loglikelihood
	    += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
	  sample_size += (sample_set[i]->getSequence()).size();
	}

	return m;

}
ProbabilisticModelPtr TrainFixedLengthMarkovChain::create(
		ProbabilisticModelParameters & parameters) const {
	double loglike;
	int samplesize;
	return create(parameters, loglike, samplesize);

}

}
;
