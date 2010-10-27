#include "MultinomialDistribution.hpp"
#include "MultinomialDistributionCreator.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {
  
  MultinomialDistribution:: MultinomialDistribution(const DoubleVector & probabilities)
  {
    _size = probabilities.size() - 1;
    _log_probabilities.resize(_size+1);
    double  sum = 0;
    for(unsigned int i = 0; i < probabilities.size(); i++)
      sum += probabilities[i];
    for(int i = 0; (i <= _size); i++) 
      if(i >= (int)probabilities.size())
	_log_probabilities[i] = -HUGE;
      else if(close(probabilities[i], 0.0, 1e-10))
	_log_probabilities[i] = -HUGE;
      else
	_log_probabilities[i] = log(probabilities[i]/sum);
  }

  void MultinomialDistribution::setProbabilities(const DoubleVector & probabilities) {
    _size = probabilities.size() - 1;
    _log_probabilities.resize(_size+1);
    double  sum = 0;
    for(unsigned int i = 0; i < probabilities.size(); i++)
      sum += probabilities[i];
    for(int i = 0; (i <= _size); i++) 
      if(i >= (int)probabilities.size())
	_log_probabilities[i] = -HUGE;
      else if(close(probabilities[i], 0.0, 1e-10))
	_log_probabilities[i] = -HUGE;
      else
	_log_probabilities[i] = log(probabilities[i]/sum);
  }
 double MultinomialDistribution::choosePosition(const Sequence & s, int i ) const {
   return choose();
 }

  double MultinomialDistribution::evaluatePosition(const Sequence & s, int i) const  {
    return log_probability_of(s[i]);
  }

   double MultinomialDistribution::log_probability_of(int s) const  {
     if ((s>=0) && (s < (int)(_log_probabilities.size() )))
	return _log_probabilities[s];
      else {
	return -HUGE;
      }
    }

  double MultinomialDistribution::log_probability_of(int s, double new_value) {
    _log_probabilities[s] = new_value;
    return log_probability_of(s);
  }

  int MultinomialDistribution::size() const
  {
    return _size;
  }

  double MultinomialDistribution:: choose() const {
    double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
    int result;
    for(result = 0; result < (int)_log_probabilities.size(); result++)
      {
	random -= exp(_log_probabilities[result]);
	if(random <= 0)
	  break;
      }
    if(result >= (int)_log_probabilities.size())
      return (double)(result-1);
    return (double)result;
  }

  ProbabilisticModelCreatorPtr MultinomialDistribution::getFactory () const{
    return MultinomialDistributionCreatorPtr(new MultinomialDistributionCreator());
  }

  std::string MultinomialDistribution::str () const {
    std::stringstream out;
    AlphabetPtr alpha = alphabet();
    
    out << "model_name = \"MultinomialDistribution\"\n" ;
    out << "probabilities = (";
    if(_log_probabilities.size() > 0){	
      out << exp(_log_probabilities[0]);
      for(int k = 1; k < (int)_log_probabilities.size(); k++)
	{
	  out << "," << exp(_log_probabilities[k]);
	}
      out << ")\n";
    }
    if(alpha != NULL)
      {
	out << alpha->str();
      }
    return out.str();
  }
  
  
  void MultinomialDistribution::initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet) 
  {
    std::map<std::string,double>::const_iterator it;    
    DoubleVector probs;
    probs.resize(alphabet->size());
    for(int i = 0; i < (int)alphabet->size()  ; i++)
      probs[i] = 0.0;
    for(it = probabilities.begin(); it != probabilities.end(); it++)
      {

	int id = alphabet->getSymbol(it->first)->id();
	if(id < (int)probs.size())
	  probs[id] = it->second;
      }
    _log_probabilities.resize(probs.size());
    double  sum = 0;
    for(unsigned int i = 0; i < probs.size(); i++)
      {
	sum += probs[i] ;
	if(close(probs[i], 0.0, 1e-100))
	  _log_probabilities[i] = -HUGE;
	else
	  _log_probabilities[i] = log(probs[i]);
      }
    if(close(sum, 1.0, 1e-5))
      _log_probabilities.push_back(-HUGE);
    else {
      _log_probabilities[probs.size()-1] = log(1.0 - sum);
      std::cerr << "WARNING: MultinomialDistribution parameters must sum 1.0: sum is " << sum << std::endl;
    }
    setAlphabet(alphabet);
  }

  void MultinomialDistribution::initialize(const ProbabilisticModelParameters & p) {
    ProbabilisticModelParameterValuePtr probs = p.getMandatoryParameterValue("probabilities");
    ProbabilisticModelParameterValuePtr alphabet = p.getOptionalParameterValue("alphabet");
    DoubleVector distr = probs->getDoubleVector();
    setProbabilities(distr);
    if (alphabet != NULL)
      {
	AlphabetPtr alpha = AlphabetPtr(new Alphabet());
	alpha->initializeFromVector(alphabet->getStringVector());
	setAlphabet(alpha);
      }
  }

  ProbabilisticModelParameters MultinomialDistribution::parameters () const {
    AlphabetPtr alpha = alphabet();
    ProbabilisticModelParameters par;
    par.add("model_name", StringParameterValuePtr(new StringParameterValue("MultinomialDistribution")));
    DoubleVector probs;
    for(int k = 0; k < (int)_log_probabilities.size(); k++)
      {
	probs.push_back( exp(_log_probabilities[k]));
      }
    par.add("probabilities", DoubleVectorParameterValuePtr(new DoubleVectorParameterValue(probs)));
    if(alpha->size() > 0)
      par.add("alphabet", alpha->getParameterValue());
    return par;
  }
}


