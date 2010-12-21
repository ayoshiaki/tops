#ifndef MSM_CREATOR_HPP
#define MSM_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the Target Model
  class MultipleSequentialModelCreator : public ProbabilisticModelCreator 
  {
  public:
    MultipleSequentialModelCreator() {}
    virtual ~MultipleSequentialModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const 
    {
      std::stringstream out;
      return out.str();
    }

  };
  typedef boost::shared_ptr <MultipleSequentialModelCreator> MultipleSequentialModelCreatorPtr;
}

#endif
