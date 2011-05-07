#ifndef TRAIN_PHMM_BAUM_WELCH_HPP
#define TRAIN_PHMM_BAUM_WELCH_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a HMM using Baum-Welch
  class TrainPHMMBaumWelch : public ProbabilisticModelCreator {
  public:
    TrainPHMMBaumWelch () {}
    virtual ~TrainPHMMBaumWelch () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }


  };
  typedef boost::shared_ptr <TrainPHMMBaumWelch> TrainPHMMBaumWelchPtr ;
};


#endif
