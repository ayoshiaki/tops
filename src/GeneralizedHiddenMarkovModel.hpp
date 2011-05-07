/*
 *       GeneralizedHiddenMarkovModel.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef GENERALIZED_HIDDEN_MARKOV_MODEL_H
#define GENERALIZED_HIDDEN_MARKOV_MODEL_H
#include <set>

#include "MultinomialDistribution.hpp"
#include "ProbabilisticModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "DecodableModel.hpp"
#include "GHMMStates.hpp"
#include "util.hpp"

namespace tops {


  //! This is a class representing Hidden semi-Markov Models
  class GeneralizedHiddenMarkovModel: public DecodableModel {
  private:
      Matrix _alpha;
      Sequence _last;
      MultinomialDistributionPtr _last_state_probabilities;
      MultinomialDistributionPtr _duration_state_probabilities;
    MultinomialDistributionPtr _initial_probabilities;
    MultinomialDistributionPtr _terminal_probabilities;
    GHMMStates _all_states;
    AlphabetPtr _state_names;
    GHMMStates _geometric_duration_states;
    GHMMSignalStates _signal_states;
    GHMMExplicitDurationStates _explicit_duration_states;
    void initialize_prefix_sum_arrays(const Sequence & s) const;
    void buildDoubleParameterValue(MultinomialDistributionPtr distr, ProbabilisticModelParameters & answer, const char *) const;
    void restore_model(std::string & model_name, const ProbabilisticModelParameters & parameters);
    std::map<std::string, ProbabilisticModelPtr> _models;
  public:
    GeneralizedHiddenMarkovModel() {
    }

    virtual ~GeneralizedHiddenMarkovModel() {
    }

    void fixStatesPredecessorSuccessor();

    virtual double efficient_forward(const Sequence & s, Matrix &alpha) const;

    virtual std::string print_graph () const ;

    //! Forward algorithm
    virtual double forward(const Sequence & s, Matrix &alpha) const;

    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const;

    //! Viterbi algorithm
    virtual double
    viterbi(const Sequence &s, Sequence &path, Matrix & gamma) const;

      //! Choose a path given a sequence_length
      virtual void choosePath(const Sequence &s, Sequence &path) ;

      virtual void initializeChoosePathAlgorithm(const Sequence &s);
      
    //! Inefficient Viterbi algorithm
    virtual double _viterbi(const Sequence &s, Sequence &path, Matrix & gamma) const;

    //! Choose the observation given a state
    virtual Sequence & chooseObservation(Sequence & h, int i, int state) const;

    //! Choose a state
    virtual int chooseState(int state) const;

    //! Choose the initial state
    virtual int chooseFirstState() const;

    //! Choose the initial state
    virtual MultinomialDistributionPtr getInitialProbabilities() const {
      return _initial_probabilities;
    }

    //! Get state name
    virtual std::string getStateName(int state) const;

    //! Get the state names
    virtual AlphabetPtr getStateNames() const;
    virtual std::string model_name() const {
      return "GeneralizedHiddenMarkovModel";
    }

    virtual ProbabilisticModelCreatorPtr getFactory() const;
    virtual std::string str() const;

    virtual DecodableModel * decodable() {
      return this;
    }
      void configureExplicitDurationState(std::string observation_model_name, MultinomialDistributionPtr transition_distr,
                                          std::string duration_model_name, std::string state_name, int iphase, int ophase);

    void configureSignalState(std::string observation_model_name,
                              MultinomialDistributionPtr transition_distr,
                              int size, std::string state_name, int iphase, int ophase);

    void configureGeometricDurationState(std::string observation_model_name,
                                         MultinomialDistributionPtr transition_distr,
                                         std::string state_name, int iphase, int ophase);
    void setInitialProbability(MultinomialDistributionPtr init);
    void setTerminalProbability(MultinomialDistributionPtr term);
    void setObservationSymbols(AlphabetPtr obs) {
      setAlphabet(obs);
    }
    void setStateNames(AlphabetPtr alphabet);
    virtual ProbabilisticModelParameters parameters() const ;
    virtual void initialize(const ProbabilisticModelParameters & p) ;


  };

  typedef boost::shared_ptr<GeneralizedHiddenMarkovModel>
  GeneralizedHiddenMarkovModelPtr;
}
#endif
