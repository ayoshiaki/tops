#include "SmoothedHistogramKernelDensity.hpp"
#include "FiniteDiscreteDistribution.hpp"
#include "util.hpp"

namespace tops {



  ProbabilisticModelPtr SmoothedHistogramKernelDensity::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr training_set_parameter = 
      parameters.getMandatoryParameterValue("training_set");
    
    if(training_set_parameter == NULL) {
      std::cerr << help () << std::endl;
      ProbabilisticModelPtr nullmodel;
      exit(-1);
      return nullmodel;
    }

    DoubleVector data;
    AlphabetPtr alpha = AlphabetPtr(new Alphabet());;
    SequenceEntryList sample_set;
    readSequencesFromFile(sample_set, alpha, training_set_parameter->getString());
    for(int i = 0; i < (int)sample_set.size();i++)
      for(int j = 0; j < (int) (sample_set[i]->getSequence()).size(); j++)
	data.push_back((sample_set[i]->getSequence())[j]);
    long max_value = -1;
    std::map<long,double> sum;
    double total = 0.0;
	
    if(data.size() > 0) 
      {
	double bandwidth = sj_bandwidth(data);

	for(int i = 0; i < (int)data.size(); i++)
	  if(max_value < (long) data[i])
	    max_value = (long)data[i];
	for (int pos = 0; pos <= max_value; pos++) {
	  sum[pos] = 0.0;
	  double integral = 0.0;
	  double min = kernel_density_estimation(pos-0.5, bandwidth, data);
	  double max = kernel_density_estimation(pos+0.5, bandwidth, data);
	  if(max < min) {
	    double aux = min;
	    min = max;
	    max = aux;
	  }
	  integral += min + (max - min)/2;
	  sum[pos] = integral;
	  total += integral;
	}
      }	
    DoubleVector prob;
    prob.resize(max_value+2);
    for (int k = 0; k <= max_value; k++){
      prob[k] =  sum[k]/total;
    }


    FiniteDiscreteDistributionPtr result = 
      FiniteDiscreteDistributionPtr(new FiniteDiscreteDistribution(prob));
    
    return result;

  }

}
