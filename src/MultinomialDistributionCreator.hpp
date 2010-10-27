#ifndef FINITE_DISCRETE_DISTRIBUTION_CREATOR_HPP
#define FINITE_DISCRETE_DISTRIBUTION_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include "MultinomialDistribution.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the finite discrete distribution
  class MultinomialDistributionCreator : public ProbabilisticModelCreator 
  {
  public:
    MultinomialDistributionCreator() {}
    virtual ~MultinomialDistributionCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual MultinomialDistributionPtr createMultinomialDistribution(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tprobabilities = <a vector of doubles>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "\talphabet = <a vector of strings>" << std::endl;
      out << "Example: " << std::endl;
      out << "\tcreate_model=\"MultinomialDistribution\"" << std::endl;
      out << "\toutput_model_file=\"./example.model\"" << std::endl;
      out << "\talphabet= (\"A\", \"C\", \"G\", \"T\")" << std::endl;
      out << "\tprobabilities= (0.25, 0.25, 0.25)" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr < MultinomialDistributionCreator> MultinomialDistributionCreatorPtr;
}

#endif
