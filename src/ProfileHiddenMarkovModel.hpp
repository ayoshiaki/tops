/*
 *       ProfileProfileHiddenMarkovModel.hpp
 *
 *       Copyright 2011 Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andr� Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bon�dio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *                      Felipe Amado <amadofelipe@gmail.com>
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

#ifndef Profile_ProfileHidden_MARKOV_MODEL
#define Profile_ProfileHidden_MARKOV_MODEL


#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"
#include "HiddenMarkovModel.hpp"
#include "ProfileHiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops{

  class DLLEXPORT ProfileHiddenMarkovModel : public DecodableModel
    {
    public:

      ProfileHiddenMarkovModel() {
      };

      ProfileHiddenMarkovModel( std::vector <HMMStatePtr> states, DiscreteIIDModelPtr initial_probability, AlphabetPtr state_names, AlphabetPtr observation_symbols) :    _states(states) , _initial_probability(initial_probability), _state_names (state_names) {
        tops::ProbabilisticModel::setAlphabet(observation_symbols);
      }

      void setStates(std::vector<HMMStatePtr> states) {
        _states = states;
      }

      virtual ~ProfileHiddenMarkovModel(){}

      //! Choose the observation given a state
      virtual Sequence &  chooseObservation ( Sequence & h,int i,  int state) const ;
      //! Choose a state
      virtual int chooseState(int state ) const ;
      //! Choose first state
      virtual int chooseFirstState() const ;


      virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const;


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


      virtual int getStateIndex(char type, int index) const;

      //! Forward algorithm
      virtual double forward(const Sequence & s, Matrix &alpha) const;

      //! Backward algorithm
      virtual double backward(const Sequence & s, Matrix &beta) const;

      //! Viterbi algorithm
      virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const ;

      virtual std::string model_name() const {
        return "ProfileHiddenMarkovModel";
      }
/*
      virtual ProbabilisticModelCreatorPtr getFactory() const {
        return ProfileHiddenMarkovModelCreatorPtr(new ProfileHiddenMarkovModelCreator());
      }
      */

      virtual DecodableModel * decodable()  {
        return this;
      }


      virtual void trainMaxLikelihood(SequenceList & observedStates, SequenceList & observedEmissions, int pseudocouts);
      virtual void trainBaumWelch (SequenceList & training_set, int maxiterations, double diff) ;

      virtual void initialize(const ProbabilisticModelParameters & par) ;

      virtual ProbabilisticModelParameters parameters() const ;

      void setInitialProbability(DiscreteIIDModelPtr initial) ;
      void setObservationSymbols(AlphabetPtr obs) ;
      void setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) ;

      virtual ProfileHiddenMarkovModel * profileDecodable() {
        return this;
      }


    private:
      std::vector <HMMStatePtr> _states;
      DiscreteIIDModelPtr _initial_probability;
      std::vector<double> _ctFactors;
      AlphabetPtr _state_names;
      //void scale(std::vector<double> & in, int t);
      double max(double a, double b, double c) const;
      double max(double a, double b) const;
      //std::vector<double> iterate(Sequence & obs);
    };

    typedef boost::shared_ptr<ProfileHiddenMarkovModel> ProfileHiddenMarkovModelPtr;

}




#endif

