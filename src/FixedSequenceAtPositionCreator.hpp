#ifndef FIXED_SEQUENCE_AT_POSITION_CREATOR_HPP
#define FIXED_SEQUENCE_AT_POSITION_CREATOR_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops {
  class FixedSequenceAtPositionCreator :public ProbabilisticModelCreator { 
  public:
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

  };
  typedef boost::shared_ptr<FixedSequenceAtPositionCreator> FixedSequenceAtPositionCreatorPtr;
}

#endif
