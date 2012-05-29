#ifndef GENERALIZED_PAIR_HIDDEN_MARKOV_MODEL_CREATOR_HPP
#define GENERALIZED_PAIR_HIDDEN_MARKOV_MODEL_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  class GeneralizedPairHiddenMarkovModelCreator : public ProbabilisticModelCreator 
  {
  public:
    GeneralizedPairHiddenMarkovModelCreator() {}
    virtual ~GeneralizedPairHiddenMarkovModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tstate_names" << std::endl;
      out << "\tobservation_symbols" << std::endl;
      out << "\tinitial_probabilities" << std::endl;
      out << "\tend_probabilities" << std::endl;
      out << "\tnumber_of_emissions" << std::endl;
      out << "\tduration_probabilities" << std::endl;
      out << "\ttransitions" << std::endl;
      out << "\temission_models" << std::endl;
      out << "Example: " << std::endl;
      out << "\tmodel_name = \"GeneralizedPairHiddenMarkovModel\"" << std::endl;
      out << "\tstate_names = (\"N\",\"C\")" << std::endl;
      out << "\tobservation_symbols = (\"A\",\"T\",\"C\",\"G\")" << std::endl;
      out << "\ttransitions = (\"N\" | \"N\": 0.899;" << std::endl;
      out << "\t \"C\" | \"N\": 0.1;" << std::endl;
      out << "\t \"N\" | \"C\": 0.999;" << std::endl;
      out << "\t(\"C\" : \"codingEmissionModel.txt\";" << std::endl;
      out << "\t\"N\" : \"noncodingEmissionModel.txt\";)" << std::endl;
      out << "\tinitial_probabilities = (\"N\":  0.9;" << std::endl;
      out << "\t \"C\": 0.1)" << std::endl;
      out << "end_probabilities = (\"N\" = 0.0001;" << std::endl;
      out << "\"C\" = 0.0001;)" << std::endl;
      out << "number_of_emissions = (\"C\" : \"1-1000,1-1000\";" << std::endl;
      out << "\"N\" : \"0-1,0-1\";)" << std::endl;
      out << "duration_probabilities = (\"C\" : \"durationCoding.txt\";)" << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr <GeneralizedPairHiddenMarkovModelCreator> GeneralizedPairHiddenMarkovModelCreatorPtr;
}

#endif
