#ifndef FIXED_SEQUENCE_AT_POSITION_HPP
#define FIXED_SEQUENCE_AT_POSITION_HPP

#include "ProbabilisticModelDecorator.hpp"
#include "FiniteDiscreteDistribution.hpp"
namespace tops { 
  //! A decorator that forces the emission of the same sequence at a fixed position of the sequence.
  class FixedSequenceAtPosition : public ProbabilisticModelDecorator {
  private:
    int _position;
    Sequence _sequence;
    FiniteDiscreteDistributionPtr _probabilities;
    std::vector<bool> _matchSeq;
    void initializeMatchedPositions(const Sequence & s) ;
    void addSequence(Sequence & h) const;
  public:
    FixedSequenceAtPosition(ProbabilisticModelPtr m) : ProbabilisticModelDecorator(m) {};
    virtual ~FixedSequenceAtPosition(){};
    virtual void initialize(int position,Sequence sequence, FiniteDiscreteDistributionPtr distr);
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual Sequence & choose(Sequence & h, int size) const ;
    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const;
    virtual double prefix_sum_array_compute(int begin , int end) ;
    virtual double prefix_sum_array_compute(int begin , int end, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s) ;
    virtual std::string str() const;
    std::string model_name () const;
  };
  typedef boost::shared_ptr<FixedSequenceAtPosition> FixedSequenceAtPositionPtr;
}

#endif
