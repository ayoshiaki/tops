#ifndef SBSW_CREATOR_HPP
#define SBSW_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the Target Model
  class SimilarityBasedSequenceWeightingCreator : public ProbabilisticModelCreator 
  {
  public:
    SimilarityBasedSequenceWeightingCreator() {}
    virtual ~SimilarityBasedSequenceWeightingCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "model_name = \"SimilarityBasedSequenceWeighting\"" << std::endl;
      out << "\talphabet = <a vector of strings>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "Example: " << std::endl;
      out << "model_name = \"SimilarityBasedSequenceWeighting\"" << std::endl;
      out << "\talphabet= (\"A\", \"C\", \"G\", \"T\")" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr <SimilarityBasedSequenceWeightingCreator> SimilarityBasedSequenceWeightingCreatorPtr;
}

#endif
