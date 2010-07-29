#ifndef TRAIN_BAUM_WELCH_HPP
#define TRAIN_BAUM_WELCH_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a probabilistic model
  class TrainHMMBaumWelch : public ProbabilisticModelCreator {
  public:
    TrainHMMBaumWelch () {}
    virtual ~TrainHMMBaumWelch () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }


  };
  typedef boost::shared_ptr <TrainHMMBaumWelch> TrainHMMBaumWelchPtr ;
};


#endif
