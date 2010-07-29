#ifndef NULL_PREFIX_SUM_ARRAY_HPP
#define NULL_PREFIX_SUM_ARRAY_HPP

#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"
#include "ProbabilisticModel.hpp"
namespace tops {

  //! This class is a generic prefix sum array.
  class NullPrefixSumArray : public PrefixSumArray 
  {
  private:
    ProbabilisticModel *_model;
    Sequence _s;
  public:
    NullPrefixSumArray(ProbabilisticModel *model): _model(model){}
    virtual ~NullPrefixSumArray(){};
  
    //! Initialize the prefix sum array
    virtual void initialize(const Sequence & s) {
      initialize(s, 0);
    }
    
    //! Initialize the prefix sum array
    virtual void initialize(const Sequence & s, int phase) { 
      _s = s;
    }

    //! compute the prefix sum array
    virtual double compute(int begin, int end ) const {
      return  _model->evaluate(_s, begin,end);
    }

    //! compute the prefix sum array
    virtual double compute (int begin, int end, int start_phase) const {
      return compute(begin, end);
    }
  };

typedef boost::shared_ptr <NullPrefixSumArray> NullPrefixSumArrayPtr;
}

#endif
