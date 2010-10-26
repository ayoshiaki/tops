#ifndef TRAIN_GHMM_TRANSITIONS_HPP
#define TRAIN_GHMM_TRANSITIONS_HPP

#include <boost/shared_ptr.hpp>
#include "ProbabilisticModelCreator.hpp"
namespace tops {

  class ProbabilisticModelParameters;

  //! This class is  a factory for the Multinomial Distribution
  class TrainGHMMTransitionsCreator : public ProbabilisticModelCreator 
  {
  public:
    TrainGHMMTransitionsCreator(){}
    virtual ~TrainGHMMTransitionsCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr < TrainGHMMTransitionsCreator> TrainGHMMTransitionsCreatorPtr;
}

#endif
