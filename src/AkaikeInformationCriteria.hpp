#ifndef AKAIKE_INFORMATION_CRITERIA_HPP
#define AKAIKE_INFORMATION_CRITERIA_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops
{
  //! This class contains the implementation of the Akaike information criteria. 
  class AkaikeInformationCriteria : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    AkaikeInformationCriteria (){}
    AkaikeInformationCriteria (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~AkaikeInformationCriteria () {};
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const {
      std::string s;
      return s;
    }
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }
  };
  typedef boost::shared_ptr <AkaikeInformationCriteria> AkaikeInformationCriteriaPtr ;
};



#endif
