#ifndef TRAIN_VLMC_WAM_HPP
#define TRAIN_VLMC_WAM_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a probabilistic model
  class TrainVariableLengthInhomogeneousMarkovChain : public ProbabilisticModelCreator {
  public:
    TrainVariableLengthInhomogeneousMarkovChain () {}
    virtual ~TrainVariableLengthInhomogeneousMarkovChain () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const ;
    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }

  };
  typedef boost::shared_ptr <TrainVariableLengthInhomogeneousMarkovChain> TrainVariableLengthInhomogeneousMarkovChainPtr ;
};


#endif
