#ifndef TRAIN_PHASED_MARKOV_CHAIN_HPP
#define TRAIN_PHASED_MARKOV_CHAIN_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Train a Phased Markov Chain
  class TrainPhasedMarkovChain : public ProbabilisticModelCreator {
  public:
    TrainPhasedMarkovChain () {}
    virtual ~TrainPhasedMarkovChain () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const;


	virtual std::string help() const {
	  std::string s;

	  return s;
	}
  };
  typedef boost::shared_ptr <TrainPhasedMarkovChain> TrainPhasedMarkovChainPtr ;
};


#endif
