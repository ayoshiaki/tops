#ifndef AKAIKE_INFORMATION_CRITERIA_HPP
#define AKAIKE_INFORMATION_CRITERIA_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops
{
  //! This class implements the Akaike Information Criteria. 
  class AkaikeInformationCriteria : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    AkaikeInformationCriteria (){}
    AkaikeInformationCriteria (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~AkaikeInformationCriteria () {};
    //! Creates a new model using the received parameters
    /*!
      \param parameters of the model's creator
      \return An instance of ProbabilisticModel 
    */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! returns a help message of this creator
    virtual std::string help() const;

    //! Set a creator 
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }
  };
  typedef boost::shared_ptr <AkaikeInformationCriteria> AkaikeInformationCriteriaPtr ;
};



#endif
