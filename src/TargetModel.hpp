#ifndef TargetModel_H
#define TargetModel_H

#include "Sequence.hpp"
#include "Alphabet.hpp"

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "TargetModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  //! This  represent  probability distributions  over a finite set of symbols
  class TargetModel : public ProbabilisticModel
  {
  public:
    
    TargetModel()  {
    };
    
    virtual ~TargetModel() {}
    virtual void initialize (const ProbabilisticModelParameters & p ) ;
    virtual ProbabilisticModelParameters parameters() const;
    virtual std::string model_name() const {
      return "TargetModel";
    }

    //! Calculates the sequence likelihood given this model 
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;

    virtual ProbabilisticModelCreatorPtr getFactory () const{
          return TargetModelCreatorPtr(new TargetModelCreator());
    }

    virtual std::string str() const;


  };
  
  typedef boost::shared_ptr<TargetModel> TargetModelPtr;
}


#endif
