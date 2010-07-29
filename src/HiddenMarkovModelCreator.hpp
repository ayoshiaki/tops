#ifndef HIDDEN_MARKOV_MODEL_CREATOR_HPP
#define HIDDEN_MARKOV_MODEL_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the finite discrete distribution
  class HiddenMarkovModelCreator : public ProbabilisticModelCreator 
  {
  public:
    HiddenMarkovModelCreator() {}
    virtual ~HiddenMarkovModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tstate_names" << std::endl;
      out << "\tobservation_symbols" << std::endl;
      out << "\tinitial_probabilities" << std::endl;
      out << "\ttransitions" << std::endl;
      out << "\temission_probabilities" << std::endl;
      out << "Example: " << std::endl;
      out << "\tmodel_name = \"HiddenMarkovModel\"" << std::endl;
      out << "\tstate_names = (\"F\",\"L\")" << std::endl;
      out << "\tobservation_symbols = (\"1\",\"2\",\"3\",\"4\",\"5\",\"6\")" << std::endl;
      out << "\ttransitions = (\"F\" | \"F\": 0.7;" << std::endl;
      out << "\t \"L\" | \"F\": 0.3;" << std::endl;
      out << "\t \"F\" | \"L\": 0.3;" << std::endl;
      out << "\t \"L\" | \"L\": 0.7)" << std::endl;
      out << "\temissions = (\"1\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"2\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"3\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"4\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"5\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"6\" | \"F\": 0.166667;" << std::endl;
      out << "\t             \"1\" | \"L\": 0.5;" << std::endl;
      out << "\t             \"2\" | \"L\": 0.1;" << std::endl;
      out << "\t             \"3\" | \"L\": 0.1;" << std::endl;
      out << "\t             \"4\" | \"L\": 0.1;" << std::endl;
      out << "\t             \"5\" | \"L\": 0.1;" << std::endl;
      out << "\t             \"6\" | \"L\": 0.1)" << std::endl;
      out << "\tinitial_probabilities = (\"F\":  0.5;" << std::endl;
      out << "\t \"L\": 0.5)" << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr < HiddenMarkovModelCreator> HiddenMarkovModelCreatorPtr;
}

#endif
