#ifndef FINITE_DISCRETE_DISTRIBUTION_H
#define FINITE_DISCRETE_DISTRIBUTION_H

#include "Sequence.hpp"
#include "Alphabet.hpp"

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  //! This  represent  probability distributions  over a finite set of symbols
  class FiniteDiscreteDistribution :  public FactorableModel
  {
  private:
    DoubleVector _log_probabilities;
    double _huge;
    int _size;
  public:
    
    FiniteDiscreteDistribution()  {
    };
    //! Constructor
    /*! \param probabilities is the probabilities value */
    FiniteDiscreteDistribution(const DoubleVector & probabilities) ;

    //! Choose 
    virtual double choose()const ;    


    //! Returns the log_probability_of the number s
    virtual double log_probability_of(int s) const;


    //! Set the probability value of the number s
    virtual double log_probability_of(int s, double new_value) ;

    virtual double evaluatePosition(const Sequence & s, int i) const ;
    
    virtual double choosePosition(const Sequence & s, int i )const ;

    virtual std::string model_name() const {
      return "MultinomialDistribution";
    }

    virtual ProbabilisticModelCreatorPtr getFactory () const;

    virtual int size() const;
    virtual std::string str() const;

    virtual void initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet) ;

    virtual void initialize(const ProbabilisticModelParameters & p) ;

    virtual ProbabilisticModelParameters parameters() const;
    
    void setProbabilities(const DoubleVector & probabilities) ;
  };
  
  typedef boost::shared_ptr<FiniteDiscreteDistribution> FiniteDiscreteDistributionPtr;
}


#endif
