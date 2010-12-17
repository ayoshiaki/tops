#ifndef TRAIN_SBSW_ESTIMATION_HPP
#define TRAIN_SBSW_ESTIMATION_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a similarity based sequence weighting model
  class TrainSimilarityBasedSequenceWeighting : public ProbabilisticModelCreator {
  public:
    TrainSimilarityBasedSequenceWeighting () {}
    virtual ~TrainSimilarityBasedSequenceWeighting () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    virtual ProbabilisticModelPtr create(
    		ProbabilisticModelParameters & parameters, double & loglikelihood,
    		int & sample_size) const ;


  };
  typedef boost::shared_ptr <TrainSimilarityBasedSequenceWeighting> TrainSimilarityBasedSequenceWeightingPtr ;
};


#endif
