#include "SimilarityBasedSequenceWeighting.hpp"
//#include "SimilarityBasedSequenceWeightingCreator.hpp"
#include "TrainMultinomialDistribution.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {



  double SimilarityBasedSequenceWeighting::prefix_sum_array_compute(int begin, int end, int phase) {
    if ((begin < (int) _scores.size()) && (begin >= 0)){
      return _scores[begin];
    }
    return -HUGE;
  }
  double SimilarityBasedSequenceWeighting::prefix_sum_array_compute(int begin, int end) 
  {
    return prefix_sum_array_compute(begin, end, 0);
  }
    
  bool SimilarityBasedSequenceWeighting::initialize_prefix_sum_array(const Sequence & s, int phase)
  {
    if(ProbabilisticModel::initialize_prefix_sum_array(s))
      return true;
    _scores.resize(s.size());
    for (int i = 0; i < (int) s.size(); i++)  {
      _scores[i] = evaluate(s, i, s.size() - 1);
    }
    return true; 
  }

  bool SimilarityBasedSequenceWeighting::initialize_prefix_sum_array(const Sequence & s)
  {
    return initialize_prefix_sum_array(s, 0);
  }




  std::string SimilarityBasedSequenceWeighting::str() const
  {
    std::stringstream out;

    DoubleMapParameterValuePtr p = DoubleMapParameterValuePtr(new DoubleMapParameterValue(_counter)) ;    
    DoubleParameterValuePtr x = DoubleParameterValuePtr(new DoubleParameterValue(_normalizer));
    IntParameterValuePtr so = IntParameterValuePtr(new IntParameterValue(_skip_offset));
    IntParameterValuePtr sl = IntParameterValuePtr(new IntParameterValue(_skip_length));
    out << "model_name=\"SimilarityBasedSequenceWeighting\""<< std::endl;
    out << ProbabilisticModel::alphabet()->str() << std::endl;
    out << "counter = " <<  p->str() << std::endl;
    out << "normalizer = " << x->str() << std::endl;
    if(_skip_offset >= 0)
      {
	out << "skip_offset = " <<  so->str() << std::endl;
	out << "skip_length = " << sl->str() << std::endl;
      }
      
    return out.str();
  }

    //! Calculates the sequence likelihood given this model 
  double SimilarityBasedSequenceWeighting::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
    if (end >= s.size())
      return -HUGE;
    if(begin < 0)
      return -HUGE;
    int length =     (_counter.begin()->first).size();
    std::stringstream qstream;
    for(int i = begin ; (i <= (int)end) && (i < (int) (begin + length)) ; i++){
      qstream << alphabet()->getSymbol(s[i])->name();
    }
    std::string q = qstream.str();
    std::map<std::string, double>::const_iterator it;
    double sum = 0.0;
    for(it = _counter.begin(); it != _counter.end();  it++)
      {
	std::string q2 = it->first;
	int diff = 0;
	if(q.size () != q2.size())
	  return -HUGE;
	for(int i = 0; i < (int)q2.size(); i++)
	  {
	    if((i >= _skip_offset) && (i <= _skip_offset+_skip_length)){
	      if(q[i] != q2[i])
		diff+=2;
	    }else if(q[i] != q2[i])
	      diff++;
	  }
	if(diff == 1){
	  sum += 0.001 * it->second;
	} else if (diff==0){
	  sum += it->second;
	} 
      }
    if(close(sum, 0.0, 1e-5))
      return -HUGE;
    return log(sum/_normalizer);
  }
  void SimilarityBasedSequenceWeighting::initialize (const ProbabilisticModelParameters & p ) 
  {
    ProbabilisticModelParameterValuePtr alphabetpar = p.getOptionalParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr counterpar = p.getOptionalParameterValue("counter");
    ProbabilisticModelParameterValuePtr lengthpar = p.getOptionalParameterValue("length");
    ProbabilisticModelParameterValuePtr normalizerp = p.getOptionalParameterValue("normalizer");
 
    ProbabilisticModelParameterValuePtr offsetpar = p.getOptionalParameterValue("skip_offset");
    ProbabilisticModelParameterValuePtr skiplengthpar = p.getOptionalParameterValue("skip_length");

    if (alphabetpar != NULL)
      {
	AlphabetPtr alpha = AlphabetPtr(new Alphabet());
	alpha->initializeFromVector(alphabetpar->getStringVector());
	setAlphabet(alpha);
      } 
    _counter = counterpar->getDoubleMap();
    _normalizer  = normalizerp->getDouble();
    if((skiplengthpar == NULL) || (offsetpar == NULL) ) 
      {
	_skip_offset = -1;
	_skip_length = -1;
      }
    else 
      {
	_skip_offset = offsetpar->getInt();
	_skip_length = skiplengthpar->getInt();
      }
  }

  ProbabilisticModelParameters SimilarityBasedSequenceWeighting::parameters() const
  {
    ProbabilisticModelParameters p;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue("SimilarityBasedSequenceWeighting")));
    p.add("alphabet", alphabet()->getParameterValue());
    p.add("counter", DoubleMapParameterValuePtr(new DoubleMapParameterValue(_counter)));
    p.add("normalizer", DoubleParameterValuePtr(new DoubleParameterValue(_normalizer)));
    if(_skip_offset >= 0){
      p.add("skip_length", IntParameterValuePtr(new IntParameterValue(_skip_length)));
      p.add("skip_offset", IntParameterValuePtr(new IntParameterValue(_skip_offset)));
    }
    return p;
  }
}
