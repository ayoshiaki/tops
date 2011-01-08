/*
 *       SimilarityBasedSequenceWeighting.hpp
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

#ifndef SIM_BASED_SEQ_WEIGHTING
#define SIM_BASED_SEQ_WEIGHTING

#include "Sequence.hpp"
#include "Alphabet.hpp"

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "SimilarityBasedSequenceWeightingCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  //! This class is the implementation of Similiarity Based Sequence Weighting 
  class SimilarityBasedSequenceWeighting : public ProbabilisticModel
  {
  public:
    
    SimilarityBasedSequenceWeighting()  {
    };
    virtual double prefix_sum_array_compute(int begin, int end, int phase);

    virtual double prefix_sum_array_compute(int begin, int end) ;

    virtual bool initialize_prefix_sum_array(const Sequence & s);

    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    
    virtual ~SimilarityBasedSequenceWeighting() {}
    virtual void initialize (const ProbabilisticModelParameters & p ) ;
    virtual ProbabilisticModelParameters parameters() const;
    virtual std::string model_name() const {
      return "SimilarityBasedSequenceWeighting";
    }

    //! Calculates the sequence likelihood given this model 
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
  
    virtual std::string str() const;

  private: 
    DoubleVector _scores;
    std::map<std::string,double> _counter;
    int _skip_offset;
    int _skip_length;
    double _normalizer;
  };
  
  typedef boost::shared_ptr<SimilarityBasedSequenceWeighting> SimilarityBasedSequenceWeightingPtr;
}



#endif
