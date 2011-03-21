#include "Alphabet.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "util.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <algorithm>

namespace tops {

  

  std::string PairHiddenMarkovModel::str ()
  {
    int nstates = _states.size();
    std::stringstream out;
    out << "model_name = \"PairHiddenMarkovModel\"" << std::endl;
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
	  if(close(exp(getPHMMState(i)->transitions()->log_probability_of(j)), 0.0, 1e-10))
	    continue;
	  if(it == 0){
	    out << "\"" << getStateName(i) << "\" | \"" << getStateName(i) << "\": " << exp(getPHMMState(i)->transitions()->log_probability_of(j)) ;
	    it++;
	  }
	  else
	    out << ";\n \"" << getStateName(j) << "\" | \"" << getStateName(i) << "\": " << exp(getPHMMState(i)->transitions()->log_probability_of(j));
      }
    }
    out << ";)" << std::endl;
    
    
    out << "emission_probabilities = (" ;
    it = 0;
    for(int i = 0; i < nstates; i++){
      for(int k = 0; k < (int)alphabet()->size(); k++){
	for(int j = 0; j < (int)alphabet()->size(); j++){
	  if(close(exp(getPHMMState(i)->emission()->log_probability_of_pair(k,j)), 0.0, 1e-10))
	    continue;
	  if(it == 0){
	    out << "\"" <<  alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" << getStateName(i) << "\": " << exp(getPHMMState(i)->emission()->log_probability_of_pair(k,j)) ;
	    it++;
	  }
	  else
	    out << ";\n \"" << alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" <<  getStateName(i) << "\": " << exp(getPHMMState(i)->emission()->log_probability_of_pair(k,j));
	}
      }
    }
    out << ";)" << std::endl;
    
    out << "number_of_emissions = (";
    out << "\"" << getStateName(0) << "\" : \"" << getPHMMState(0)->eSeq1() << "," << getPHMMState(0)->eSeq2() << "\"";
    for(int i = 1; i < nstates; i++)
      out << ";\n\"" << getStateName(i) << "\" : \"" << getPHMMState(i)->eSeq1() << "," << getPHMMState(i)->eSeq2() << "\"";
    out << ";)" << endl;
    return out.str();
  }

  void PairHiddenMarkovModel::trainBaumWelch(SequenceList & sample1, SequenceList & sample2, int maxiterations, double diff_threshold)
  {
    int nstates = _states.size();
    int alphabet_size = alphabet()->size();

    double diff = HUGE;
    double last = -HUGE;
    if(maxiterations < 0)
      maxiterations = 500;

    int iterations;
    for(iterations = 0; iterations < maxiterations; iterations++){
      DoubleVector sumA (nstates);
      DoubleVector sumE (nstates);
      double sumP = -HUGE;
      Matrix A (nstates, nstates);
      for(int a = 0; a < nstates; a++){
	for(int b = 0; b < nstates; b++){
	  A(a,b) = -HUGE;
	}
      }
      vector<Matrix> E;
      E.resize(nstates);
      for(int i = 0; i < nstates; i++){
	E[i].resize(alphabet_size,alphabet_size);
	sumA[i] = -HUGE;
	sumE[i] = -HUGE;
	for(int a = 0; a < alphabet_size; a++){
	  for(int b = 0; b < alphabet_size; b++){
	    E[i](a,b) = -HUGE;
	  }
	}
      }
	
      for(int s = 0; s < (int)sample1.size(); s++){
	vector<Matrix> alpha;
	vector<Matrix> beta;
	int length1 = sample1[s].size();
	int length2 = sample2[s].size();
	
	double P = forward(sample1[s], sample2[s], alpha);
	sumP = log_sum(sumP,P);
	backward(sample1[s], sample2[s], beta);
	
	
	for(int k = 0; k < nstates; k++){
	  for(int l = 0; l < nstates; l++){
	    for(int i = 0; i <= length1; i++){
	      for(int j = 0; j <= length2; j++){
		int ne1 = getPHMMState(l)->eSeq1();
		int ne2 = getPHMMState(l)->eSeq2();
		if((i+ne1 > length1) || (j+ne2 > length2))
		  continue;
		if(k == 0) {
		  if(ne1 == 1 && ne2 == 1)
		    E[l](sample1[s][i],sample2[s][j]) = log_sum(E[l](sample1[s][i],sample2[s][j]), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2));
		  if(ne1 == 0 && ne2 == 1)
		    E[l](_gap_id,sample2[s][j]) = log_sum(E[l](_gap_id,sample2[s][j]), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2));
		  if(ne1 == 1 && ne2 == 0)
		    E[l](sample1[s][i],_gap_id) = log_sum(E[l](sample1[s][i],_gap_id), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2));
		}
		
		if(ne1 == 1 && ne2 == 1)
		  A(k,l) = log_sum(A(k,l), alpha[k](i,j) + 
				   getPHMMState(k)->transitions()->log_probability_of(l) + 
				   getPHMMState(l)->emission()->log_probability_of_pair(sample1[s][i],sample2[s][j]) +
				   beta[l](i+ne1,j+ne2));
		if(ne1 == 0 && ne2 == 1)
		  A(k,l) = log_sum(A(k,l), alpha[k](i,j) + 
				   getPHMMState(k)->transitions()->log_probability_of(l) + 
				   getPHMMState(l)->emission()->log_probability_of_pair(_gap_id,sample2[s][j]) +
				   beta[l](i+ne1,j+ne2));
		if(ne1 == 1 && ne2 == 0)
		  A(k,l) = log_sum(A(k,l), alpha[k](i,j) + 
				   getPHMMState(k)->transitions()->log_probability_of(l) + 
				   getPHMMState(l)->emission()->log_probability_of_pair(sample1[s][i],_gap_id) +
				   beta[l](i+ne1,j+ne2));
		if(ne1 == 0 && ne2 == 0)
		  A(k,l) = log_sum(A(k,l), alpha[k](i,j) + 
				   getPHMMState(k)->transitions()->log_probability_of(l) + 
				   beta[l](i+ne1,j+ne2));
	      }
	    }
	    A(k,l) -= P;
	    sumA[k] = log_sum(sumA[k], A(k,l));
	  }
	}
	for(int k = 0; k < nstates; k++){
	  for(int a = 0; a < alphabet_size; a++){
	    for(int b = 0; b < alphabet_size; b++){
	      E[k](a,b) -= P;
	      sumE[k] = log_sum(sumE[k], E[k](a,b));
	    }
	  }
	}
      }
      
      for(int k = 0; k < nstates; k++){
	if(getPHMMState(k)->eSeq1() == 0 && getPHMMState(k)->eSeq2() == 0)
	  continue;
	for(int a = 0; a < alphabet_size; a++){
	  for(int b = 0; b < alphabet_size; b++){
	    E[k](a,b) -= sumE[k];
	    getPHMMState(k)->emission()->log_probability_of_pair(a,b,E[k](a,b));
	  }
	}
      }
      for(int k = 0; k < nstates; k++){
	if(getPHMMState(k)->oTransitions().size() == 0)
	  continue;
	for(int l = 0; l < nstates; l++){
	  A(k,l) -= sumA[k];
	  getPHMMState(k)->transitions()->log_probability_of(l, A(k,l));
	}
      }
      diff = fabs(last - sumP);
      if(diff < diff_threshold)	    
	break;
      last = sumP;
    }
    cout << "Stoped at iteration: " << iterations << endl;
    cout << "Diff: " << diff << endl;
  }

  void PairHiddenMarkovModel::generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path){
    Sequence s1, s2, p;
    int state_id = _begin_id;
    int obs1 = _gap_id;
    int obs2 = _gap_id;
    p.push_back(state_id);
    s1.push_back(obs1);
    s2.push_back(obs2);
    
    while(state_id != _end_id){
      state_id = getPHMMState(state_id)->transitions()->choose();
      if(getPHMMState(state_id)->eSeq1() == 0 && getPHMMState(state_id)->eSeq2() == 0){
	p.push_back(state_id);
	s1.push_back(_gap_id);
	s2.push_back(_gap_id);
	continue;
      }
      getPHMMState(state_id)->emission()->choosePair(&obs1,&obs2);
      p.push_back(state_id);
      s1.push_back(obs1);
      s2.push_back(obs2);
    }
    seq1 = s1;
    seq2 = s2;
    path = p;
  }
      
  void PairHiddenMarkovModel::posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, vector<Matrix> & probabilities)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> p;
    p.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (p[i]).resize(length1+1,length2+1);
    }
    vector<Matrix> alpha; // forward
    vector<Matrix> beta;  // backward
    
    double full = forward(seq1, seq2, alpha);
    backward(seq1, seq2, beta);
    
    for(int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
	for(int k = 0; k < nstates; k++){
	  p[k](i,j) = alpha[k](i,j) + beta[k](i,j) - full;
	}
      }
    }
        
    probabilities = p;
  }

  void PairHiddenMarkovModel::silentStatesSort(vector<PHMMStatePtr> silStates){
    int swap = 1;
    while(swap == 1){
      swap = 0;
      for(int i = 0; i < silStates.size() - 1; i++){
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
  }
    
  double PairHiddenMarkovModel::viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & statePath, Sequence & alignment1, Sequence & alignment2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int nsilentstates = _silent_states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    Sequence al1;
    Sequence al2;
    Sequence path;
    vector<IntMatrix> pathMatrix;
    alpha.resize(nstates);
    pathMatrix.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
      (pathMatrix[i]).resize(length1+1,length2+1);
    }
   
    //Initialization
    for(int k = 0; k < nstates; k++){
      if(k == (int)_begin_id){
	alpha[k](0,0) = 0.0;
	continue;
      }
      else
	alpha[k](0,0) = -HUGE;
    }      

    //Recursion
    for (int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
	for(int k = 0; k < nstates; k++){
	  if(i == 0 && j == 0)
	    continue;
	  if(k == (int)_begin_id){
	    alpha[k](i,j) = -HUGE;
	    if(i > 0 && j > 0)
	      pathMatrix[k](i,j) = -1;
	    continue;
	  }
	  int ne1 = getPHMMState(k)->eSeq1();
	  int ne2 = getPHMMState(k)->eSeq2();
	  if(ne1 == 0 && ne2 == 0)
	    continue;

	  alpha[k](i,j) = -HUGE;
	  if(i - ne1 < 0 || j - ne2 < 0){
	    pathMatrix[k](i,j) = -1;
	    continue;
	  }

	  double aux = -HUGE;
	  for(int l = 0; l < (getPHMMState(k)->iTransitions()).size(); l++){
	    int id = getPHMMState(k)->getITransId(l);
	    aux = alpha[id](i-ne1,j-ne2) + getPHMMState(id)->transitions()->log_probability_of(k);
	    if(aux > alpha[k](i,j)){
	      alpha[k](i,j) = aux;
	      pathMatrix[k](i,j) = id;
	    }
	  }
	  if(ne1 > 0 && ne2 > 0)
	    alpha[k](i,j) += getPHMMState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]); 
	  else if(ne1 > 0 && ne2 == 0)
	    alpha[k](i,j) += getPHMMState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
	  else
	    alpha[k](i,j) += getPHMMState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]); 
	}
	for(int k = 0; k < nsilentstates; k++){
	  if(i == 0 && j == 0)
	    continue;
	  int id1 = getSilentId(k);
	  alpha[id1](i,j) = -HUGE;
	  if(id1 == (int)_begin_id)
	    continue;
	  for(int l = 0; l < (getPHMMState(id1)->iTransitions()).size(); l++){
	    int id2 = getPHMMState(id1)->getITransId(l);
	    double aux = alpha[id2](i,j) + getPHMMState(id2)->transitions()->log_probability_of(id1);
	    if(aux > alpha[id1](i,j)){
	      alpha[id1](i,j) = aux; 
	      pathMatrix[id1](i,j) = id2;
	    }
	  }
	}
      }
    }

    //Termination
    double score = -HUGE;
    int p;
    for(int i = 0; i < nstates; i++){
      alpha[i](length1,length2) += getPHMMState(i)->transitions()->log_probability_of(_end_id); 
      if(score < alpha[i](length1,length2)){
	score = alpha[i](length1,length2);
	p = i;
      }
    }
    a = alpha;

    //Trace back
    path.push_back(_end_id);
    al1.push_back(_gap_id);
    al2.push_back(_gap_id);
    while(length1 >= 0 || length2 >= 0){
      path.push_back(p);
      if(getPHMMState(p)->eSeq1() == 0 && getPHMMState(p)->eSeq2() == 0){
	al1.push_back(_gap_id);
	al2.push_back(_gap_id);
	if(length1 == 0 && length2 == 0)
	  break;
	p = pathMatrix[p](length1,length2);
      }
      else if(getPHMMState(p)->eSeq1() > 0 && getPHMMState(p)->eSeq2() == 0){
	al1.push_back(seq1[length1-1]);
	al2.push_back(_gap_id);
	p = pathMatrix[p](length1,length2);
	length1--;
      }
      else if(getPHMMState(p)->eSeq1() == 0 && getPHMMState(p)->eSeq2() > 0){
	al1.push_back(_gap_id);
	al2.push_back(seq2[length2-1]);
	p = pathMatrix[p](length1,length2);
	length2--;
      }
      else{
	al1.push_back(seq1[length1-1]);
	al2.push_back(seq2[length2-1]);
	p = pathMatrix[p](length1,length2);
	length1--;
	length2--;
      }
    }
    //path.push_back(p);
    alignment1.resize(al1.size());
    alignment2.resize(al2.size());
    statePath.resize(path.size());
    for(unsigned int i = 0; i < al1.size(); i++){
      alignment1[i] = al1[al1.size()-1-i];
      alignment2[i] = al2[al2.size()-1-i];
    }
    for(unsigned int i = 0; i < path.size(); i++)
      statePath[i] = path[path.size() - i - 1];

    return score;
  }

  double PairHiddenMarkovModel::forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int nsilentstates = _silent_states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    alpha.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
    }

    //Initialization
    for(int k = 0; k < nstates; k++){
      if(k == _begin_id){
	alpha[k](0,0) = 0.0;
	continue;
      }
      else
	alpha[k](0,0) = -HUGE;
    }

    //Recursion
    for (int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
	for(int k = 0; k < nstates; k++){
	  if(i == 0 && j == 0)
	    continue;
	  if(k == _begin_id){
	    alpha[k](i,j) = -HUGE;
	    continue;
	  }
	  int ne1 = getPHMMState(k)->eSeq1();
	  int ne2 = getPHMMState(k)->eSeq2();
	  if(ne1 == 0 && ne2 == 0)
	    continue;
	  if(i - ne1 < 0 || j - ne2 < 0){
	    alpha[k](i,j) = -HUGE;	  
	    continue;
	  }

	  double aux = -HUGE;
	  for(int l = 0; l < (getPHMMState(k)->iTransitions()).size(); l++){
	    int id = getPHMMState(k)->getITransId(l);
	    aux = log_sum(aux, alpha[id](i-ne1,j-ne2) + getPHMMState(id)->transitions()->log_probability_of(k));
	  }
	  if(ne1 > 0 && ne2 > 0)
	    alpha[k](i,j) = aux + getPHMMState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]); 
	  else if(ne1 > 0 && ne2 == 0)
	    alpha[k](i,j) = aux + getPHMMState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
	  else
	    alpha[k](i,j) = aux + getPHMMState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]); 
	}

	for(int k = 0; k < nsilentstates; k++){
	  if(i == 0 && j == 0)
	    continue;
	  int id1 = getSilentId(k);
	  alpha[id1](i,j) = -HUGE;
	  if(id1 == _begin_id)
	    continue;
	  for(int l = 0; l < (getPHMMState(id1)->iTransitions()).size(); l++){
	    int id2 = getPHMMState(id1)->getITransId(l);
	    alpha[id1](i,j) = log_sum(alpha[id1](i,j), alpha[id2](i,j) + getPHMMState(id2)->transitions()->log_probability_of(id1));
	  }
	}
      }
    }

    //Termination
    double sum = -HUGE;
    for(int i = 0; i < nstates; i++){
      alpha[i](length1,length2) += getPHMMState(i)->transitions()->log_probability_of(_end_id);
      sum = log_sum(sum, alpha[i](length1,length2));
    }
    a = alpha;
    return sum;
  }

  double PairHiddenMarkovModel::backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int nsilentstates = _silent_states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    alpha.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
    }

    //Initialization
    alpha[_end_id](length1,length2) = 0.0;
    for(int k = 0; k < nstates; k++){
      if(k != _begin_id)
	alpha[k](0,0) = -HUGE;
    }

    //Recursion
    for (int i = length1; i >= 0; i--){
      for(int j = length2; j >= 0; j--){
	
	//Silent states elements must be calculated first
	for(int k = nsilentstates-1; k >= 0; k--){
	  int id1 = getSilentId(k);
	  if(id1 == _end_id && i == length1 && j == length2)
	    continue;
	  if(id1 != _begin_id && i == 0 && j == 0)
	    continue;
	  alpha[id1](i,j) = -HUGE;
	  for(int l = 0; l < (getPHMMState(id1)->oTransitions()).size(); l++){
	    int id2 = getPHMMState(id1)->getOTransId(l);
	    int ne1 = getPHMMState(id2)->eSeq1();
	    int ne2 = getPHMMState(id2)->eSeq2();
	    if(i + ne1 > length1 || j + ne2 > length2)
	      continue;

	    if(ne1 > 0 && ne2 > 0){
	      alpha[id1](i,j) = log_sum(alpha[id1](i,j), 
					alpha[id2](i+ne1,j+ne2) + 
					getPHMMState(id1)->transitions()->log_probability_of(id2) + 
					getPHMMState(id2)->emission()->log_probability_of_pair(seq1[i-1+ne1],seq2[j-1+ne2])); 
	    }
	    else if(ne1 > 0 && ne2 == 0){
	      alpha[id1](i,j) = log_sum(alpha[id1](i,j), 
					alpha[id2](i+ne1,j+ne2) + 
					getPHMMState(id1)->transitions()->log_probability_of(id2) + 
					getPHMMState(id2)->emission()->log_probability_of_pair(seq1[i-1+ne1],_gap_id)); 
	    }
	    else if(ne1 == 0 && ne2 >0){
	      alpha[id1](i,j) = log_sum(alpha[id1](i,j), 
					alpha[id2](i+ne1,j+ne2) + 
					getPHMMState(id1)->transitions()->log_probability_of(id2) + 
					getPHMMState(id2)->emission()->log_probability_of_pair(_gap_id,seq2[j-1+ne2]));
	    }
	    else{
	      alpha[id1](i,j) = log_sum(alpha[id1](i,j), 
					alpha[id2](i+ne1,j+ne2) + 
					getPHMMState(id1)->transitions()->log_probability_of(id2)); 
	    }
	  }
	}
	
	//Non-silent states loop
	for(int k = 0; k < nstates; k++){
	  if(k == _end_id && i == length1 && j == length2)
	    continue;
	  if(getPHMMState(k)->eSeq1() == 0 && getPHMMState(k)->eSeq2() == 0)
	    continue;
	  if(k != _begin_id && i == 0 && j == 0)
	    continue;
	  
	  alpha[k](i,j) = -HUGE;
	  for(int l = 0; l < (getPHMMState(k)->oTransitions()).size(); l++){
	    int id = getPHMMState(k)->getOTransId(l);
	    int ne1 = getPHMMState(id)->eSeq1();
	    int ne2 = getPHMMState(id)->eSeq2();
	    if(i + ne1 > length1 || j + ne2 > length2)
	      continue;

	    if(ne1 > 0 && ne2 > 0)
	      alpha[k](i,j) = log_sum(alpha[k](i,j), 
				      alpha[id](i+ne1,j+ne2) + 
				      getPHMMState(k)->transitions()->log_probability_of(id) + 
				      getPHMMState(id)->emission()->log_probability_of_pair(seq1[i-1+ne1],seq2[j-1+ne2])); 
	    else if(ne1 > 0 && ne2 == 0)
	      alpha[k](i,j) = log_sum(alpha[k](i,j), 
				      alpha[id](i+ne1,j+ne2) + 
				      getPHMMState(k)->transitions()->log_probability_of(id) + 
				      getPHMMState(id)->emission()->log_probability_of_pair(seq1[i-1+ne1],_gap_id)); 
	    else if(ne1 == 0 && ne2 >0)
	      alpha[k](i,j) = log_sum(alpha[k](i,j), 
				      alpha[id](i+ne1,j+ne2) + 
				      getPHMMState(k)->transitions()->log_probability_of(id) + 
				      getPHMMState(id)->emission()->log_probability_of_pair(_gap_id,seq2[j-1+ne2]));
	    else
	      alpha[k](i,j) = log_sum(alpha[k](i,j), 
				      alpha[id](i+ne1,j+ne2) + 
				      getPHMMState(k)->transitions()->log_probability_of(id)); 
	  }
	}
      }
    }

    //Termination
    a = alpha;
    return alpha[_begin_id](0,0);
  }

  void PairHiddenMarkovModel::initialize(const ProbabilisticModelParameters & parameters) {
    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr number_of_emissions = parameters.getMandatoryParameterValue("number_of_emissions");
    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");

    int end = 0;
    int begin = 0;
    std::vector<PHMMStatePtr> state_list;
    std::vector<PHMMStatePtr> silent_state_list;

    AlphabetPtr states = AlphabetPtr(new Alphabet());
    states->initializeFromVector(state_names->getStringVector());

    AlphabetPtr observations = AlphabetPtr(new Alphabet());
    observations->initializeFromVector(observation_symbols->getStringVector());
    if(observations->has("_"))
      _gap_id = observations->getSymbol("_")->id();
    else{
      observations->createSymbol("_");
      _gap_id = observations->getSymbol("_")->id();
    }

    //Initializing the emission probabilities
    std::map<std::string,double> emisspar = emissions->getDoubleMap();
    std::map<std::string,double>::const_iterator it;    
    std::map<std::string,Matrix> emiss;
    std::map<std::string,Matrix>::iterator eit;
    for(it = emisspar.begin(); it != emisspar.end(); it++){
      std::vector<std::string> splited;
      boost::regex separator("\\|");
      split_regex(it->first, splited, separator);
      if(splited.size() == 1)
	splited.push_back("");
      std::string symbol(splited[0]);
      std::string state(splited[1]);
      if(symbol.length() < 2){
	cerr << "Emission " << symbol << " is not a pair of observation symbols." << endl;
	exit(-1);
      }
      eit = emiss.find(state);
       if(eit == emiss.end()){
	int id1 = observations->getSymbol(symbol.substr(0,1))->id();
	int id2 = observations->getSymbol(symbol.substr(1,1))->id();
	emiss[state] = Matrix (observations->size(),observations->size());
	for(int i = 0; i < observations->size(); i++){
	  for(int j = 0; j < observations->size(); j++){
	    emiss[state](i,j) = 0.0;
	  }
	}
	if((id1 >= 0) && (id1 < (int)(emiss[state]).size1()) && (id2 >= 0) && (id2 < (int)(emiss[state]).size2()))
	  (emiss[state])(id1,id2) = it->second;
      }
      else{
	int id1 = observations->getSymbol(symbol.substr(0,1))->id();
	int id2 = observations->getSymbol(symbol.substr(1,1))->id();
	if((id1 >= 0) && (id1 < (int)(eit->second).size1()) && (id2 >= 0) && (id2 < (int)(eit->second).size2()))
	  emiss[state](id1,id2) = it->second;
      }
    }    

    //Initializing the transition probabilities
    std::map<std::string,double> transpar = transitions->getDoubleMap();
    std::map<std::string,DoubleVector> trans;
    std::map<std::string,DoubleVector>::const_iterator it2;
    std::map<std::string,IntVector> inTrans;
    std::map<std::string,IntVector> outTrans;
    for(it = transpar.begin(); it != transpar.end(); it++){
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
	  IntVector outgoing;
	  outgoing.push_back(id);
	  outTrans[from] = outgoing;
	  probs.resize(states->size());
	  trans[from]=probs;
	  if(id < (int)trans[from].size())
	    trans[from][id] = it->second;
	}
      else
	{
	  int id = states->getSymbol(to)->id();
	  outTrans[from].push_back(id);
	  if(id < (int)trans[from].size())
	    trans[from][id] = it->second;
	}
      if(inTrans.find(to) == inTrans.end()){
	int id = states->getSymbol(from)->id();
	IntVector incoming;
	incoming.push_back(id);
	inTrans[to] = incoming;
      }
      else{
	int id = states->getSymbol(from)->id();
	inTrans[to].push_back(id);
      }	
    }
    
    //Configuring the PHMM states
    std::map<std::string,std::string> numEmiss = number_of_emissions->getStringMap();
    std::map<std::string,std::string>::const_iterator numEmissIt;
    std::map<std::string,Matrix>::const_iterator it3;
    for(unsigned int i = 0; i < states->size(); i++){
      SymbolPtr state_name = states->getSymbol(i);
      MultinomialDistributionPtr e ;
      MultinomialDistributionPtr t ;
      IntVector itr;
      IntVector otr;
      int numEmiss1, numEmiss2;
      std::map<std::string,IntVector>::const_iterator inTransIt;

      numEmissIt = numEmiss.find(state_name->name());
      if(numEmissIt != numEmiss.end()){
	std::vector<std::string> splited;
	boost::regex separator(",");
	split_regex(numEmissIt->second, splited, separator);
	numEmiss1 = atoi(splited[0].c_str());
	numEmiss2 = atoi(splited[1].c_str());
      }

      it3 = emiss.find(state_name->name());
      if(it3 != emiss.end())
	e = MultinomialDistributionPtr(new MultinomialDistribution(it3->second));

      if(it3 == emiss.end() && numEmiss1 == 0 && numEmiss2 == 0)
	e = MultinomialDistributionPtr(new MultinomialDistribution());
                  
      it2 = trans.find(state_name->name());
      if(it2 != trans.end())
	t = MultinomialDistributionPtr(new MultinomialDistribution(it2->second));
      
      else{
	t = MultinomialDistributionPtr(new MultinomialDistribution());
      }
      
      inTransIt = inTrans.find(state_name->name());
      if(inTransIt != inTrans.end())
	itr = inTransIt->second;
      else{
	begin++;
	_begin_id = state_list.size();
	IntVector incoming;
	itr = incoming;
      }

      inTransIt = outTrans.find(state_name->name());
      if(inTransIt != outTrans.end())
	otr = inTransIt->second;
      else{
	end++;
	_end_id = state_list.size();
	IntVector outgoing;
	otr = outgoing;
      }
      
      PHMMStatePtr statePtr = PHMMStatePtr(new PHMMState(state_list.size(), state_name, e, t, itr, otr, numEmiss1, numEmiss2));
      if(numEmiss1 == 0 && numEmiss2 == 0)
	silent_state_list.push_back(statePtr);
      state_list.push_back(statePtr);
    }
    if(end != 1){
      cerr << "An end state must be defined." << endl;
      exit(-1);
    }
    if(begin != 1){
      cerr << "A begin state must be defined." << endl;
      exit(-1);
    }
    setStates(state_list, states);
    if(silent_state_list.size() > 1)
      silentStatesSort(silent_state_list);
    setSilentStates(silent_state_list);
    setObservationSymbols(observations);
  }
}


