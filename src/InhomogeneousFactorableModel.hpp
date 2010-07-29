#ifndef IH_FACTORABLE_MODEL_HPP
#define IH_FACTORABLE_MODEL_HPP

#include "ProbabilisticModel.hpp"

namespace tops {
  //! Interface defining inhomogeneous models in which the  likelihood of the sequence is factorable in the sense that they can be expressed as a product of terms evaluated at each position in a sequence 
  class InhomogeneousFactorableModel : public ProbabilisticModel {
  private:
    ProbabilisticModel * _model;
    std::vector<DoubleVector> _alpha;
    std::vector<DoubleVector> _precision;
    DoubleVector _scores;
    int _phase;
  public:
     InhomogeneousFactorableModel() {};
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const; 
    
    virtual Sequence & choose(Sequence & h, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const;
    virtual ~ InhomogeneousFactorableModel(){};
    //! Evaluate the position i of the sequence s.
    /*! 
      \param s is the sequence
      \param i is the position of the sequence
      \param t is the considered time 
    */
    virtual double evaluatePosition(const Sequence & s, int i, int t) const = 0;
    //! Choose the position i of the sequence s given the subsequence before the position i.
    /*! 
      \param s is the sequence
      \param i is the position of the sequence
      \param t is the considered time 
    */
    virtual int choosePosition(const Sequence & s, int i, int t) const = 0;

    //! Returns the maximum value of t.
    virtual int maximumTimeValue() const = 0;

    //! Phased 
    virtual bool isPhased() const = 0;

    virtual  double prefix_sum_array_compute(int begin, int end) ;
    virtual  double prefix_sum_array_compute(int begin, int end, int phase) ;
    
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s){
      return initialize_prefix_sum_array(s, 0);
    }

    virtual InhomogeneousFactorableModel * inhomogeneous() {
      return this;
    }

  };
}

#endif
