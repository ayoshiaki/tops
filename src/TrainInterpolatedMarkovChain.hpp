#ifndef TRAIN_INTERPOLATED_MC_ESTIMATION_HPP
#define TRAIN_INTERPOLATED_MC_ESTIMATION_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a fixed length markov chain
  class TrainInterpolatedMarkovChain : public ProbabilisticModelCreator {
  public:
    TrainInterpolatedMarkovChain () {}
    virtual ~TrainInterpolatedMarkovChain () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    virtual ProbabilisticModelPtr create(
    		ProbabilisticModelParameters & parameters, double & loglikelihood,
    		int & sample_size) const ;


  };
  typedef boost::shared_ptr <TrainInterpolatedMarkovChain> TrainInterpolatedMarkovChainPtr ;
};


#endif
