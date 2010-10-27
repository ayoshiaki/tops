#include "SmoothedHistogramBurge.hpp"
#include "MultinomialDistribution.hpp"
#include "util.hpp"
namespace tops {


  ProbabilisticModelPtr SmoothedHistogramBurge::create( ProbabilisticModelParameters & parameters) const
  {


    ProbabilisticModelParameterValuePtr training_set_parameter = 
      parameters.getMandatoryParameterValue("training_set");
    ProbabilisticModelParameterValuePtr cpar = 
      parameters.getMandatoryParameterValue("C");
    
    if((training_set_parameter == NULL)||(cpar == NULL)) {
      std::cerr << help () << std::endl;
      ProbabilisticModelPtr nullmodel;
      exit(-1);
      return nullmodel;
    }
    double C = cpar->getDouble();
    DoubleVector data;
    AlphabetPtr alpha = AlphabetPtr(new Alphabet());;
    SequenceList sample_set;
    readSequencesFromFile(sample_set, alpha, training_set_parameter->getString());

    for(int i = 0; i < (int)sample_set.size();i++)
      for(int j = 0; j < (int) sample_set[i].size(); j++)
	data.push_back(sample_set[i][j]);

    typedef std::map<long,double> Lengths;
    int N = data.size();
    Lengths counter;
    Lengths sum;
    Lengths::const_iterator iter;
    long max = -1;
    for(int i = 0; i < N; i++){
      iter = counter.find((long)data[i]);
      if(max < data[i])
	max = (long)data[i];
      if(iter == counter.end()) 
	counter[(long)data[i]] = 1.0;
      else
	counter[(long)data[i]] += 1.0;
    }
    double total = 0.0;
    for (int k = 1; k <= max; k++){
      int start = k - 10;
      int end = k + 10;
      if(start < 0)  start = 0;
      sum[k] = 0.0;
      for(int x = start; x < end; x++){
	iter = counter.find((long)x);
	if(iter != counter.end() && iter->second > 0.0){
	  double nx = iter->second;
	  double mean = x+1.0;
	  double sd = sqrt(2*((double)(x+1.0))*C/nx);
	  double px2 = 0.5*(1 + erf((((double)k+1.5) - mean))/ (sd*sqrt(2.0)));
	  double px1 = 0.5*(1 + erf((((double)k+0.5) - mean))/ (sd*sqrt(2.0)));
	  assert(nx > 0.0);
	  assert(mean > 0.0);
	  assert(sd > 0.0);
	  sum[k] += nx*(px2 - px1);
	}
      }
      sum[k] = sum[k]/N;
      total = total+ sum[k];
    }
    double epsilon = 1e-5;
    total = total/(1 - max*epsilon);
    
    DoubleVector prob;
    prob.resize(max+1);
    for (int k = 1; k <= max; k++){
      prob[k] = epsilon + sum[k]/total;
    }
    
    ProbabilisticModelParameters pars;
    pars.add("probabilities", ProbabilisticModelParameterValuePtr (new DoubleVectorParameterValue(prob)));
    pars.add("alphabet", alpha->getParameterValue());
    MultinomialDistributionPtr result = 
      MultinomialDistributionPtr(new MultinomialDistribution());
    result->initialize(pars);
    return result;
  }

}
