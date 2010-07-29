#ifndef REVERSE_COMPLEMENT_CREATOR_HPP
#define REVERSE_COMPLEMENT_CREATOR_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops {
  class ReverseComplementDNACreator :public ProbabilisticModelCreator { 
  public:
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const;
  };
  typedef boost::shared_ptr<ReverseComplementDNACreator> ReverseComplementDNACreatorPtr;
}

#endif
