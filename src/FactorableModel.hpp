#ifndef FACTORABLE_MODEL_HPP
#define FACTORABLE_MODEL_HPP
#include "Sequence.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>
#include <iostream>

namespace tops {
  //! Abstract class defining models in which the likelihood of the sequence is factorable in the sense that they can be expressed as a product of terms evaluated at each position in a sequence.
  class FactorableModel : public ProbabilisticModel {
  private:
    double _dummy;
    DoubleVector _alpha;
    DoubleVector _precision;
  public:
     FactorableModel() {};
    virtual ~FactorableModel(){};
    
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const; 
    
    virtual Sequence & choose(Sequence & h, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i, int size) const;
    
    //! Evaluate the position i of the sequence s.
    virtual double evaluatePosition(const Sequence & s, int i) const = 0;
    
    //! Choose the position i of the sequence s given the subsequence before the position i.
    virtual double choosePosition(const Sequence & s, int i) const = 0;
    


    virtual  double prefix_sum_array_compute(int begin, int end) ;

    virtual bool initialize_prefix_sum_array(const Sequence & s) ;

    virtual FactorableModel* factorable() {
      return this;
    }
  };
  typedef boost::shared_ptr<FactorableModel> FactorableModelPtr;
}

#endif
