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
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const {
      std::string s;
      return s;

    }
    virtual std::string factory_name() const {
      return "BayesianInformationCriteria";
    }
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }

  };
  typedef boost::shared_ptr <BayesianInformationCriteria> BayesianInformationCriteriaPtr ;
};



#endif
