#ifndef TRAIN_VARIABLE_LENGTH_MARKOV_CHAIN_HPP
#define TRAIN_VARIABLE_LENGTH_MARKOV_CHAIN_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! This class trains the Variable Length Markov Chain using the context algorithm.
  class TrainVariableLengthMarkovChain : public ProbabilisticModelCreator {
  public:
    TrainVariableLengthMarkovChain () {}
    virtual ~TrainVariableLengthMarkovChain () {};
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & training_set, double & loglikelihood, int & sample_size) const;
    virtual std::string help() const ;

  };
  typedef boost::shared_ptr <TrainVariableLengthMarkovChain> TrainVariableLengthMarkovChainPtr ;
};


#endif
