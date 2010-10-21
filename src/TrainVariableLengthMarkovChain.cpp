#include "util.hpp"

#include "ContextTree.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "TrainVariableLengthMarkovChain.hpp"

#include <iostream>
#include <fstream>
#include <set>

namespace tops {

ProbabilisticModelPtr TrainVariableLengthMarkovChain::create(
		ProbabilisticModelParameters & parameters) const {
	double loglike;
	int samplesize;
	return create(parameters, loglike, samplesize);
}

  ProbabilisticModelPtr TrainVariableLengthMarkovChain::create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & training_set, double & loglikelihood, int & sample_size) const {
	ProbabilisticModelParameterValuePtr alphabet_parameter =
			parameters.getMandatoryParameterValue("alphabet");
	ProbabilisticModelParameterValuePtr delta_parameter =
			parameters.getMandatoryParameterValue("cut");
	if ((alphabet_parameter == NULL)|| (delta_parameter == NULL)) {
		std::cerr << help() << std::endl;
		exit(-1);
	}
	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet->initializeFromVector(alphabet_parameter->getStringVector());
	SequenceFactory factory(alphabet);
	SequenceEntryList sample_set;
	for(int i = 0; i < (int)sample_set.size(); i++) {
	  Sequence s = factory.createSequence(training_set[i]);
	  SequenceEntryPtr e =  SequenceEntryPtr  (new SequenceEntry(alphabet));
	  e->setSequence(s);
	  sample_set.push_back(e);
	}

	ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
	tree->initializeContextTreeRissanen(sample_set);
	tree->pruneTree(delta_parameter->getDouble());
	tree->removeContextNotUsed();
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

ProbabilisticModelPtr TrainVariableLengthMarkovChain::create(
		ProbabilisticModelParameters & parameters, double & loglikelihood,
		int & sample_size) const {
	ProbabilisticModelParameterValuePtr training_set_parameter =
			parameters.getMandatoryParameterValue("training_set");
	ProbabilisticModelParameterValuePtr alphabet_parameter =
			parameters.getMandatoryParameterValue("alphabet");
	ProbabilisticModelParameterValuePtr delta_parameter =
			parameters.getMandatoryParameterValue("cut");

	if ((training_set_parameter == NULL) || (alphabet_parameter == NULL)
			|| (delta_parameter == NULL)) {
		std::cerr << help() << std::endl;
		exit(-1);
	}
	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet->initializeFromVector(alphabet_parameter->getStringVector());

	SequenceEntryList sample_set;
	readSequencesFromFile(sample_set, alphabet, training_set_parameter->getString());

	ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
	tree->initializeContextTreeRissanen(sample_set);
	tree->pruneTree(delta_parameter->getDouble());
	tree->removeContextNotUsed();
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

//! Provides a help
std::string TrainVariableLengthMarkovChain::help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\ntraining_set" << std::endl;
      out << "\talphabet" << std::endl;
      out << "\tcut" << std::endl;
      out << "Example: " << std::endl;
      out << "\ttraining_algorithm=\"ContextAlgorithm\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\ttraining_set= \"input.seq" << std::endl;
      out << "\tcut=1" << std::endl;
      return out.str();
}

}
;

