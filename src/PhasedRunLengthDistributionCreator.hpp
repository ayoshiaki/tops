#ifndef PHASED_RUN_LENGTH_CREATOR_HPP
#define PHASED_RUN_LENGTH_CREATOR_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops {
  class PhasedRunLengthDistributionCreator :public ProbabilisticModelCreator { 
  public:
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const ;
  };
  typedef boost::shared_ptr<PhasedRunLengthDistributionCreator> PhasedRunLengthDistributionCreatorPtr;
}

#endif
