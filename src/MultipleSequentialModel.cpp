#include "MultipleSequentialModel.hpp"
#include "TrainMultinomialDistribution.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {



  double MultipleSequentialModel::prefix_sum_array_compute(int begin, int end, int phase) {
    if(begin < 0)
      return -HUGE;
    double sum = -HUGE;
    int b = begin;
    int e = 0;
    for(int i = 0; i < (int)_sub_models.size(); i++)
      {
	e = b + _max_size[i] - 1;
	if (e >= _seqsize) 
	  e = _seqsize-1;

	if(i == 0)
	  sum = _sub_models[i]->prefix_sum_array_compute(b,e,phase);
	else
	  sum += _sub_models[i]->prefix_sum_array_compute(b,e,phase);
	phase = mod(phase + e - b + 1, 3);
	if( e >=  end) 
	  return sum;
	b = e + 1;
      }
    sum += _last_model->prefix_sum_array_compute(b,_seqsize,phase);
    return sum;
  }
  double MultipleSequentialModel::prefix_sum_array_compute(int begin, int end) 
  {
    return prefix_sum_array_compute(begin, end, 0);
    
  }
  
  bool MultipleSequentialModel::initialize_prefix_sum_array(const Sequence & s, int phase)
  {
    if(ProbabilisticModel::initialize_prefix_sum_array(s))
      return true;
    _last_model->initialize_prefix_sum_array(s);
    for(int i = 0; i < (int)_sub_models.size(); i++)
      {
	_sub_models[i]->initialize_prefix_sum_array(s);
      }
    _seqsize = s.size() -1;

    return true; 
  }

  bool MultipleSequentialModel::initialize_prefix_sum_array(const Sequence & s)
  {
    return initialize_prefix_sum_array(s, 0);
  }




  std::string MultipleSequentialModel::str() const
  {
    std::stringstream s;
    std::map <std::string, ProbabilisticModelParameterValuePtr> ::const_iterator it;
    std::map <std::string, ProbabilisticModelParameterValuePtr> p = _parameters.parameters();
    for(it = p.begin(); it != p.end(); it++)
      {
	s << it->first << " = " << (it->second)->str() << std::endl;
      }
    return s.str();
  }

  double MultipleSequentialModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end, int phase) const {
    if (end >= s.size())
      return -HUGE;
    if(begin < 0)
      return -HUGE;
    if(begin > end)
      return -HUGE;

    double sum = -HUGE;
    int b = begin;
    int e = 0;
    for(int i = 0; i < (int)_sub_models.size(); i++)
      {
	e = b + _max_size[i] - 1;
	if (e >= s.size()) 
	  e = s.size()-1;
	if(i == 0)
	  sum = _sub_models[i]->evaluate(s,b,e, phase);
	else
	  sum += _sub_models[i]->evaluate(s,b,e,phase);
	phase = mod(phase + e - b + 1, 3);
	if( e >=  (int)end) 
	  return sum;
	b = e + 1;
      }
    sum += _last_model->evaluate(s, b, end,phase);
    return sum;
  }
  void MultipleSequentialModel::initialize (const ProbabilisticModelParameters & p ) 
  {
    ProbabilisticModelParameterValuePtr modelspar = p.getMandatoryParameterValue("models");
    ProbabilisticModelParameterValuePtr maxsizepar = p.getMandatoryParameterValue("max_length");
    ProbabilisticModelParameterValuePtr lastmodelpar = p.getMandatoryParameterValue("last_model");
    ProbabilisticModelCreatorClient creator;
    StringVector modelnames = modelspar->getStringVector();
    DoubleVector maxlength = maxsizepar->getDoubleVector();
    std::string lastmodelname = lastmodelpar->getString();

    if(maxlength.size() != modelnames.size() )
      {
	std::cerr << "ERROR: number of models does not match the number of max_length";
	exit(-1);
      }
    _sub_models.resize(modelnames.size());
    _max_size.resize(modelnames.size());
    for(int i = 0; i < (int)modelnames.size();i++)
      {
	_sub_models[i] = creator.create(modelnames[i]);
	if(_sub_models[i] == NULL)
	  {
	    std::cerr << "ERROR: Cannot load model " << modelnames[i] << std::endl;
	    exit(-1);
	  }
	setAlphabet(_sub_models[i]->alphabet());
	_max_size[i] = maxlength[i];
      }
    _last_model = creator.create(lastmodelname);
    _parameters =  p;

    
  }
    //! Calculates the sequence likelihood given this model 
  double MultipleSequentialModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
    return evaluate(s, begin,end,0);
  }

  ProbabilisticModelParameters MultipleSequentialModel::parameters() const
  {
    return _parameters;
  }
}
