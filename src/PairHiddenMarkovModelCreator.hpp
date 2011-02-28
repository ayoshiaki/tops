#ifndef PAIR_HIDDEN_MARKOV_MODEL_CREATOR_HPP
#define PAIR_HIDDEN_MARKOV_MODEL_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the finite discrete distribution
  class PairHiddenMarkovModelCreator : public ProbabilisticModelCreator 
  {
  public:
    PairHiddenMarkovModelCreator() {}
    virtual ~PairHiddenMarkovModelCreator(){};
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
      out << "\tmodel_name = \"PairHiddenMarkovModel\"" << std::endl;
      out << "\tstate_names = (\"I\",\"D\",\"M\")" << std::endl;
      out << "\tobservation_symbols = (\"A\",\"T\",\"C\",\"G\")" << std::endl;
      out << "\ttransitions = (\"M\" | \"M\": 0.7;" << std::endl;
      out << "\t \"M\" | \"I\": 0.15;" << std::endl;
      out << "\t \"M\" | \"D\": 0.3;" << std::endl;
      out << "\t \"I\" | \"M\": 0.7)" << std::endl;
      out << "\t \"I\" | \"I\": 0.7)" << std::endl;
      out << "\t \"D\" | \"D\": 0.7)" << std::endl;
      out << "\t \"D\" | \"M\": 0.15)" << std::endl;
      out << "\temissions = (\"AA\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"AT\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"AC\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"AG\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"TA\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"TT\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"TC\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"TG\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"CA\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"CT\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"CC\" | \"M\": 0.0625;" << std::endl;
      out << "\t             \"CG\" | \"M\": 0.0625)" << std::endl;
      out << "\t             \"GA\" | \"M\": 0.0625)" << std::endl;
      out << "\t             \"GT\" | \"M\": 0.0625)" << std::endl;
      out << "\t             \"GC\" | \"M\": 0.0625)" << std::endl;
      out << "\t             \"GG\" | \"M\": 0.0625)" << std::endl;
      out << "\t             \"A_\" | \"I\": 0.25)" << std::endl;
      out << "\t             \"T_\" | \"I\": 0.25)" << std::endl;
      out << "\t             \"C_\" | \"I\": 0.25)" << std::endl;
      out << "\t             \"G_\" | \"I\": 0.25)" << std::endl;
      out << "\t             \"_A\" | \"D\": 0.25)" << std::endl;
      out << "\t             \"_T\" | \"D\": 0.25)" << std::endl;
      out << "\t             \"_C\" | \"D\": 0.25)" << std::endl;
      out << "\t             \"_G\" | \"D\": 0.25)" << std::endl;
      out << "\tinitial_probabilities = (\"M\":  1.0;" << std::endl;
      out << "\t \"I\": 0.0)" << std::endl;
      out << "\t \"D\": 0.0)" << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr <PairHiddenMarkovModelCreator> PairHiddenMarkovModelCreatorPtr;
}

#endif
