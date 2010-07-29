#ifndef FACTORABLE_PREFIX_SUM_ARRAY_HPP
#define FACTORABLE_PREFIX_SUM_ARRAY_HPP

#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"
#include "PrefixSumArray.hpp"
#include "ProbabilisticModel.hpp"
#include "util.hpp"

namespace tops {

//! This class provides an interface for working with the prefix  sum arrays.
  class FactorableModelPrefixSumArray : public PrefixSumArray
  {
  private:
    ProbabilisticModel * _model;
    DoubleVector _alpha;
    DoubleVector _precision;
    Sequence _last;
  public:
    virtual ~FactorableModelPrefixSumArray(){};
    FactorableModelPrefixSumArray (ProbabilisticModel * model) : _model(model) {}
    //! Initialize the prefix sum array
    virtual void initialize(const Sequence & s) ;
    
    //! compute the prefix sum array
    virtual double compute(int begin, int end ) const ;
    

  };
  
  typedef boost::shared_ptr <FactorableModelPrefixSumArray> FactorableModelPrefixSumArrayPtr;
}

#endif
