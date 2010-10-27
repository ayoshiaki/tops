#include "TargetModel.hpp"
//#include "TargetModelCreator.hpp"
#include "TrainMultinomialDistribution.hpp"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {

  std::string TargetModel::str() const
  {
 {
      std::stringstream out;
      out << "model_name=\"TargetModel\""<< std::endl;
      out << ProbabilisticModel::alphabet()->str() << std::endl;
      return out.str();
    }

  }

    //! Calculates the sequence likelihood given this model 
  double TargetModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
    if (end >= s.size())
      return -HUGE;
    if(begin < 0)
      return -HUGE;
    Sequence subseq(end - begin + 1);
    int k = 0;
    for(int i = begin ; i <= (int)end; i++){
      subseq[k] = s[i];
      k++;
    }
    SequenceEntryList samples;
    SequenceEntryPtr entry = SequenceEntryPtr(new SequenceEntry(alphabet()));
    entry->setSequence(subseq);
    samples.push_back(entry);
    TrainMultinomialDistributionPtr trainingAlgorithm = TrainMultinomialDistributionPtr(new TrainMultinomialDistribution());
    ProbabilisticModelPtr m = trainingAlgorithm->train(samples, alphabet());
    return m->evaluate(s, begin, end);
  }

  void TargetModel::initialize (const ProbabilisticModelParameters & p ) 
  {
    ProbabilisticModelParameterValuePtr alphabet = p.getOptionalParameterValue("alphabet");
    if (alphabet != NULL)
      {
	AlphabetPtr alpha = AlphabetPtr(new Alphabet());
	alpha->initializeFromVector(alphabet->getStringVector());
	setAlphabet(alpha);
      } 
  }
  ProbabilisticModelParameters TargetModel::parameters() const
  {
    ProbabilisticModelParameters p;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue("TargetModel")));
    p.add("alphabet", alphabet()->getParameterValue());
    return p;
  }
  
}
