/*
 *       ProfileHiddenMarkovModel.cpp
 *
 *      Copyright 2011  Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bon�dio <ibonadio@ime.usp.br>
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
#include <cfloat>
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
    int st;
    int n_states = _states.size();
    int n_match_states = (n_states + 3)/3;
    for (st = 0; st < n_states && !getState(st)->emission();++st);
    if (st == n_states) {
      cout << "ERROR: There is only silent states." << endl;
      exit(-1);
    }
    int emiss_size = getState(st)->emission()->size() + 1;
    int seq_len = s.size() + 1;

    Matrix mat(n_states,seq_len);
    Matrix ins(n_states,seq_len);
    Matrix del(n_states,seq_len);

    mat.clear();
    ins.clear();
    del.clear();

    for (int j = 0; j == 0 && j < n_match_states; j++) {
      for (int i = 1; i < seq_len; ++i) {
          ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                      + log(emiss_size)
                      + log_sum(
                          _states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                          + mat(j,i-1),
                          _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                          + ins(j,i-1));
      }
    }

    for (int j = 1; j == 1 && j < n_match_states; j++) {
        for (int i = 1; i < seq_len; ++i) {

            mat(j,i) = _states[getStateIndex('M',j)]->emission()->log_probability_of(s[i-1])
                        + log(emiss_size)
                        + log_sum(
                              _states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                              + mat(j-1,i-1),
                              _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                              + ins(j-1,i-1));

            ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                        + log(emiss_size)
                        + log_sum(
                            log_sum(
                              _states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                              + mat(j,i-1),
                              _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                              + ins(j,i-1)),
                              _states[getStateIndex('D',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                              + del(j,i-1));

            del(j,i) = log_sum(
                          _states[getStateIndex('M',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j))
                          + mat(j-1,i),
                          _states[getStateIndex('I',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j))
                          + ins(j-1,i));
        }
    }

    for (int j = 2; j < n_match_states - 1; j++) {
        for (int i = 1; i < seq_len; ++i) {

            mat(j,i) = _states[getStateIndex('M',j)]->emission()->log_probability_of(s[i-1])
                        + log(emiss_size)
                        + log_sum(
                            log_sum(
                              _states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                              + mat(j-1,i-1),
                              _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                              + ins(j-1,i-1)),
                              _states[getStateIndex('D',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                              + del(j-1,i-1));


            ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                            + log(emiss_size)
                            + log_sum(
                                log_sum(
                                  _states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                                  + mat(j,i-1),
                                  _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                                  + ins(j,i-1)),
                                  _states[getStateIndex('D',j)]->transitions()->log_probability_of(getStateIndex('I',j))
                                  + del(j,i-1));

            del(j,i) = log_sum(
                          log_sum(
                            _states[getStateIndex('M',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j))
                            + mat(j-1,i),
                            _states[getStateIndex('I',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j))
                            + ins(j-1,i)),
                            _states[getStateIndex('D',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j))
                            + del(j-1,i));

        }
    }

    for (int j = n_match_states - 1; j == n_match_states - 1 && j < n_match_states; j++) {
      for (int i = 1; i < seq_len; ++i) {
        mat(j,i) = log_sum(
                      log_sum(
                        _states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                        + mat(j-1,i-1),
                        _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                        + ins(j-1,i-1)),
                        _states[getStateIndex('D',j-1)]->transitions()->log_probability_of(getStateIndex('M',j))
                        + del(j-1,i-1));
      }
    }

    return mat(getStateIndex('M', n_match_states - 1),seq_len - 1);
  }

  //TODO
  double ProfileHiddenMarkovModel::backward(const Sequence & s, Matrix &beta) const
  {
    cerr << "ERROR: Backward not implemented for ProfileHMM." << endl;
    exit(-1);
    return 0.0;
  }


  double ProfileHiddenMarkovModel::viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const
  {
     int st;
     int n_states = _states.size();
     int n_match_states = (n_states + 3)/3;
     for (st = 0; st < n_states && !getState(st)->emission();++st);
     if (st == n_states) {
       cout << "ERROR: There is only silent states." << endl;
       exit(-1);
     }
     int emiss_size = getState(st)->emission()->size() + 1;
     int seq_len = s.size() + 1;


     Matrix mat(n_states,seq_len);
     Matrix ins(n_states,seq_len);
     Matrix del(n_states,seq_len);

     mat.clear();
     ins.clear();
     del.clear();

     for (int j = 0; j == 0 && j < n_match_states; j++) {
       for (int i = 1; i < seq_len; ++i) {
           ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                       + log(emiss_size)
                       + max(_states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + mat(j,i-1),
                             _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + ins(j,i-1));
       }
     }

     for (int j = 1; j == 1 && j < n_match_states; j++) {
         for (int i = 1; i < seq_len; ++i) {

             mat(j,i) = _states[getStateIndex('M',j)]->emission()->log_probability_of(s[i-1])
                         + log(emiss_size)
                         + max(_states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + mat(j-1,i-1),
                               _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + ins(j-1,i-1));

             ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                         + log(emiss_size)
                         + max(_states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + mat(j,i-1),
                               _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + ins(j,i-1),
                               _states[getStateIndex('D',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + del(j,i-1));

             del(j,i) = max(_states[getStateIndex('M',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j)) + mat(j-1,i),
                            _states[getStateIndex('I',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j)) + ins(j-1,i));
         }
     }

     for (int j = 2; j < n_match_states - 1; j++) {
         for (int i = 1; i < seq_len; ++i) {

             mat(j,i) = _states[getStateIndex('M',j)]->emission()->log_probability_of(s[i-1])
                         + log(emiss_size)
                         + max(_states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + mat(j-1,i-1),
                               _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + ins(j-1,i-1),
                               _states[getStateIndex('D',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + del(j-1,i-1));

             ins(j,i) = _states[getStateIndex('I',j)]->emission()->log_probability_of(s[i-1])
                             + log(emiss_size)
                             + max(_states[getStateIndex('M',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + mat(j,i-1),
                                   _states[getStateIndex('I',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + ins(j,i-1),
                                   _states[getStateIndex('D',j)]->transitions()->log_probability_of(getStateIndex('I',j)) + del(j,i-1));

             del(j,i) = max(_states[getStateIndex('M',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j)) + mat(j-1,i),
                            _states[getStateIndex('I',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j)) + ins(j-1,i),
                            _states[getStateIndex('D',j - 1)]->transitions()->log_probability_of(getStateIndex('D',j)) + del(j-1,i));

         }
     }

     for (int j = n_match_states - 1; j == n_match_states - 1 && j < n_match_states; j++) {
       for (int i = 1; i < seq_len; ++i) {
         mat(j,i) = max(_states[getStateIndex('M',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + mat(j-1,i-1),
                        _states[getStateIndex('I',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + ins(j-1,i-1),
                        _states[getStateIndex('D',j-1)]->transitions()->log_probability_of(getStateIndex('M',j)) + del(j-1,i-1));
       }
     }

     return mat(getStateIndex('M', n_match_states - 1),seq_len - 1);
  }



  ////////////////////////////////////////////////
  ////// Model training///////////////////////////
  ////////////////////////////////////////////////

  void ProfileHiddenMarkovModel::trainMaxLikelihood(SequenceList & observedStates, SequenceList & observedEmissions, int pseudocouts)
  {
    int s;
    int n_states = _states.size();
    for (s = 0; s < n_states && !getState(s)->emission();++s);
    if (s == n_states) {
      cout << "ERROR: There is only silent states." << endl;
      exit(-1);
    }
    int emiss_size = getState(s)->emission()->size() + 1;

    Matrix emiss(n_states, emiss_size);
    Matrix trans(n_states, n_states);

    emiss.clear();
    trans.clear();

    if (observedStates.size() == 0)
      return;

    // Counting emissions
    for (int seq = 0; seq < observedStates.size();++seq) {
      int pos = 0;

     // cout << observedStates.size() << endl;
     // cout << observedEmissions.size() << endl;


      /*
      for (int i = 0; i < observedStates[seq].size();++i) {
          cout << " " << getState(observedStates[seq][i])->getName()->name();
      }
      cout << endl;

      for (int i = 0; i < observedEmissions[seq].size();++i) {
          cout << " " << observedEmissions[seq][i];
      }
      cout << endl;
*/


      for (int i = 0; i < observedStates[seq].size();++i) {
      //  cout << " pos = " << pos << endl;
        if (getState(observedStates[seq][i])->emission()) {
       //     cout << "observedStates[seq][i] = " << observedStates[seq][i] << " observedEmissions[seq][pos++] = " << observedEmissions[seq][pos] << endl;
        //    cout << "emiss_size = " << emiss_size;
          emiss(observedStates[seq][i], observedEmissions[seq][pos++])++;
        }
      }
    }

    // Counting transitions
    for (int seq = 0; seq < observedStates.size(); ++seq) {
      for (int i = 0; i < observedStates[seq].size() - 1; ++i) {
          trans(observedStates[seq][i], observedStates[seq][i + 1])++;
      }
    }
    trans(*(observedStates[0].end() - 1), *(observedStates[0].end() - 1))++;

    //DEBUG:Print emiss matrix
    /*cout << "Emissions matrix:" << endl;
    for (int i = 0; i < emiss.size1(); ++i) {
      for (int j = 0; j < emiss.size2(); ++j)
          cout << emiss(i,j) << " ";
      cout << endl;
    }
    cout << endl;*/

    //DEBUG:Print trans matrix
   /* cout << "Transitions matrix:" << endl;
    for (int i = 0; i < trans.size1(); ++i) {
      for (int j = 0; j < trans.size2(); ++j)
          cout << trans(i,j) << " ";
      cout << endl;
    }
    cout << endl;*/


    // Emission Pseudocounts
    for (int i = 0; i < n_states; ++i) {
      if (getState(i)->emission()) {
        for (int symbol = 0; symbol < getState(i)->emission()->size(); ++symbol)
          emiss(i, symbol) += pseudocouts;
      }
    }


    //Transitions Pseudocounts
    int n_match_states = (n_states + 3)/3;
    for (int i = 0; i < n_match_states - 1; ++i) {
      trans (getStateIndex('M', i), getStateIndex('M', i + 1)) += pseudocouts;
      trans (getStateIndex('M', i), getStateIndex('I', i)) += pseudocouts;
      trans (getStateIndex('I', i), getStateIndex('M', i + 1)) += pseudocouts;
      trans (getStateIndex('I', i), getStateIndex('I', i)) += pseudocouts;

      if (i != 0) {
        trans (getStateIndex('D', i), getStateIndex('M', i + 1)) += pseudocouts;
        trans (getStateIndex('D', i), getStateIndex('I', i)) += pseudocouts;
        if (i != n_match_states - 2) {
            trans (getStateIndex('D', i), getStateIndex('D', i + 1)) += pseudocouts;
        }
      }
      if (i != n_match_states - 2) {
          trans (getStateIndex('M', i), getStateIndex('D', i + 1)) += pseudocouts;
          trans (getStateIndex('I', i), getStateIndex('D', i + 1)) += pseudocouts;
      }
    }

    //Emission probabilities
    for (int i = 0; i < n_states; ++i) {
      int sum = 0;
      for (int symbol = 0; symbol < emiss_size; ++symbol)
        sum += emiss (i, symbol);
      if (sum == 0)
        continue;
      for (int symbol = 0; symbol < emiss_size; ++symbol)
        emiss (i, symbol) /= sum;
    }

    //Transition probabilities
    for (int i = 0; i < n_states; ++i) {
      int sum = 0;
      for (int j = 0; j < n_states; ++j)
        sum += trans (i, j);
      if (sum == 0)
        continue;
      for (int j = 0; j < n_states; ++j)
        trans (i, j) /= sum;
    }

    //DEBUG:Print emiss matrix
    /*cout << "Emissions matrix:" << endl;
    for (int i = 0; i < emiss.size1(); ++i) {
      for (int j = 0; j < emiss.size2(); ++j)
          cout << emiss(i,j) << " ";
      cout << endl;
    }
    cout << endl;*/

    //DEBUG:Print trans matrix
    /*cout << "Transitions matrix:" << endl;
    for (int i = 0; i < trans.size1(); ++i) {
      for (int j = 0; j < trans.size2(); ++j)
          cout << trans(i,j) << " ";
      cout << endl;
    }
    cout << endl;*/

    // Puts the emission matrix info inside HMMStates
    for (int i = 0; i < n_states; ++i) {
      for (int j = 0; j < emiss_size; ++j) {
        if (getState(i)->emission() && emiss(i,j))
            getState(i)->emission()->log_probability_of(j, log(emiss(i,j)));
        else if (getState(i)->emission())
            getState(i)->emission()->log_probability_of(j, -DBL_MAX);
      }
    }

    // Puts the transition matrix info inside HMMStates
    for (int i = 0; i < n_states; ++i) {
      for (int j = 0; j < n_states; ++j) {
        if (trans(i,j))
            getState(i)->transitions()->log_probability_of(j, log(trans(i,j)));
        else
            getState(i)->transitions()->log_probability_of(j, -DBL_MAX);
      }
    }
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

	    setStates(state_list, states);
	    setInitialProbability(pi);
	    setObservationSymbols(observations);
  }

  ////////////////////////////////////////////////////
  ///////// Auxiliary and debug functions ////////////
  ////////////////////////////////////////////////////


  double ProfileHiddenMarkovModel::max(double a, double b, double c) const {
     if (a > b && a > c)
       return a;
     if (b > c)
       return b;
     return c;
   }

  double ProfileHiddenMarkovModel::max(double a, double b) const {
     return (a > b)? a:b;
   }

  std::string ProfileHiddenMarkovModel::str () const
    {
      int nstates = _states.size();
      std::stringstream out;
      out << "model_name = \"" << model_name() << "\"" << std::endl;
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
      int first = 1;
      for(int i = 0; i < nstates; i++) {
        for(int j = 0; j < nstates; j++) {
          double prob = exp(getState(i)->transitions()->log_probability_of(j));
          if (prob) {
              if (first) {
                  first = 0;
                  out << "\"";
              }
              else out << ";\n \"";
            out << getStateName(j) << "\" | \"" << getStateName(i) << "\": " << prob;
          }
        }
      }
      out << ")" << std::endl;


      out << "emission_probabilities = (" ;
      first = 1;
      for(int i = 0; i < nstates; i++)
        for(int j = 0; j < (int)alphabet()->size(); j++)
          if(getState(i)->emission()) {
              double prob = exp(getState(i)->emission()->log_probability_of(j));
              if (prob) {
                if (first) {
                  first = 0;
                  out << "\"";
                }
                else out << ";\n \"";
                out << alphabet()->getSymbol(j)->name() << "\" | \"" <<  getStateName(i) << "\": " << exp(getState(i)->emission()->log_probability_of(j));
              }
          }
      out << ")" << std::endl;

      double sum = 0;
      std::vector <double> probs;
      probs.resize(nstates);
      for(int i = 0; i < nstates; i++){
        probs[i] = exp(_initial_probability->log_probability_of(i));
        sum += probs[i];
      }

      out << "initial_probabilities = (";
      first = 1;
      for(int i = 0; i < nstates; i++) {
          int prob = probs[i]/sum;
          if (prob) {
            if (first) {
              first = 0;
              out << "\"";
            }
            else out << ";\n \"";
            out << getStateName(i) << "\": " << probs[i]/sum;
          }
      }
      out << ")" << std::endl;
      return out.str();
    }

  int ProfileHiddenMarkovModel::getStateIndex(char type, int index) const {
    stringstream name;
    name << type << index;
    return _state_names->getSymbol(name.str())->id();
  }

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

  Sequence & ProfileHiddenMarkovModel::choose(Sequence & h, Sequence & path,  int i, int size) const
    {
      assert(path.size() == h.size());
      int n_match_states = (_states.size() + 3)/3;
      int last_state = getStateIndex('M', n_match_states - 1);
      int state = chooseFirstState();
      int states_c = 0;
      int emiss_c = 0;

      if (((i - 1) < (int) path.size()) && (i - 1) >= 0)
        state = path[i - 1];

      if (path.size() > 0)
        path[0] = state;
      else
        path.push_back(state);
      states_c++;

      while (state != last_state) {
        if (getState(state)->emission()) {
          chooseObservation(h, i + emiss_c, state);
          emiss_c++;
        }

        state = chooseState(state); // next state

        if (states_c < (int) path.size())
          path[states_c] = state;
        else
          path.push_back(state);
        states_c++;
      }
      //h.resize(size); This is a possible bug in DecodableModel, I believe these lines should not be there..
      //path.resize(size);
      return h;
    }

  ProbabilisticModelParameters ProfileHiddenMarkovModel::parameters() const {
     ProbabilisticModelParameters answer;
     int nstates = _states.size();
     answer.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name().c_str())));
     answer.add("state_names", _state_names->getParameterValue());
     answer.add("observation_symbols", alphabet()->getParameterValue());
     std::map <std::string, double> trans;
     std::stringstream out;
     out << getStateName(0) << "|" << getStateName(0) ;
     trans[out.str()] =  exp(getState(0)->transitions()->log_probability_of(0)) ;
     for(int i = 0; i < nstates; i++)
       for(int j = 0; j < nstates; j++)
         if((i != 0) || (j != 0)) {
           std::stringstream out2;
           out2 << getStateName(j) << "|" << getStateName(i) ;
           trans[out2.str()] = exp(getState(i)->transitions()->log_probability_of(j));
         }
     answer.add("transitions", DoubleMapParameterValuePtr(new DoubleMapParameterValue(trans)));

     std::map <std::string, double> emission;
     std::stringstream out3;
     out3 <<   alphabet()->getSymbol(0)->name() << "|" << getStateName(0) ;
     if(getState(0)->emission())
       emission[out3.str()] =  exp(getState(0)->emission()->log_probability_of(0));
     for(int i = 0; i < nstates; i++)
       for(int j = 0; j < (int)alphabet()->size(); j++)
         if(((i != 0) || (j != 0)) && getState(i)->emission()){
           std::stringstream out4;
           out4 <<alphabet()->getSymbol(j)->name() << "|" <<  getStateName(i) ;
           emission[out4.str()] =  exp(getState(i)->emission()->log_probability_of(j));
         }
     answer.add("emission_probabilities", DoubleMapParameterValuePtr(new DoubleMapParameterValue(emission)));
     double sum = 0;
     std::vector <double> probs;
     probs.resize(nstates);
     for(int i = 0; i < nstates; i++){
       probs[i] = exp(_initial_probability->log_probability_of(i));
       sum += probs[i];
     }
     std::map <std::string, double> initial;
     std::stringstream out5;
     out5 <<  getStateName(0);
     initial[out5.str()] =  probs[0]/sum;
     for(int i = 0; i < nstates; i++)
       for(int j = 0; j < (int)alphabet()->size(); j++)
         if((i != 0) || (j != 0)){
           std::stringstream out6;
           out6 << getStateName(i);
           initial[out6.str()] = probs[i]/sum;
         }
     answer.add("initial_probabilities", DoubleMapParameterValuePtr(new DoubleMapParameterValue(initial)));
     return answer;

   }


   void ProfileHiddenMarkovModel::setInitialProbability(DiscreteIIDModelPtr initial) {
     _initial_probability = initial;
   }
   void ProfileHiddenMarkovModel::setObservationSymbols(AlphabetPtr obs) {
     tops::ProbabilisticModel::setAlphabet(obs);
   }
   void ProfileHiddenMarkovModel::setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) {
     _states = states;
     _state_names = state_names;
   }
}


