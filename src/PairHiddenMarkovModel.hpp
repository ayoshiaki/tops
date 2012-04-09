/*
 *       PairHiddenMarkovModel.hpp
 *
 *       Copyright 2011 Vitor Onuchic <vitoronuchic@gmail.com>
 *                      André Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#ifndef PAIR_HIDDEN_MARKOV_MODEL
#define PAIR_HIDDEN_MARKOV_MODEL

#include "crossplatform.hpp"

#include "HiddenMarkovModel.hpp"
#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"
#include "Symbol.hpp"
#include "SparseMatrix.hpp"
#include "PairHiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "Symbol.hpp"
#include <cstdarg>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <algorithm>

namespace tops{
  class DLLEXPORT PHMMState: public HMMState {

  private:
    int _emissionSeq1, _emissionSeq2;
    IntVector _incomingTransitions, _outgoingTransitions;

  public:
    PHMMState(){}

    PHMMState(int id, SymbolPtr name, MultinomialDistributionPtr emission,  MultinomialDistributionPtr transitions, IntVector iTransitions, IntVector oTransitions, int eSeq1, int eSeq2){
      _id = id;
      _name = name;
      _emission = emission;
      _transitions = transitions;
      _incomingTransitions = iTransitions;
      _outgoingTransitions = oTransitions;
      _emissionSeq1 = eSeq1;
      _emissionSeq2 = eSeq2;
    }

    int eSeq1(){
      return _emissionSeq1;
    }

    int eSeq2(){
      return _emissionSeq2;
    }

    void setNumEmissionSeq1(int i){
      _emissionSeq1 = i;
    }

    void setNumemissionSeq2(int i){
      _emissionSeq2 = i;
    }

    vector<int> iTransitions() const{
      return _incomingTransitions;
    }

    int getITransId(int i){
      return _incomingTransitions[i];
    }

    vector<int> oTransitions(){
      return _outgoingTransitions;
    }

    int getOTransId(int i){
      return _outgoingTransitions[i];
    }
  };

  typedef boost::shared_ptr <PHMMState> PHMMStatePtr;

  class DLLEXPORT PairHiddenMarkovModel : public ProbabilisticModel{
  private:
    int _gap_id;
    int _end_id;
    int _begin_id;
    std::vector<PHMMStatePtr> _states;
    AlphabetPtr _state_names;
    std::vector<PHMMStatePtr> _silent_states;

  public:

    PairHiddenMarkovModel() {
    };

    PairHiddenMarkovModel( std::vector <PHMMStatePtr> states, AlphabetPtr state_names, AlphabetPtr observation_symbols) {
      _states = states;
      _state_names = state_names;
      tops::ProbabilisticModel::setAlphabet(observation_symbols);
    }

    int getGapId(){
      return _gap_id;
    }

    virtual ~PairHiddenMarkovModel(){}

    //Forward, Backward and Viterbi algorithms. We assume there are no cycles of silent states.
    virtual double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double backwardSum(int k,int i,int j,vector<Matrix> &a,const Sequence & seq1, const Sequence & seq2);

    virtual double viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & path, Sequence & al1, Sequence & al2, vector<Matrix> &a);

    virtual float posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, SparseMatrixPtr &ppMatch,SparseMatrixPtr &ppGap1, SparseMatrixPtr &ppGap2);

    virtual float expectedAccuracy(SparseMatrixPtr postProbs, SparseMatrixPtr postProbsGap1, SparseMatrixPtr postProbsGap2);

    virtual float expectedAccuracyWithGaps(SparseMatrixPtr postProbs, SparseMatrixPtr postProbsGap1, SparseMatrixPtr postProbsGap2);

    virtual void generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path);

    virtual void trainBaumWelch(SequenceList & sample, int maxiterations, double diff_threshold);

    virtual std::string model_name() const {
      return "PairHiddenMarkovModel";
    }

    virtual ProbabilisticModelCreatorPtr getFactory() const {
      return PairHiddenMarkovModelCreatorPtr(new PairHiddenMarkovModelCreator());
    }

    virtual void initialize(const ProbabilisticModelParameters & par) ;

    void setStates(std::vector<PHMMStatePtr> states, AlphabetPtr state_names){
      _states = states;
      _state_names = state_names;
    }

    void setObservationSymbols(AlphabetPtr obs){
      tops::ProbabilisticModel::setAlphabet(obs);
    }

    void setSilentStates(std::vector<PHMMStatePtr> states){
      _silent_states = states;
    }

    PHMMStatePtr getPHMMState(int id) const {
      return _states[id];
    }

    int getSilentId(int k) const {
      return _silent_states[k]->getId();
    }

    virtual AlphabetPtr getStateNames() const  {
      return _state_names;
    }

    virtual string getStateName(int state) const{
      return _state_names->getSymbol(state)->name();
    }

    std::string str () const;

    void silentStatesSort(vector<PHMMStatePtr> silStates);

    virtual PairHiddenMarkovModel * pairDecodable() {
      return this;
    }

  };

  typedef boost::shared_ptr<PairHiddenMarkovModel> PairHiddenMarkovModelPtr;
}

#endif

