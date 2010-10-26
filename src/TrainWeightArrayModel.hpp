#ifndef TRAIN_WAM_HPP
#define TRAIN_WAM_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! A creator that trains a Weight Array Model
  class TrainWeightArrayModel : public ProbabilisticModelCreator {
  public:
    TrainWeightArrayModel () {}
    virtual ~TrainWeightArrayModel () {};
    //! Creates a probability model given the parameters
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Creates a probability model given the parameters and returns the log likelihood of the training set.
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const ;
    //! Provides a help message 
    virtual std::string help() const {
      std::string s;
      return s;
    }

  };
  typedef boost::shared_ptr <TrainWeightArrayModel> TrainWeightArrayModelPtr ;
};


#endif
