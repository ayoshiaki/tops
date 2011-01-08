/*
 *       SmoothedHistogramKernelDensity.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *     
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *     
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *      
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#include "SmoothedHistogramKernelDensity.hpp"
#include "MultinomialDistribution.hpp"
#include "util.hpp"

namespace tops {



  ProbabilisticModelPtr SmoothedHistogramKernelDensity::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr training_set_parameter = 
      parameters.getMandatoryParameterValue("training_set");
    ProbabilisticModelParameterValuePtr geompar = 
      parameters.getOptionalParameterValue("geometric_tail");
    
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


    ProbabilisticModelParameters pars;
    pars.add("probabilities", ProbabilisticModelParameterValuePtr (new DoubleVectorParameterValue(prob)));
    pars.add("alphabet", alpha->getParameterValue());
    if(geompar != NULL) 
      pars.add("geometric_tail", geompar);
    MultinomialDistributionPtr result = 
      MultinomialDistributionPtr(new MultinomialDistribution());
    result->initialize(pars);
    
    return result;

  }

}
