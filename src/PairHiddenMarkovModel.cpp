/*
 *       PairHiddenMarkovModel.cpp
 *
 *      Copyright 2011  Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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

#include "PairHiddenMarkovModel.hpp"

namespace tops {

  /////////////////////////////////////////////////////////
  //////// Decoding algorithms ////////////////////////////
  /////////////////////////////////////////////////////////

  float PairHiddenMarkovModel::posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, SparseMatrixPtr &sparsePPMatch,SparseMatrixPtr &sparsePPGap1, SparseMatrixPtr &sparsePPGap2)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    fMatrix ppMatch(length1+1,length2+1);
    fMatrix ppGap1(length1+1,length2+1);
    fMatrix ppGap2(length1+1,length2+1);
    vector<Matrix> alpha; // forward
    vector<Matrix> beta;  // backward

    double full = forward(seq1, seq2, alpha);
    backward(seq1, seq2, beta);
    for(int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
  ppMatch(i,j) = 0;
  ppGap1(i,j) = 0;
  ppGap2(i,j) = 0;
  for(int k = 0; k < nstates; k++)
    _states[k]->postProbSum(ppMatch,ppGap1,ppGap2,alpha[k],beta[k],full,i,j);
      }
    }
    float ea = expectedAccuracy(length1+1,length2+1,ppMatch);
    sparsePPMatch = SparseMatrixPtr(new SparseMatrix(length1+1,length2+1,ppMatch));
    sparsePPGap1 = SparseMatrixPtr(new SparseMatrix(length1+1,length2+1,ppGap1));
    sparsePPGap2 = SparseMatrixPtr(new SparseMatrix(length1+1,length2+1,ppGap2));

    return ea;
  }

  void MatchState::postProbSum(fMatrix &ppMatch, fMatrix &ppGap1, fMatrix &ppGap2, Matrix &alpha, Matrix &beta, double full, int i, int j){
    ppMatch(i,j) += exp(alpha(i,j) + beta(i,j) - full);
  }
  void Gap1State::postProbSum(fMatrix &ppMatch, fMatrix &ppGap1, fMatrix &ppGap2, Matrix &alpha, Matrix &beta, double full, int i, int j){
    ppGap1(i,j) += exp(alpha(i,j) + beta(i,j) - full);
  }
  void Gap2State::postProbSum(fMatrix &ppMatch, fMatrix &ppGap1, fMatrix &ppGap2, Matrix &alpha, Matrix &beta, double full, int i, int j){
    ppGap2(i,j) += exp(alpha(i,j) + beta(i,j) - full);
  }

  /*float PairHiddenMarkovModel::expectedAccuracyWithGaps(SparseMatrixPtr postProbs, SparseMatrixPtr gap1Probs, SparseMatrixPtr gap2Probs){
    int size1 = postProbs->nrows();
    int size2 = postProbs->ncols();
    SparseMatrixPtr ea = SparseMatrixPtr(new SparseMatrix(size1,size2));
    SparseMatrixPtr ptrs = SparseMatrixPtr(new SparseMatrix(size1,size2));
    for(int i = 0; i < size1; i++){
      for(int j = 0; j < size2; j++){
  if(i == 0 && j == 0){
    continue;
  }
  if(i == 0){
    ea->add(i,j,ea->get(i,j-1)+gap1Probs->get(i,j));
    ptrs->add(i,j,2);
    continue;
  }
  if(j == 0){
    ea->add(i,j,ea->get(i-1,j)+gap2Probs->get(i,j));
    ptrs->add(i,j,1);
    continue;
  }
  ea->add(i,j,ea->get(i-1,j)+gap2Probs->get(i,j));
  ptrs->add(i,j,1);
  if(ea->get(i,j) <= ea->get(i,j-1)+gap1Probs->get(i,j)){
    ea->add(i,j,ea->get(i,j-1)+gap1Probs->get(i,j));
    ptrs->add(i,j,2);
  }
  if(ea->get(i,j) <= ea->get(i-1,j-1)+postProbs->get(i,j)){
    ea->add(i,j,ea->get(i-1,j-1)+postProbs->get(i,j));
    ptrs->add(i,j,3);
  }
      }
    }
    int i = size1-1;
    int j = size2-1;
    int allen = 0;
    while(i > 0 || j > 0){
      allen++;
      float ptr = ptrs->get(i,j);
      if(ptr == 1)
  i--;
      if(ptr == 2)
  j--;
      if(ptr == 3){
  i--;
  j--;
      }
    }
    float mea = ea->get(size1-1,size2-1)/allen;
    return mea;
    }*/

  float PairHiddenMarkovModel::expectedAccuracy(int size1, int size2, fMatrix &postProbs){
    int allen = size1;
    if(size2 < allen)
      allen = size2;
    fMatrix ea(size1,size2);

    for(int i = 0; i < size1; i++)
      ea(i,0) = 0.0;
    for(int j = 1; j < size2; j++)
      ea(0,j) = 0.0;

    for(int i = 1; i < size1; i++){
      for(int j = 1; j < size2; j++){
  ea(i,j) = ea(i-1,j);
  if(ea(i,j) <= ea(i,j-1)){
    ea(i,j) = ea(i,j-1);
  }
  if(ea(i,j) <= ea(i-1,j-1)+postProbs(i,j)){
    ea(i,j) = ea(i-1,j-1)+postProbs(i,j);
  }
      }
    }
    return ea(size1-1,size2-1)/allen;;
  }

  /*  double PairHiddenMarkovModel::viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & statePath, Sequence & alignment1, Sequence & alignment2, vector<Matrix> &a)
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
          int ne1 = _states[k]->eSeq1();
          int ne2 = _states[k]->eSeq2();
          if(ne1 == 0 && ne2 == 0)
            continue;

          alpha[k](i,j) = -HUGE;
          if(i - ne1 < 0 || j - ne2 < 0){
            pathMatrix[k](i,j) = -1;
            continue;
          }

          double aux = -HUGE;
          for(int l = 0; l < (int)(_states[k]->iTransitions()).size(); l++){
            int id = _states[k]->getITransId(l);
            aux = alpha[id](i-ne1,j-ne2) + _states[id]->transitions()->log_probability_of(k);
            if(aux > alpha[k](i,j)){
              alpha[k](i,j) = aux;
              pathMatrix[k](i,j) = id;
            }
          }
          if(ne1 > 0 && ne2 > 0)
            alpha[k](i,j) += _states[k]->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]);
          else if(ne1 > 0 && ne2 == 0)
            alpha[k](i,j) += _states[k]->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
          else
            alpha[k](i,j) += _states[k]->emission()->log_probability_of_pair(_gap_id,seq2[j-1]);
        }
        for(int k = 0; k < nsilentstates; k++){
          if(i == 0 && j == 0)
            continue;
          int id1 = getSilentId(k);
          alpha[id1](i,j) = -HUGE;
          if(id1 == (int)_begin_id)
            continue;
          for(int l = 0; l < (int)(_states[id1]->iTransitions()).size(); l++){
            int id2 = _states[id1]->getITransId(l);
            double aux = alpha[id2](i,j) + _states[id2]->transitions()->log_probability_of(id1);
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
      alpha[i](length1,length2) += _states[i]->transitions()->log_probability_of(_end_id);
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
      if(_states[p]->eSeq1() == 0 && _states[p]->eSeq2() == 0){
        al1.push_back(_gap_id);
        al2.push_back(_gap_id);
        if(length1 == 0 && length2 == 0)
          break;
        p = pathMatrix[p](length1,length2);
      }
      else if(_states[p]->eSeq1() > 0 && _states[p]->eSeq2() == 0){
        al1.push_back(seq1[length1-1]);
        al2.push_back(_gap_id);
        p = pathMatrix[p](length1,length2);
        length1--;
      }
      else if(_states[p]->eSeq1() == 0 && _states[p]->eSeq2() > 0){
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
    }*/

  void MatchState::forwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int begin_id){
    if(i == 1 && j == 1){
      a[_id](i,j) = states[begin_id]->transitions()->log_probability_of(_id) + _emission->log_probability_of_pair(seq1[0],seq2[0]);
      return;
    }

    if(i == 0 || j == 0){
      a[_id](i,j) = -HUGE;
      return;
    }

    vector<int>::iterator predStateNumIt = _incomingTransitions.begin();
    a[_id](i,j) = a[*predStateNumIt](i-1,j-1) + states[*predStateNumIt]->transitions()->log_probability_of(_id);
    for(predStateNumIt = _incomingTransitions.begin()+1; predStateNumIt != _incomingTransitions.end(); predStateNumIt++)
      a[_id](i,j) = log_sum(a[_id](i,j), a[*predStateNumIt](i-1,j-1) + states[*predStateNumIt]->transitions()->log_probability_of(_id));

    a[_id](i,j) += _emission->log_probability_of_pair(seq1[i-1],seq2[j-1]);
  }

  void Gap1State::forwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int begin_id){
    if(i == 0 && j == 1){
      a[_id](i,j) = states[begin_id]->transitions()->log_probability_of(_id) + _emission->log_probability_of_pair(gap_id,seq2[0]);
      return;
    }

    if(j == 0){
      a[_id](i,j) = -HUGE;
      return;
    }

    vector<int>::iterator predStateNumIt = _incomingTransitions.begin();
    a[_id](i,j) = a[*predStateNumIt](i,j-1) + states[*predStateNumIt]->transitions()->log_probability_of(_id);
    for(predStateNumIt = _incomingTransitions.begin()+1; predStateNumIt != _incomingTransitions.end(); predStateNumIt++)
      a[_id](i,j) = log_sum(a[_id](i,j), a[*predStateNumIt](i,j-1) + states[*predStateNumIt]->transitions()->log_probability_of(_id));

    a[_id](i,j) += _emission->log_probability_of_pair(gap_id,seq2[j-1]);
  }

  void Gap2State::forwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int begin_id){
    if(i == 1 && j == 0){
      a[_id](i,j) = states[begin_id]->transitions()->log_probability_of(_id) + _emission->log_probability_of_pair(seq1[0],gap_id);
      return;
    }

    if(i == 0){
      a[_id](i,j) = -HUGE;
      return;
    }

    vector<int>::iterator predStateNumIt = _incomingTransitions.begin();
    a[_id](i,j) = a[*predStateNumIt](i-1,j) + states[*predStateNumIt]->transitions()->log_probability_of(_id);
    for(predStateNumIt = _incomingTransitions.begin()+1; predStateNumIt != _incomingTransitions.end(); predStateNumIt++)
      a[_id](i,j) = log_sum(a[_id](i,j), a[*predStateNumIt](i-1,j) + states[*predStateNumIt]->transitions()->log_probability_of(_id));

    a[_id](i,j) += _emission->log_probability_of_pair(seq1[i-1],gap_id);
  }

  void SilentState::forwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int begin_id){
    return;
  }

  double PairHiddenMarkovModel::forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();

    a.resize(nstates);
    for(int i = 0; i < nstates; i++){
      if(i != _begin_id && i != _end_id)
  (a[i]).resize(length1+1,length2+1);
      else
  (a[i]).resize(0,0);
    }

    //Recursion
    vector<PHMMStatePtr>::iterator currState;
    for (int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
        for(currState = _states.begin(); currState != _states.end(); currState++){
    (*currState)->forwardSum(_states,seq1,seq2,a,i,j,_gap_id,_begin_id);
  }
      }
    }

    //Termination
    double sum = -HUGE;
    for(currState = _states.begin(); currState != _states.end(); currState++){
      if(!(*currState)->isSilent())
  sum = log_sum(sum, a[(*currState)->getId()](length1,length2) + (*currState)->transitions()->log_probability_of(_end_id));
    }
    return sum;
  }

  void MatchState::backwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int currStateId, double *accumulator){
    if(i == (int)seq1.size() || j == (int)seq2.size())
      return;
    (*accumulator) = log_sum((*accumulator),
           a[_id](i+1,j+1) +
           states[currStateId]->transitions()->log_probability_of(_id) +
           _emission->log_probability_of_pair(seq1[i],seq2[j]));
  }

  void Gap1State::backwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int currStateId, double *accumulator){
    if(j == (int)seq2.size())
      return;
    (*accumulator) = log_sum((*accumulator),
           a[_id](i,j+1) +
           states[currStateId]->transitions()->log_probability_of(_id) +
           _emission->log_probability_of_pair(gap_id,seq2[j]));
  }

  void Gap2State::backwardSum(vector<PHMMStatePtr> &states, const Sequence &seq1, const Sequence &seq2, vector<Matrix> &a, int i, int j, int gap_id, int currStateId, double *accumulator){
    if(i == (int)seq1.size())
      return;
    (*accumulator) = log_sum((*accumulator),
           a[_id](i+1,j) +
           states[currStateId]->transitions()->log_probability_of(_id) +
           _emission->log_probability_of_pair(seq1[i],gap_id));
  }

  double PairHiddenMarkovModel::backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    a.resize(nstates);
    for(int i = 0; i < nstates; i++){
      if(i == _end_id || i == _begin_id){
  (a[i]).resize(0,0);
  continue;
      }
      (a[i]).resize(length1+1,length2+1);
    }

    //Initialization
    for(int k = 0; k < nstates; k++){
      if(k != _end_id && k != _begin_id)
  a[k](length1,length2) = _states[k]->transitions()->log_probability_of(_end_id);
    }

    //Recursion
    IntVector::iterator nextStateNumIt;
    double accumulator;
    for (int i = length1; i >= 0; i--){
      for(int j = length2; j >= 0; j--){
  if(i == length1 && j == length2)
    continue;
        for(int currStateId = 0; currStateId < nstates; currStateId++){
    if(_states[currStateId]->isSilent())
      continue;
    accumulator = -HUGE;
    for(nextStateNumIt = _states[currStateId]->outTransitions().begin(); nextStateNumIt != _states[currStateId]->outTransitions().end(); nextStateNumIt++)
      _states[*nextStateNumIt]->backwardSum(_states,seq1,seq2,a,i,j,_gap_id,currStateId,&accumulator);
    a[currStateId](i,j) = accumulator;
        }
      }
    }

    //Termination
    accumulator = -HUGE;
    for(nextStateNumIt = _states[_begin_id]->outTransitions().begin(); nextStateNumIt != _states[_begin_id]->outTransitions().end(); nextStateNumIt++)
      _states[*nextStateNumIt]->backwardSum(_states,seq1,seq2,a,0,0,_gap_id,_begin_id,&accumulator);
    return accumulator;
  }

  ////////////////////////////////////////////////
  ////// Model training///////////////////////////
  ////////////////////////////////////////////////

  /*  void PairHiddenMarkovModel::trainBaumWelch(SequenceList & sample, int maxiterations, double diff_threshold)
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
    if(!close(exp(_states[a]->transitions()->log_probability_of(b)), 0.0, 1e-10))
      A(a,b) = _states[a]->transitions()->log_probability_of(b) + log(3);
    else
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
      if(!close(exp(_states[i]->emission()->log_probability_of_pair(a,b)), 0.0, 1e-10)){
        if(_states[i]->eSeq1() > 0 && _states[i]->eSeq2() > 0)
    E[i](a,b) = _states[i]->emission()->log_probability_of_pair(a,b) + log(210);
        else
    E[i](a,b) = _states[i]->emission()->log_probability_of_pair(a,b) + log(19);
      }
      else
        E[i](a,b) = -HUGE;
          }
  }
      }
      for(int s1 = 0; s1 < (int)sample.size(); s1++){
  for(int s2 = s1+1; s2 < (int)sample.size(); s2++){
    vector<Matrix> alpha;
    vector<Matrix> beta;
    int length1 = sample[s1].size();
    int length2 = sample[s2].size();

    double P = forward(sample[s1], sample[s2], alpha);
    sumP = log_sum(sumP,P);
    backward(sample[s1], sample[s2], beta);


    for(int k = 0; k < nstates; k++){
      for(int l = 0; l < nstates; l++){
        if(l == _begin_id || k == _end_id){
    continue;
        }
        if(k == _begin_id && l != _end_id){
    A(k,l) = log_sum(A(k,l), _states[k]->transitions()->log_probability_of(l) + beta[l](0,0) - P);
    continue;
        }
        if(l == _end_id && k != _begin_id){
    A(k,l) = log_sum(A(k,l), alpha[k](length1,length2) + _states[k]->transitions()->log_probability_of(l)-P);
    continue;
        }
        if(k == _begin_id && l == _end_id){
    continue;
        }
        for(int i = 0; i <= length1; i++){
    for(int j = 0; j <= length2; j++){
      int ne1 = getPHMMState(l)->eSeq1();
      int ne2 = getPHMMState(l)->eSeq2();
      if((i+ne1 > length1) || (j+ne2 > length2))
        continue;
      if(k == 0) {
        if(ne1 == 1 && ne2 == 1)
          E[l](sample[s1][i],sample[s2][j]) = log_sum(E[l](sample[s1][i],sample[s2][j]), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2) - P);
        if(ne1 == 0 && ne2 == 1)
          E[l](_gap_id,sample[s2][j]) = log_sum(E[l](_gap_id,sample[s2][j]), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2) - P);
        if(ne1 == 1 && ne2 == 0)
          E[l](sample[s1][i],_gap_id) = log_sum(E[l](sample[s1][i],_gap_id), alpha[l](i+ne1,j+ne2) + beta[l](i+ne1,j+ne2) - P);
      }

      if(ne1 == 1 && ne2 == 1)
        A(k,l) = log_sum(A(k,l), alpha[k](i,j) +
             _states[k]->transitions()->log_probability_of(l) +
             getPHMMState(l)->emission()->log_probability_of_pair(sample[s1][i],sample[s2][j]) +
             beta[l](i+ne1,j+ne2)-P);
      if(ne1 == 0 && ne2 == 1)
        A(k,l) = log_sum(A(k,l), alpha[k](i,j) +
             _states[k]->transitions()->log_probability_of(l) +
             getPHMMState(l)->emission()->log_probability_of_pair(_gap_id,sample[s2][j]) +
             beta[l](i+ne1,j+ne2)-P);
      if(ne1 == 1 && ne2 == 0)
        A(k,l) = log_sum(A(k,l), alpha[k](i,j) +
             _states[k]->transitions()->log_probability_of(l) +
             getPHMMState(l)->emission()->log_probability_of_pair(sample[s1][i],_gap_id) +
             beta[l](i+ne1,j+ne2)-P);
      if(ne1 == 0 && ne2 == 0)
        A(k,l) = log_sum(A(k,l), alpha[k](i,j) +
             _states[k]->transitions()->log_probability_of(l) +
             beta[l](i+ne1,j+ne2)-P);
    }
        }
      }
    }
  }
      }
      for(int k = 0; k < nstates; k++){
  for(int l = 0; l < nstates; l++){
    sumA[k] = log_sum(sumA[k], A(k,l));
  }
      }
      for(int k = 0; k < nstates; k++){
  for(int a = 0; a < alphabet_size; a++){
    for(int b = 0; b < alphabet_size; b++){
      double s = log_sum(E[k](a,b), E[k](b,a));
      E[k](a,b) = E[k](b,a) = s-log(2);
      sumE[k] = log_sum(sumE[k], E[k](a,b));
    }
  }
      }
      for(int k = 0; k < nstates; k++){
  if(_states[k]->eSeq1() == 0 && _states[k]->eSeq2() == 0)
    continue;
  for(int a = 0; a < alphabet_size; a++){
    for(int b = 0; b < alphabet_size; b++){
      E[k](a,b) -= sumE[k];
      _states[k]->emission()->log_probability_of_pair(a,b,E[k](a,b));
    }
  }
      }
      for(int k = 0; k < nstates; k++){
  if(_states[k]->oTransitions().size() == 0)
    continue;
  for(int l = 0; l < nstates; l++){
    A(k,l) -= sumA[k];
    _states[k]->transitions()->log_probability_of(l, A(k,l));
  }
      }
      //cerr << "sumP = " << sumP << endl;
      diff = fabs(last - sumP);
      if(diff < diff_threshold)
  break;
      last = sumP;
    }
    //cerr << "Stoped at iteration: " << iterations << endl;
    //cerr << "Diff: " << diff << endl;
    }*/

  ////////////////////////////////////////////////
  ////// Model Initialization ////////////////////
  ////////////////////////////////////////////////

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
    if(observations->has("-"))
      _gap_id = observations->getSymbol("-")->id();
    else{
      observations->createSymbol("-");
      _gap_id = observations->getSymbol("-")->id();
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
        for(int i = 0; i < (int)observations->size(); i++){
            for(int j = 0; j < (int)observations->size(); j++){
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
      DiscreteIIDModelPtr e ;
      DiscreteIIDModelPtr t ;
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
        e = DiscreteIIDModelPtr(new DiscreteIIDModel(it3->second));

      if(it3 == emiss.end() && numEmiss1 == 0 && numEmiss2 == 0)
        e = DiscreteIIDModelPtr(new DiscreteIIDModel());

      it2 = trans.find(state_name->name());
      if(it2 != trans.end())
        t = DiscreteIIDModelPtr(new DiscreteIIDModel(it2->second));

      else{
        t = DiscreteIIDModelPtr(new DiscreteIIDModel());
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

      PHMMStatePtr statePtr;
      if(numEmiss1 == 0 && numEmiss2 == 0){
  statePtr = PHMMStatePtr(new SilentState(state_list.size(), state_name, e, t, itr, otr));
        silent_state_list.push_back(statePtr);
      }
      if(numEmiss1 == 1 && numEmiss2 == 1){
  statePtr = PHMMStatePtr(new MatchState(state_list.size(), state_name, e, t, itr, otr));
      }
      if(numEmiss1 == 0 && numEmiss2 == 1){
  statePtr = PHMMStatePtr(new Gap1State(state_list.size(), state_name, e, t, itr, otr));
      }
      if(numEmiss1 == 1 && numEmiss2 == 0){
  statePtr = PHMMStatePtr(new Gap2State(state_list.size(), state_name, e, t, itr, otr));
      }
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
    //if(silent_state_list.size() > 1)
    //silentStatesSort(silent_state_list);
    setSilentStates(silent_state_list);
    setObservationSymbols(observations);
  }

  ////////////////////////////////////////////////////
  ///////// Auxiliary and debug functions ////////////
  ////////////////////////////////////////////////////

  /*  void PairHiddenMarkovModel::silentStatesSort(vector<PHMMStatePtr> silStates){
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


  /*  std::string PairHiddenMarkovModel::str () const
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
          if(close(exp(_states[i]->emission()->log_probability_of_pair(k,j)), 0.0, 1e-10))
            continue;
          if(it == 0){
            out << "\"" <<  alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" << getStateName(i) << "\": " << exp(_states[i]->emission()->log_probability_of_pair(k,j)) ;
            it++;
          }
          else
            out << ";\n \"" << alphabet()->getSymbol(k)->name() << alphabet()->getSymbol(j)->name() << "\" | \"" <<  getStateName(i) << "\": " << exp(_states[i]->emission()->log_probability_of_pair(k,j));
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

  /*  void PairHiddenMarkovModel::generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path){
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
      _states[state_id]->emission()->choosePair(&obs1,&obs2);
      p.push_back(state_id);
      s1.push_back(obs1);
      s2.push_back(obs2);
    }
    seq1 = s1;
    seq2 = s2;
    path = p;
    }*/
}


