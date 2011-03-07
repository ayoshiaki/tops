/*
 *       SmoothedHistogramStanke.cpp
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

#include "SmoothedHistogramStanke.hpp"
#include "MultinomialDistribution.hpp"
#include "util.hpp"
namespace tops {


  ProbabilisticModelPtr SmoothedHistogramStanke::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr training_set_parameter =
      parameters.getMandatoryParameterValue("training_set");
    ProbabilisticModelParameterValuePtr geompar =
      parameters.getOptionalParameterValue("geometric_tail");

    if(training_set_parameter == NULL) {
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
    DoubleVector prob;

    if(data.size() > 0)
      {


          long max = -1;

        for(int i = 0; i < (int)data.size(); i++){
          if(counter.find((long)data[i]) == counter.end())
            counter[(long)data[i]] = 1.0;
          else
            counter[(long)data[i]] += 1.0;
          if(max < (long) data[i])
            max = (long)data[i];
        }
        if(max > 15000)
            max = 15000;
        vector<double> pi;
        pi.resize(max);


        std::vector <double> sigmas;
        sigmas.resize(max);
        double a = 0.5;
        int m = 8;
        long r = 0;
        double count_left = 0;
        double count_right = 0;

        for(int pos = 0; pos < (int)sigmas.size(); pos+=1)
            {
                int bwd = (int) ((a / pow(n, 1.0/5.0) ) * (double)pos);
                if(bwd <= 0)
                    bwd = 1;
                for(int j = pos - bwd + 1; (j >= 0 && j < n) && (j <= pos + bwd -1)  ; j++)
                    {
                        int c = 1;
                        if(counter[j] > c)
                            c = counter[j];
                        if(j <= pos)
                            count_left += c;
                        if(j >= pos)
                            count_right += c;
                    }

                while (count_left < m && count_right < m && bwd < n)
                    {
                        bwd ++;
                        int c = 1;
                        if (counter[pos + bwd - 1] > c)
                            c = counter[pos + bwd - 1];
                        if(pos + bwd -1 < n)
                            count_left += c;
                        if(pos - bwd + 1 >= 0)
                            count_right += c;
                    }
                pi[pos] += epanechnikov((double)pos, (double)bwd) * counter[pos];
                bool negligible = false;
                int j=1;
                while (!negligible && (pos-j>=0 || pos+j<max)){
                    double  wj = epanechnikov(bwd, j) * counter[pos];
                    if (pos-j>=0 && pos-j<pi.size() ) {
                        pi[pos-j] += wj;
                    }
                    if (pos+j<pi.size() && pos+j>=0) {
                        pi[pos+j] += wj;
                    }
                    negligible = (wj < 1e-20);
                    j++;
                }
                total += pi[pos];
            }


        prob.resize(max+2);
        for (long k = 1; k <= max+1; k++){
            prob[k] =  pi[k]/(total) ;
        }
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
