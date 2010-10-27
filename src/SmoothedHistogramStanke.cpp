#include "SmoothedHistogramStanke.hpp"
#include "MultinomialDistribution.hpp"
#include "util.hpp"
namespace tops {


  ProbabilisticModelPtr SmoothedHistogramStanke::create( ProbabilisticModelParameters & parameters) const
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
    SequenceList sample_set;
    readSequencesFromFile(sample_set, alpha, training_set_parameter->getString());
    for(int i = 0; i < (int)sample_set.size();i++)
      for(int j = 0; j < (int) sample_set[i].size(); j++)
	data.push_back(sample_set[i][j]);

    std::map<long,double> sum;
    double total = 0.0;
    std::map<long,double> d;
    std::map<long,int> counter;
    double n = data.size();
    
    long max = -1;	
    if(data.size() > 0) 
      {
    

	for(int i = 0; i < (int)data.size(); i++){
	  if(counter.find((long)data[i]) == counter.end())
	    counter[(long)data[i]] = 1.0;
	  else
	    counter[(long)data[i]] += 1.0;
	  if(max < (long) data[i])
	    max = (long)data[i];
	}
    

	/* Smoothing the histogram using parzen window (kernel density estimation) */
	for (long pos = 1; pos <= max + 1; pos++) {
	  double h = (0.5 / pow(n, 1.0/5.0)) * (double)pos;
	  int c1 = 0;
	  int c2 = 0;
	  double r = 0.0;
	  for (r = 1;  r < (double)(h); r++){
	    c1 = 0; c2 = 0;
	    for(long x = pos ; x <= pos + (long)r - 1; x++) 
	      if(counter.find(x) != counter.end())
		c1 += counter[x];
	    for(long x = pos - (long)r + 1; x <= pos; x++) 
	      if(counter.find(x) != counter.end())
		c2 += counter[x];
	    if(c1 >= 8 || c2 >= 8){
	      break;
	    }
	  }
	  if(r > h) 
	    h = r;
	  double fx = kernel_density_estimation_gaussian(pos, h, data);
	  d[pos] = fx;
	  total += fx;
	}
	
      }	
    DoubleVector prob;
    prob.resize(max+2);
    for (long k = 1; k <= max+1; k++){
      prob[k] =  (d[k] + 1e-10)/(total + 1e-10*total) ;
    }


    MultinomialDistributionPtr result = 
      MultinomialDistributionPtr(new MultinomialDistribution(prob));
    
    return result;

  }

}
