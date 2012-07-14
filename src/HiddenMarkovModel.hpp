/*
 *       HiddenMarkovModel.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
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

#ifndef HIDDEN_MARKOV_MODEL_HPP
#define HIDDEN_MARKOV_MODEL_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"

#include "HiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  class DLLEXPORT HMMState {
  protected:
    int _id;
    SymbolPtr _name;
    DiscreteIIDModelPtr _emission;
    DiscreteIIDModelPtr _transitions;
  public:
    HMMState(){}
    HMMState (int id, SymbolPtr name, DiscreteIIDModelPtr emission,  DiscreteIIDModelPtr transitions) : _id(id), _name(name), _emission(emission), _transitions(transitions) {}
    void setName (SymbolPtr name) {
      _name = name;
    }
    void setEmissions (DiscreteIIDModelPtr e)
    {
      _emission = e;
    }
    void setTransition (DiscreteIIDModelPtr t)
    {
      _transitions = t;
    }
    DiscreteIIDModelPtr &emission() {
      return _emission;
    }
    DiscreteIIDModelPtr &transitions() {
      return _transitions;
    }
    bool isSilent() {
      return (_emission == NULL);
    }
    SymbolPtr getName() const {
      return _name;
    }
    int getId() {
      return _id;
    }
    void setId(int i) {
      _id = i;
    }


  };
  typedef boost::shared_ptr <HMMState> HMMStatePtr;

  //! This class represents a hidden markov model
  class DLLEXPORT HiddenMarkovModel :   public DecodableModel
  {
  public:

    HiddenMarkovModel() {
    };

    HiddenMarkovModel( std::vector <HMMStatePtr> states, DiscreteIIDModelPtr initial_probability, AlphabetPtr state_names, AlphabetPtr observation_symbols) :    _states(states) , _initial_probability(initial_probability), _state_names (state_names) {
      tops::ProbabilisticModel::setAlphabet(observation_symbols);


    }


    void setStates(std::vector<HMMStatePtr> states) {
      _states = states;
    }


    virtual ~HiddenMarkovModel(){}

    //! Choose the observation given a state
    virtual Sequence &  chooseObservation ( Sequence & h,int i,  int state) const ;
    //! Choose a state
    virtual int chooseState(int state ) const ;
    //! Choose first state
    virtual int chooseFirstState() const ;
    virtual AlphabetPtr getStateNames() const {
      return _state_names;
    }
    virtual std::string getStateName(int state) const;

    virtual std::string str () const ;

    virtual void setState (int id, HMMStatePtr state)
    {
      if(_states.size() < _state_names->size())
        _states.resize(_state_names->size());
      _states[id] = state;
      state->setId(id);
    }

    virtual HMMStatePtr getState(int id) const
    {
      return _states[id];
    }
    //! Forward algorithm
    virtual double forward(const Sequence & s, Matrix &alpha) const;

    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const;

    //! Viterbi algorithm
    virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const ;

    virtual std::string model_name() const {
      return "HiddenMarkovModel";
    }
    virtual ProbabilisticModelCreatorPtr getFactory() const {
      return HiddenMarkovModelCreatorPtr(new HiddenMarkovModelCreator());
    }
    virtual DecodableModel * decodable()  {
      return this;
    }
    virtual void trainBaumWelch (SequenceList & training_set, int maxiterations, double diff) ;

    virtual void initialize(const ProbabilisticModelParameters & par) ;

    virtual ProbabilisticModelParameters parameters() const ;

    void setInitialProbability(DiscreteIIDModelPtr initial) ;
    void setObservationSymbols(AlphabetPtr obs) ;
    void setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) ;



  private:
    std::vector <HMMStatePtr> _states;
    DiscreteIIDModelPtr _initial_probability;
    std::vector<double> _ctFactors;
    AlphabetPtr _state_names;
    void scale(std::vector<double> & in, int t);
    std::vector<double> iterate(Sequence & obs);
  };

  typedef boost::shared_ptr<HiddenMarkovModel> HiddenMarkovModelPtr;
}


#endif
