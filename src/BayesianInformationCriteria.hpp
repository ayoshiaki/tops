#ifndef BAYESIAN_INFORMATION_CRITERIA_HPP
#define BAYESIAN_INFORMATION_CRITERIA_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"

namespace tops
{
  //! Bayesian Information Criteria
  class BayesianInformationCriteria : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    BayesianInformationCriteria (){}
    BayesianInformationCriteria (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~BayesianInformationCriteria () {};
    //! Creates a new model using the received parameters
    /*! 
      \param parameters of the model's creators
      \return An instance of ProbabilisticModel
    */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Returns a help message of this creator
    virtual std::string help() const;

    //! Set a creator 
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }

  };
  typedef boost::shared_ptr <BayesianInformationCriteria> BayesianInformationCriteriaPtr ;
};



#endif
