#ifndef TRAIN_PHASED_MARKOV_CHAIN_CT_HPP
#define TRAIN_PHASED_MARKOV_CHAIN_CT_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a probabilistic model
  class TrainPhasedMarkovChainContextAlgorithm : public ProbabilisticModelCreator {
  public:
    TrainPhasedMarkovChainContextAlgorithm () {}
    virtual ~TrainPhasedMarkovChainContextAlgorithm () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const;


	virtual std::string help() const {
	  std::string s;

	  return s;
	}
  };
  typedef boost::shared_ptr <TrainPhasedMarkovChainContextAlgorithm> TrainPhasedMarkovChainContextAlgorithmPtr ;
};


#endif
