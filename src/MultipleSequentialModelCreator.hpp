#ifndef MSM_CREATOR_HPP
#define MSM_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the Target Model
  class MultipleSequentialModelCreator : public ProbabilisticModelCreator 
  {
  public:
    MultipleSequentialModelCreator() {}
    virtual ~MultipleSequentialModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "model_name = \"MultipleSequentialModel\"" << std::endl;
      out << "\talphabet = <a vector of strings>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "Example: " << std::endl;
      out << "model_name = \"MultipleSequentialModel\"" << std::endl;
      out << "\talphabet= (\"A\", \"C\", \"G\", \"T\")" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr <MultipleSequentialModelCreator> MultipleSequentialModelCreatorPtr;
}

#endif
