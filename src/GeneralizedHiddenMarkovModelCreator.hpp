#ifndef GENERALIZED_HIDDEN_MARKOV_MODEL_CREATOR_HPP
#define GENERALIZED_HIDDEN_MARKOV_MODEL_CREATOR_HPP

#include <boost/shared_ptr.hpp>
#include "ProbabilisticModelCreator.hpp"
namespace tops {

  class ProbabilisticModelParameters;

  //! This class is  a factory for the finite discrete distribution
  class GeneralizedHiddenMarkovModelCreator : public ProbabilisticModelCreator 
  {
  public:
    GeneralizedHiddenMarkovModelCreator(){}
    virtual ~GeneralizedHiddenMarkovModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr < GeneralizedHiddenMarkovModelCreator> GeneralizedHiddenMarkovModelCreatorPtr;
}

#endif
