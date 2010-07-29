#ifndef REMOVE_SEQUENCE_FROM_MODEL_HPP
#define REMOVE_SEQUENCE_FROM_MODEL_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"

namespace tops
{
  class RemoveSequenceFromModel : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    RemoveSequenceFromModel (){}

    RemoveSequenceFromModel (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~RemoveSequenceFromModel () {};
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
  typedef boost::shared_ptr <RemoveSequenceFromModel> RemoveSequenceFromModelPtr ;
};



#endif
