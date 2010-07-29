#ifndef TRAIN_WAM_HPP
#define TRAIN_WAM_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a probabilistic model
  class TrainWeightArrayModel : public ProbabilisticModelCreator {
  public:
    TrainWeightArrayModel () {}
    virtual ~TrainWeightArrayModel () {};
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
  typedef boost::shared_ptr <TrainWeightArrayModel> TrainWeightArrayModelPtr ;
};


#endif
