#include "ProbabilisticModel.hpp"
#include "SimilarityBasedSequenceWeighting.hpp"
#include "TrainSimilarityBasedSequenceWeighting.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "Symbol.hpp"
#include "util.hpp"
namespace tops {


  double calculate_normalizer (std::string q, double n, int max_length, std::map<std::string, double > & counter, AlphabetPtr alphabet, int skip_offset, int skip_length) 
  {
    if((int)q.size() >= max_length) 
      {
	double sum = 0;
	std::map<std::string, double>::const_iterator it;
	for(it = counter.begin(); it != counter.end();  it++)
	  {
	    std::string q2 = it->first;
	    int diff = 0;
	    if(q.size () != q2.size())
	      return -HUGE;
	    
	    for(int i = 0; i < (int)q2.size(); i++)
	      {
		if((i >= skip_offset) && (i <= skip_offset+skip_length)){
		  if(q[i] != q2[i])
		    diff+=2;
		} else 	if(q[i] != q2[i])
		  diff++;
	      }
	    if(diff == 1){
	      sum += 0.001 * it->second;
	    } else if (diff==0){
	      sum += it->second;
	    } 
	  }
	return sum;
      } 
    else 
      {
	double sum = 0;
	for(int i = 0; i < (int)alphabet->size(); i++)
	  {
	    std::stringstream x;
	    x << q << alphabet->getSymbol(i)->name();
	    n++;
	    sum += calculate_normalizer(x.str(), n, max_length,counter,alphabet, skip_offset, skip_length);
	  }
	return sum;
      }
  
  }




ProbabilisticModelPtr TrainSimilarityBasedSequenceWeighting::create(
		ProbabilisticModelParameters & parameters, double & loglikelihood,
		int & sample_size) const {
	ProbabilisticModelParameterValuePtr trainpar =
			parameters.getMandatoryParameterValue("training_set");
	ProbabilisticModelParameterValuePtr alphapar =
			parameters.getMandatoryParameterValue("alphabet");

	ProbabilisticModelParameterValuePtr offsetpar = parameters.getOptionalParameterValue("skip_offset");
	ProbabilisticModelParameterValuePtr skiplengthpar = parameters.getOptionalParameterValue("skip_length");

	if ((trainpar == NULL) || (alphapar == NULL) ) {
	  std::cerr << help() << std::endl;
	  exit(-1);
	}
	int skip_length = -1;
	int skip_offset  = -1;
	if((offsetpar != NULL) && (skiplengthpar != NULL))
	  {
	    skip_offset = offsetpar->getInt();
	    skip_length = skiplengthpar->getInt();

	  }

	AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
	alphabet ->initializeFromVector(alphapar->getStringVector());
	SequenceEntryList sample_set;
	readSequencesFromFile(sample_set, alphabet, trainpar->getString());
	std::map<std::string, double> counter;
	int min_length = 999999999;
	for(int i = 0; i < (int) sample_set.size(); i++) 
	  {
	    std::stringstream o;
	    for(int j = 0; j < (int)(sample_set[i]->getSequence()).size();j++)
	      {
		o << alphabet->getSymbol((sample_set[i]->getSequence())[j])->name();
	      }
	    if(counter.find(o.str()) == counter.end())
	      {
		counter[o.str()] = 1;
	      }
	    else 
	      {
		counter[o.str()] += 1;
	      }
	    if((int)o.str().size() < min_length)
	      min_length = o.str().size();
	  }
	std::string q;
	double normalize = calculate_normalizer(q, 0, min_length, counter, alphabet, skip_offset, skip_length);
	ProbabilisticModelParameters pars;
	pars.add("alphabet", alphapar);
	pars.add("counter", DoubleMapParameterValuePtr(new DoubleMapParameterValue(counter)));
	pars.add("normalizer", DoubleParameterValuePtr(new DoubleParameterValue(normalize)));
	if(skiplengthpar != NULL && offsetpar != NULL) {
	  pars.add("skip_length", skiplengthpar);
	  pars.add("skip_offset", offsetpar);
	}
	ProbabilisticModelPtr m = SimilarityBasedSequenceWeightingPtr(new SimilarityBasedSequenceWeighting());
	m->initialize(pars);
	loglikelihood = 0.0;
	sample_size = 0;
	for (int i = 0; i < (int) sample_set.size(); i++) {
	  loglikelihood
	    += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
	  sample_size += (sample_set[i]->getSequence()).size();
	}
	return m;
}
  ProbabilisticModelPtr TrainSimilarityBasedSequenceWeighting::create(
								      ProbabilisticModelParameters & parameters) const {
    double loglike;
    int samplesize;
    return create(parameters, loglike, samplesize);
    
  }
  
}
;
