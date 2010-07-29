#include "ProbabilisticModel.hpp"
#include "TrainFiniteDiscreteDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "util.hpp"
namespace tops {

  ProbabilisticModelPtr TrainFiniteDiscreteDistribution::create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & sample_set, double & loglikelihood, int & sample_size) const {

	ProbabilisticModelParameterValuePtr alphapar =
			parameters.getMandatoryParameterValue("alphabet");

	if (alphapar == NULL) {
	  std::cerr << help() << std::endl;
	  exit(-1);
	}
	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet ->initializeFromVector(alphapar->getStringVector());
	SequenceEntryList samples;
	SequenceFactory factory(alphabet);
	for(int i = 0; i < (int)sample_set.size();i++){
	  SequenceEntryPtr  inseq = SequenceEntryPtr(new SequenceEntry(alphabet));
	  inseq->setSequence(factory.createSequenceRemovedSpaces(sample_set[i]));
	  samples.push_back(inseq);
	}

	ProbabilisticModelPtr m = train (samples, alphabet);
	loglikelihood = 0.0;
	sample_size = 0;
	for (int i = 0; i < (int) samples.size(); i++) {
	  loglikelihood
	    += m->evaluate((samples[i]->getSequence()), 0, (samples[i]->getSequence()).size() - 1);
	  sample_size += (samples[i]->getSequence()).size();
	}
	return m;
  }

ProbabilisticModelPtr TrainFiniteDiscreteDistribution::train(const SequenceEntryList & sample_set, AlphabetPtr alphabet) const{
	ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet->size()));
	tree->initializeCounter(sample_set, 0);
	tree->normalize();
	FiniteDiscreteDistributionPtr m = tree->getRoot()->getDistribution();
	m->setAlphabet(alphabet);
	return m;
  }

  ProbabilisticModelPtr TrainFiniteDiscreteDistribution::create(
		ProbabilisticModelParameters & parameters, double & loglikelihood,
		int & sample_size) const {
	ProbabilisticModelParameterValuePtr trainpar =
			parameters.getMandatoryParameterValue("training_set");
	ProbabilisticModelParameterValuePtr alphapar =
			parameters.getMandatoryParameterValue("alphabet");

	if ((trainpar == NULL) || (alphapar == NULL) ) {
	  std::cerr << help() << std::endl;
	  exit(-1);
	}
	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet ->initializeFromVector(alphapar->getStringVector());
	SequenceEntryList sample_set;
	readSequencesFromFile(sample_set, alphabet, trainpar->getString());
	ProbabilisticModelPtr m =  train(sample_set, alphabet);
	loglikelihood = 0.0;
	sample_size = 0;
	for (int i = 0; i < (int) sample_set.size(); i++) {
	  loglikelihood
	    += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
	  sample_size += (sample_set[i]->getSequence()).size();
	}
	return m;

}
  ProbabilisticModelPtr TrainFiniteDiscreteDistribution::create(
								ProbabilisticModelParameters & parameters) const {
    double loglike;
    int samplesize;
    return create(parameters, loglike, samplesize);
    
  }

   std::string TrainFiniteDiscreteDistribution::help() const   {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\ttraining_set" << std::endl;
      out << "\talphabet" << std::endl;
      out << "Example: " << std::endl;
      out << "\ttraining_algorithm=\"FiniteDiscreteDistribution\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\ttraining_set= \"input.seq" << std::endl;
      return out.str();
  }
}

