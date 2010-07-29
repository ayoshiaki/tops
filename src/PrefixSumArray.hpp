#ifndef PREFIX_SUM_ARRAY_HPP
#define PREFIX_SUM_ARRAY_HPP

#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"

namespace tops {

//! This class provides an interface for working with the prefix  sum arrays.
class PrefixSumArray 
{
public:
  virtual ~PrefixSumArray(){};
  
  //! Initialize the prefix sum array
  virtual void initialize(const Sequence & s) = 0;
  
  //! Initialize the prefix sum array
  virtual void initialize(const Sequence & s, int phase) {
    initialize(s);
  }

  //! compute the prefix sum array
  virtual double compute(int begin, int end ) const = 0;

  //! compute the prefix sum array
  virtual double compute (int begin, int end, int start_phase) const {
    return compute(begin, end);
  }
};

typedef boost::shared_ptr <PrefixSumArray> PrefixSumArrayPtr;
}

#endif
