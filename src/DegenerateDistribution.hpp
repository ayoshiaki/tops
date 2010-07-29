#ifndef DEGENERATE_DISTRIBUTION_H
#define DEGENERATE_DISTRIBUTION_H 
#include "FiniteDiscreteDistribution.hpp"
namespace tops {
  //! A probabilistic model that emits a constant integer value.
  class DegenerateDistribution : public FiniteDiscreteDistribution
  {
    friend class boost::serialization::access;
    template <class Archive> 
    void serialize (Archive & ar, const unsigned int ) 
    {
      ar & boost::serialization::base_object<FiniteDiscreteDistribution> (*this);
      ar & _constant;
    }
  private:
    int _constant;
    double _huge;
    double _zero;
  public:
    ~DegenerateDistribution() {}
    DegenerateDistribution() {
      _huge = -HUGE;
      _zero = 0.0;
    }
    DegenerateDistribution(int c) : FiniteDiscreteDistribution("DegenerateDistribution")
    {
      _huge = -HUGE;
      _zero = 0.0;
      _constant=c;
    }

    //! Calculates the sequence likelihood given the model
    virtual double evaluate(const Sequence & sequence, unsigned int begin, unsigned int end) const 
    {
      for (int i = begin; (i < (int)sequence.size()) && (i < (int)end);  i++)
	if (sequence[i] != _constant)
	  return -HUGE;
      return 0.0;
    }
    
    //! Generates a sequence by the simulation of the model
    virtual double choose(Sequence & s, unsigned int size) const
    {
      s.resize(size);
      for (int i = 0; i < (int)size; i++)
	s[i] = _constant;
      return 0.0;
    }
    virtual std::string str() const 
    {
          std::stringstream out;
	  out << _constant << std::endl;
	  return out.str();
    }
    virtual double & log_probability_of(int c) 
    {
      if(c != _constant) 
	return _huge;
      return _zero;
    }

    virtual int choose () const
    {
      return _constant;
    }
  };
  typedef boost::shared_ptr <DegenerateDistribution> DegenerateDistributionPtr;
}

#endif
