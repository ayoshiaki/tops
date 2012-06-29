/*
 *       ProfileHiddenMarkovModel.cpp
 *
 *      Copyright 2011  Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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

#include <boost/numeric/ublas/matrix.hpp>
#include "Alphabet.hpp"
#include "ProfileHiddenMarkovModel.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdio.h>

namespace tops {

  ////////////////////////////////////////////////
  ////// Model Decoding///////////////////////////
  ////////////////////////////////////////////////

  double ProfileHiddenMarkovModel::forward(const Sequence & s, Matrix &alpha) const
  {
    return 0.0;
  }

  double ProfileHiddenMarkovModel::backward(const Sequence & s, Matrix &beta) const
  {
    return 0.0;
  }

  double ProfileHiddenMarkovModel::viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const
  {
    return 0.0;
  }


  ////////////////////////////////////////////////
  ////// Model training///////////////////////////
  ////////////////////////////////////////////////

  void ProfileHiddenMarkovModel::trainMaxLikelihood(SequenceList & observedEmissions, SequenceList & observedStates, int pseudocout)
  {
  }

  void ProfileHiddenMarkovModel::trainBaumWelch (SequenceList & training_set, int maxiterations, double diff)
  {
  }

  ////////////////////////////////////////////////
  ////// Model Initialization ////////////////////
  ////////////////////////////////////////////////

  void ProfileHiddenMarkovModel::initialize(const ProbabilisticModelParameters & parameters) {
	    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
	    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
	    ProbabilisticModelParameterValuePtr initial_probabilities = parameters.getMandatoryParameterValue("initial_probabilities");
	    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
	    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");

	    std::vector<HMMStatePtr> state_list;
	    AlphabetPtr states = AlphabetPtr(new Alphabet());
	    AlphabetPtr observations = AlphabetPtr(new Alphabet());
	    states->initializeFromVector(state_names->getStringVector());
	    observations->initializeFromVector(observation_symbols->getStringVector());

	    std::map<std::string,double>::const_iterator it;
	    std::map<std::string,std::vector<double> >::const_iterator it2;

	    DiscreteIIDModelPtr pi = DiscreteIIDModelPtr(new DiscreteIIDModel());
	    pi->initializeFromMap(initial_probabilities->getDoubleMap(), states);

	    std::map<std::string,double> emisspar = emissions->getDoubleMap();
	    std::map<std::string,double> transpar = transitions->getDoubleMap();

	    std::map<std::string,DoubleVector> emiss;
	    std::map<std::string,DoubleVector> trans;


	    for(it = emisspar.begin(); it != emisspar.end(); it++)
	      {
	        std::vector<std::string> splited;
	        boost::regex separator("\\|");
	        split_regex(it->first, splited, separator);
	        if(splited.size() == 1)
	          splited.push_back("");
	        std::string symbol(splited[0]);
	        std::string state(splited[1]);


	        std::map<std::string,DoubleVector>::iterator eit;
	        eit = emiss.find(state);
	        if(eit == emiss.end())
	          {
	            int id = observations->getSymbol(symbol)->id();
	            emiss[state].resize(observations->size());
	            if((id >= 0) && (id < (int)(emiss[state]).size()))
	              (emiss[state])[id] = it->second;
	          }
	        else
	          {
	            int id = observations->getSymbol(symbol)->id();
	            if((id >= 0) && (id < (int)(eit->second).size()))
	              (eit->second)[id] = it->second;
	          }
	      }


	    for(it = transpar.begin(); it != transpar.end(); it++)
	      {
	        std::vector<std::string> splited;
	        boost::regex separator("\\|");
	        split_regex(it->first, splited, separator);
	        if(splited.size() == 1)
	          splited.push_back("");

	        std::string to(splited[0]);
	        std::string from(splited[1]);
	        if(trans.find(from) == trans.end())
	          {
	            int id = states->getSymbol(to)->id();
	            DoubleVector probs;
	            probs.resize(states->size());
	            trans[from]=probs;
	            if(id < (int)trans[from].size())
	              trans[from][id] = it->second;
	          }
	        else
	          {
	            int id = states->getSymbol(to)->id();
	            if(id < (int)trans[from].size())
	              trans[from][id] = it->second;
	          }
	      }
	    for(unsigned int i = 0; i < states->size(); i++)
	      {
	        SymbolPtr state_name = states->getSymbol(i);
	        DiscreteIIDModelPtr e ;
	        DiscreteIIDModelPtr t ;
	        it2 = emiss.find(state_name->name());
	        if(it2 != emiss.end())
	          e = DiscreteIIDModelPtr(new DiscreteIIDModel(it2->second));
	        it2 = trans.find(state_name->name());
	        if(it2 != trans.end())
	          t = DiscreteIIDModelPtr(new DiscreteIIDModel(it2->second));
	        else {
	          std::cerr << "ERROR: Could not configure the state " << state_name->name() << "!" << std::endl;
	          exit(-1);
	        }
	        HMMStatePtr statePtr = HMMStatePtr(new HMMState(state_list.size(), state_name, e, t));
	        state_list.push_back(statePtr);
	      }

	 //   setStates(state_list, states);
	 //   setInitialProbability(pi);
	 //   setObservationSymbols(observations);
  }

  ////////////////////////////////////////////////////
  ///////// Auxiliary and debug functions ////////////
  ////////////////////////////////////////////////////

  std::string ProfileHiddenMarkovModel::getStateName(int state) const {
    return getState(state)->getName()->name();
  }

  Sequence &  ProfileHiddenMarkovModel::chooseObservation ( Sequence & h, int i, int state) const
  {
    if((state >= 0) && (!getState(state)->isSilent() ))
      return getState(state)->emission()->chooseWithHistory(h,i,1);
    return h;
  }
  int ProfileHiddenMarkovModel::chooseState(int state) const
  {
    return getState(state) ->transitions()->choose();
  }
  int ProfileHiddenMarkovModel::chooseFirstState() const
  {
    return _initial_probability->choose();
  }


  /*  void ProfileHiddenMarkovModel::silentStatesSort(vector<PHMMStatePtr> silStates){
    int swap = 1;
    while(swap == 1){
      swap = 0;
      for(int i = 0; i < (int)silStates.size() - 1; i++){
        IntVector v = silStates[i]->iTransitions();
        std::vector<int>::iterator it;
        it = find(v.begin(), v.end(), silStates[i+1]->getId());
        if(it != v.end()){
          PHMMStatePtr aux = silStates[i];
          silStates[i] = silStates[i+1];
          silStates[i+1] = aux;
          swap = 1;
        }
      }
    }
    }*/


  /*  std::string ProfileHiddenMarkovModel::str () const
  {
    int nstates = _states.size();
    std::stringstream out;
    out << "model_name = \"ProfileHiddenMarkovModel\"" << std::endl;
    out << "state_names = (" ;
    out << "\"" << getStateName(0) << "\"";
    for(int i = 1; i < (int)getStateNames()->size(); i++)
      out << ",\"" << getStateName(i) << "\"";
    out << ")" << std::endl;

    out << "observation_symbols = (" ;
    out << "\"" << alphabet()->getSymbol(0)->name() << "\"";
    for(int i = 1; i < (int)alphabet()->size(); i++)
      out << ",\"" << alphabet()->getSymbol(i)->name() << "\"";
    out << ")" << std::endl;

    out << "transitions = (" ;
    int it = 0;
    for(int i = 0; i < nstates; i++){
      for(int j = 0; j < nstates; j++){
          if(close(exp(_states[i]->transitions()->log_probability_of(j)), 0.0, 1e-10))
            continue;
          if(it == 0){
            out << "\"" << getStateName(i) << "\" | \"" << getStateName(i) << "\": " << exp(_states[i]->transitions()->log_probability_of(j)) ;
            it++;
          }
          else
            out << ";\n \"" << getStateName(j) << "\" | \"" << getStateName(i) << "\": " << exp(_states[i]->transitions()->log_probability_of(j));
      }
    }
    out << ";)" << std::endl;


    out << "emission_probabilities = (" ;
    it = 0;
    for(int i = 0; i < nstates; i++){
      for(int k = 0; k < (int)alphabet()->size(); k++){
        for(int j = 0; j < (int)alphabet()->size(); j++){
          if(close(exp(_states[i]->emission()->log_probability_of_Profile(k,j)), 0.0, 1e-10))
            continue;
          if(it == 0){
            out << "\"" <<  alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" << getStateName(i) << "\": " << exp(_states[i]->emission()->log_probability_of_Profile(k,j)) ;
            it++;
          }
          else
            out << ";\n \"" << alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" <<  getStateName(i) << "\": " << exp(_states[i]->emission()->log_probability_of_Profile(k,j));
        }
      }
    }
    out << ";)" << std::endl;

    out << "number_of_emissions = (";
    out << "\"" << getStateName(0) << "\" : \"" << _states[0]->eSeq1() << "," << _states[0]->eSeq2() << "\"";
    for(int i = 1; i < nstates; i++)
      out << ";\n\"" << getStateName(i) << "\" : \"" << _states[i]->eSeq1() << "," << _states[i]->eSeq2() << "\"";
    out << ";)" << endl;
    return out.str();
    }  */

  /*  void ProfileHiddenMarkovModel::generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path){
    Sequence s1, s2, p;
    int state_id = _begin_id;
    int obs1 = _gap_id;
    int obs2 = _gap_id;
    p.push_back(state_id);
    s1.push_back(obs1);
    s2.push_back(obs2);

    while(state_id != _end_id){
      state_id = getPHMMState(state_id)->transitions()->choose();
      if(_states[state_id]->eSeq1() == 0 && _states[state_id]->eSeq2() == 0){
        p.push_back(state_id);
        s1.push_back(_gap_id);
        s2.push_back(_gap_id);
        continue;
      }
      _states[state_id]->emission()->chooseProfile(&obs1,&obs2);
      p.push_back(state_id);
      s1.push_back(obs1);
      s2.push_back(obs2);
    }
    seq1 = s1;
    seq2 = s2;
    path = p;
    }*/
}


