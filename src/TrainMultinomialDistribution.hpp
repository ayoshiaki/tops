#ifndef TRAIN_FDD_HPP
#define TRAIN_FDD_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a Multinomial Distribution
  class TrainMultinomialDistribution : public ProbabilisticModelCreator {
  public:
    TrainMultinomialDistribution () {}
    virtual ~TrainMultinomialDistribution () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    virtual ProbabilisticModelPtr create(
    		ProbabilisticModelParameters & parameters, double & loglikelihood,
    		int & sample_size) const ;

    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & sample_set, double & loglikelihood, int & sample_size) const ;

    virtual ProbabilisticModelPtr train(const SequenceEntryList & sample_set, AlphabetPtr alphabet) const;
    virtual std::string help() const ;

  };
  typedef boost::shared_ptr <TrainMultinomialDistribution> TrainMultinomialDistributionPtr ;
};


#endif
