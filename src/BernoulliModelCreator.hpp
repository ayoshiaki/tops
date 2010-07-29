#ifndef BERNOULLI_CREATOR_HPP
#define BERNOULLI_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the bernoulli distribution
  class BernoulliModelCreator : public ProbabilisticModelCreator 
  {
  public:
    virtual ~BernoulliModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tprobability = <a double>" << std::endl;
      out << "Example: " << std::endl;
      out << "\tcreate_model=\"FiniteDiscreteDistribution\"" << std::endl;
      out << "\toutput_model_file=\"./example.model\"" << std::endl;
      out << "\tprobability= 0.25" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr < BernoulliModelCreator> BernoulliModelCreatorPtr;
}

#endif
