/*
 *       GeneralizedHiddenMarkovModel.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
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

#include "InhomogeneousFactorableModel.hpp"
#include "GeneralizedHiddenMarkovModel.hpp"
#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"
#include "GHMMStates.hpp"
#include "util.hpp"
#include <sys/types.h>


namespace tops {

  std::string GeneralizedHiddenMarkovModel::print_graph () const {
    std::stringstream out;
    for(int i = 0; i < (int)_all_states.size(); i++)
      {
        out << _all_states[i]->name() << " " << _all_states[i]->name() << std::endl;
      }
    out << "#" << std::endl;
    for(int i = 0; i < (int)_all_states.size(); i++)
      {
        for(int j = 0; j < (int)_all_states.size(); j++)
          {
            if(!close( exp(_all_states[i]->transition()->log_probability_of(j)),0.0, 1-1))
              out << _all_states[i]->name() << " " << _all_states[j]->name() << " " << exp(_all_states[i]->transition()->log_probability_of(j)) <<  std::endl;
          }
      }
    return out.str();
  }
  void GeneralizedHiddenMarkovModel::restore_model(std::string & model_name,const ProbabilisticModelParameters & parameters) {
    ProbabilisticModelParameterValuePtr modelpar =
      parameters.getOptionalParameterValue(model_name);
    if (modelpar == NULL) {
      std::cerr << "ERROR: Missing definition of the model  "
                << model_name << std::endl;
      return;
    }
    if (_models.find(model_name) != _models.end()) {
      return;
    }

    std::string cfg = modelpar->getString();
    ProbabilisticModelCreatorClient creator;
    ConfigurationReader modelreader;
    if ((cfg.size()) > 0 && (cfg[0] == '[')) {
      cfg = cfg.substr(1, cfg.size() - 2);
      if (modelreader.load(cfg)) {
        ProbabilisticModelParameterValuePtr modelpar =  (modelreader.parameters())->getOptionalParameterValue("model");
        if(modelpar != NULL)
          {
            std::string submodelstr = modelpar->getString();
            if ((submodelstr.size()) > 0 && (submodelstr[0] == '[')) {
            } else {
              restore_model(submodelstr, parameters);
            }
          }
        ProbabilisticModelPtr m = creator.create(*(modelreader.parameters()), _models);
        _models[model_name] = m;
      } else{
        std::cerr << "/=======/\n" << cfg << "/========/" << std::endl;
        exit(-1);
      }

    }
     else
        {

          ProbabilisticModelPtr m = creator.create(cfg);
          if (m == NULL) {
            std::cerr << "Can not load model " << cfg << "! " << std::endl;
            return;
          }

          _models[model_name] = m;
        }
  }

  void GeneralizedHiddenMarkovModel::setStateNames(AlphabetPtr alphabet) {
        _state_names = alphabet;
        _all_states.resize(alphabet->size());
  }
  void GeneralizedHiddenMarkovModel::setInitialProbability(
                 DiscreteIIDModelPtr init) {
    _initial_probabilities = init;
  }

  void GeneralizedHiddenMarkovModel::setNumClasses(int nclasses){
    _nclasses = nclasses;
  }

  int GeneralizedHiddenMarkovModel::getNumClasses(){
    return _nclasses;
  }

  void GeneralizedHiddenMarkovModel::setTerminalProbability(
                  DiscreteIIDModelPtr term) {
        _terminal_probabilities = term;
  }

int GeneralizedHiddenMarkovModel::configureSignalState(std::string observation_model_name,
                                                        DiscreteIIDModelPtr transition_distr,
                   int size, std::string state_name, int iphase, int ophase, vector<int> classes){
  SymbolPtr symbol = _state_names->getSymbol(state_name);
  ProbabilisticModelPtr model = _models[observation_model_name];
  GHMMSignalStatePtr signal = GHMMSignalStatePtr(new GHMMSignalState(model,transition_distr, symbol));
  signal->setSize(size);
  signal->observationModelName(observation_model_name);
  signal->setInputPhase(iphase);
  signal->setOutputPhase(ophase);
  signal->setClasses(classes);
 _all_states[symbol->id()] = signal;
  _signal_states.push_back(signal);
  return symbol->id();
}

int GeneralizedHiddenMarkovModel::configureGeometricDurationState(std::string model_name,
                  DiscreteIIDModelPtr transition_distr, std::string state_name, int iphase, int ophase, vector<int> classes) {
  ProbabilisticModelPtr model = _models[model_name];
  SymbolPtr symbol = _state_names->getSymbol(state_name);
  GHMMStatePtr state = GHMMStatePtr(new GHMMState(model, transition_distr,
                                                  symbol));
  state->setInputPhase(iphase);
  state->setOutputPhase(ophase);
  state->setClasses(classes);
  _all_states[symbol->id()] = state;
  state->observationModelName(model_name);
  _geometric_duration_states.push_back(state);
  return symbol->id();
}


int GeneralizedHiddenMarkovModel::configureExplicitDurationState(std::string observation_model_name, DiscreteIIDModelPtr transition_distr,
                 std::string duration_model_name, std::string state_name, int iphase, int ophase, vector<int> classes)
{

  ProbabilisticModelPtr model = _models[observation_model_name];
  ProbabilisticModelPtr duration = _models[duration_model_name];
  SymbolPtr symbol = _state_names->getSymbol(state_name);
  GHMMExplicitDurationStatePtr state = GHMMExplicitDurationStatePtr(
                                                                    new GHMMExplicitDurationState(model, transition_distr, symbol));
  state->observationModelName(observation_model_name);
  state->durationModelName(duration_model_name);
  state->setDuration(duration);
  state->setInputPhase(iphase);
  state->setOutputPhase(ophase);
  state->setClasses(classes);
 _all_states[symbol->id()] = state;
  return symbol->id();
}


    void GeneralizedHiddenMarkovModel::initializeChoosePathAlgorithm(const Sequence &s)
    {
        if(s != _last)
            {
                forward(s, _alpha);
                _last = s;
            }
    }

  void GeneralizedHiddenMarkovModel::choosePath(const Sequence &s, Sequence &path) {
    initializeChoosePathAlgorithm(s);
    int size = s.size();
    int nstates =_all_states.size();
    DoubleVector lastStateProbability(nstates);
    double sum = -HUGE;
    int L = size - 1;
    path.resize(s.size());
#if 0
    for(int i = 0; i < size; i++)
      {
  std::cerr << i << " " << std::endl;
  for (int j = 0; j < nstates; j++)
    {
      std::cerr << " " << _all_states[j]->name() << " " << _alpha(j, i) << std::endl;
    }
      }
#endif
    for(int k = 0; k < nstates; k++)
      {
  sum = log_sum(sum, _alpha(k,L) );
      }
    for(int k = 0; k < nstates; k++)
      {
  lastStateProbability[k] = exp(_alpha(k, L) - sum);
      }
    DiscreteIIDModelPtr m = DiscreteIIDModelPtr(new DiscreteIIDModel(lastStateProbability));

    int q = m->choose();
    int position = L;

    while (position > 0 ){
      int new_position;
      int state;
      new_position = 0;
      state = q;
      _all_states[q]->choosePredecessor(_alpha, position, state, new_position, _all_states);
      for(int p = new_position + 1; p <= position; p++)
  {
    path[p] = q;
  }
      position = new_position;
      q = state;
    }
  }

  float GeneralizedHiddenMarkovModel::MEAPred(const Sequence &s, Sequence &path){
    fMatrix probs;
    int size = s.size();
    int nstates = _all_states.size();
    posteriorProbabilitiesNoClasses(s,probs);
    Matrix ea(size,nstates);
    IntMatrix ptrs(size,nstates);

    //Initialization
    for(int k = 0; k < nstates; k++){
      float prob = probs(0,k);
      ea(0,k) = prob;
    }

    //Recursion
    for(int i = 1; i < size; i++){
      for(int k = 0; k < nstates; k++){
  float max = 0.0;
  int state = -1;
  std::vector<int> preds = _all_states[k]->predecessors();
  preds.push_back(k);
  for(int p = 0; p < (int)preds.size(); p++){
    int id = preds[p];
    if(max < ea(i-1,id)){
      max = ea(i-1,id);
      state = id;
    }
  }
  ea(i,k) = max + probs(i,k);
  ptrs(i,k) = state;
      }
    }

    //Traceback
    path.resize(size);
    float mea = 0.0;
    int state = -1;
    for(int k = 0; k < nstates; k++){
      if(ea(size-1,k) > mea){
  mea = ea(size-1,k);
  state = k;
      }
    }
    path[size-1] = state;
    for(int i = size-1; i > 0; i--){
      state = ptrs(i,path[i]);
      path[i-1] = state;
    }
    return mea/size;
  }

  float GeneralizedHiddenMarkovModel::MEAPred(const Sequence &s, Sequence &path, SparseMatrixPtr ppPred){
    fMatrix probs;
    ppPred->getfMatrixPred(probs);
    int size = s.size();
    int nstates = _all_states.size();
    Matrix ea(size,nstates);
    IntMatrix ptrs(size,nstates);

    //Initialization
    for(int k = 0; k < nstates; k++){
      ea(0,k) = probs(0,k);
    }

    //Recursion
    for(int i = 1; i < size; i++){
      for(int k = 0; k < nstates; k++){
  float max = 0.0;
  int state = -1;
  std::vector<int> preds = _all_states[k]->predecessors();
  preds.push_back(k);
  for(int p = 0; p < (int)preds.size(); p++){
    int id = preds[p];
    if(max < ea(i-1,id)){
      max = ea(i-1,id);
      state = id;
    }
  }
  ea(i,k) = max + probs(i,k);
  ptrs(i,k) = state;
      }
    }

    //Traceback
    path.resize(size);
    float mea = 0.0;
    int state = -1;
    for(int k = 0; k < nstates; k++){
      if(ea(size-1,k) > mea){
  mea = ea(size-1,k);
  state = k;
      }
    }
    path[size-1] = state;
    for(int i = size-1; i > 0; i--){
      state = ptrs(i,path[i]);
      path[i-1] = state;
    }
    return mea/size;
  }

  void GeneralizedHiddenMarkovModel::posteriorProbabilities(const Sequence &s, fMatrix &postProbs) const{
    posteriorProbabilitiesNoClasses(s,postProbs);
  }

  void GeneralizedHiddenMarkovModel::posteriorProbabilitiesNoClasses (const Sequence &s, fMatrix &postProbs) const{
    int size = s.size();
    int nstates = _all_states.size();
    initialize_prefix_sum_arrays(s);

    Matrix beta;
    double prob = backward(s,beta);

    Matrix alpha (nstates, size);

    postProbs.resize(size,nstates);

    std::vector< std::list<int> > valid_positions;
    valid_positions.resize(nstates);
    std::list <int> e;
    for(int k = 0; k <nstates; k++)
      valid_positions[k] = e;

    // initialization
    for (int c = 0; c < nstates; c++) {
      postProbs(0,c) = 0.0;
    }
    for (int k = 0; k < nstates; k++) {
      alpha(k, 0) = getInitialProbabilities()->log_probability_of(k)
  + _all_states[k]->observation()->prefix_sum_array_compute(0, 0, _all_states[k]->getInputPhase()) + _all_states[k]->duration_probability(1);
      if(alpha(k,0) > -HUGE){
  postProbs(0,k) = exp(alpha(k,0) + beta(k,0) - prob);
  std::vector<int> succ = _all_states[k]->successors();
  for(int p = 0; p < (int)succ.size(); p++){
    int id = succ[p];
    if(!_all_states[id]->isGeometricDuration())
      valid_positions[id].push_back(0);
  }
      }
    }

    for (int i = 1; i < size; i++) {
      for(int c = 0; c < nstates; c++)
  postProbs(i,c) = 0.0;
      for(int k = 0; k < nstates; k++) {
  _all_states[k]->posteriorSum(alpha, beta, postProbs, s, i, _all_states, valid_positions, prob, k);
      }
    }

    /*double sum = alpha(0, size-1) + _terminal_probabilities->log_probability_of(0);
    for(int k = 1; k < nstates; k++)
      sum = log_sum(sum, alpha(k, size-1) + _terminal_probabilities->log_probability_of(k));
      cout << "prob = " << prob << "  postProb = " << sum << endl;*/
      //cout << (probabilities.value_data()).size() << "  " << postProbs.size1()*postProbs.size2() << endl;
  }

  void GeneralizedHiddenMarkovModel::posteriorProbabilitiesWithClasses (const Sequence &s, SparseMatrixPtr probabilities) const{
    int size = s.size();
    int nstates = _all_states.size();
    initialize_prefix_sum_arrays(s);

    Matrix beta;
    double prob = backward(s,beta);

    Matrix alpha (nstates, size);

    fMatrix postProbs(size,_nclasses);

    std::vector< std::list<int> > valid_positions;
    valid_positions.resize(nstates);
    std::list <int> e;
    for(int k = 0; k <nstates; k++)
      valid_positions[k] = e;

    // initialization
    for (int c = 0; c < _nclasses; c++) {
      postProbs(0,c) = 0.0;
    }
    for (int k = 0; k < nstates; k++) {
      alpha(k, 0) = getInitialProbabilities()->log_probability_of(k)
  + _all_states[k]->observation()->prefix_sum_array_compute(0, 0, _all_states[k]->getInputPhase()) + _all_states[k]->duration_probability(1);
      if(alpha(k,0) > -HUGE){
  for(int c = 0; c < (int)(_all_states[k]->classes()).size(); c++)
    postProbs(0,_all_states[k]->classes()[c]) = exp(alpha(k,0) + beta(k,0) - prob);
  std::vector<int> succ = _all_states[k]->successors();
  for(int p = 0; p < (int)succ.size(); p++){
    int id = succ[p];
  if(!_all_states[id]->isGeometricDuration())
    valid_positions[id].push_back(0);
  }
      }
    }

    for (int i = 1; i < size; i++) {
      for(int c = 0; c < _nclasses; c++)
  postProbs(i,c) = 0.0;
      for(int k = 0; k < nstates; k++) {
  _all_states[k]->posteriorSum(alpha, beta, postProbs, s, i, _all_states, valid_positions, prob, -1);
      }
    }

    probabilities->buildPredMatrix(size,_nclasses,postProbs);


    /*double sum = alpha(0, size-1) + _terminal_probabilities->log_probability_of(0);
    for(int k = 1; k < nstates; k++)
      sum = log_sum(sum, alpha(k, size-1) + _terminal_probabilities->log_probability_of(k));

      cout << "prob = " << prob << "  postProb = " << sum << endl;*/
      //cout << (probabilities.value_data()).size() << "  " << postProbs.size1()*postProbs.size2() << endl;
  }


double GeneralizedHiddenMarkovModel::forward(const Sequence & s, Matrix &a) const {
  int size = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);

  Matrix alpha (nstates, size);

  std::vector< std::list<int> > valid_positions;
  valid_positions.resize(nstates);
  std::list <int> e;
  for(int k = 0; k <nstates; k++)
      valid_positions[k] = e;

  // initialization
  for (int k = 0; k < nstates; k++) {
    alpha(k, 0) = getInitialProbabilities()->log_probability_of(k)
      + _all_states[k]->observation()->prefix_sum_array_compute(0, 0, _all_states[k]->getInputPhase()) + _all_states[k]->duration_probability(1);
    if(alpha(k,0) > -HUGE){
      std::vector<int> succ = _all_states[k]->successors();
      for(int p = 0; p < (int)succ.size(); p++){
  int id = succ[p];
  if(!_all_states[id]->isGeometricDuration())
    valid_positions[id].push_back(0);
      }
    }
  }

  for (int i = 1; i < size; i++) {
      for(int k = 0; k < nstates; k++) {
  _all_states[k]->forwardSum(alpha, s, i, _all_states, valid_positions);
      }
  }

  a = alpha;
  double sum = alpha(0, size-1) + _terminal_probabilities->log_probability_of(0);
  for(int k = 1; k < nstates; k++)
    sum = log_sum(sum, alpha(k, size-1) + _terminal_probabilities->log_probability_of(k));

  return sum;
}



double GeneralizedHiddenMarkovModel::inefficient_forward(const Sequence & s, Matrix &a) const {
  int size = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);

  Matrix alpha (nstates, size);

  // initialization
  for (int k = 0; k < nstates; k++) {
    alpha(k, 0) = getInitialProbabilities()->log_probability_of(k) + _all_states[k]->observation()->prefix_sum_array_compute(0, 0);
  }

  for (int i = 1; i < size; i++) {
    printf("i = %d\n",i);
    for(int k = 0; k < nstates; k++) {
      alpha(k, i) = -HUGE;
      for(int d = i; d > 0; d--){
  int nphase = _all_states[k]->getInputPhase();
  if(_all_states[k]->observation()->inhomogeneous() != NULL)
    nphase = _all_states[k]->observation()->inhomogeneous()->maximumTimeValue() + 1;
  if(_all_states[k]->getStart() > 0 && _all_states[k]->getStop() > 0) {
    if((i-d+1-_all_states[k]->getStart() >= 0) && (i + _all_states[k]->getStop() < (int)s.size())) {
      double joinable = _all_states[k]->observation()->prefix_sum_array_compute(i-d+1-_all_states[k]->getStart(),i+_all_states[k]->getStop(), mod(_all_states[k]->getInputPhase()-_all_states[k]->getStart(), nphase));
      if(joinable <= -HUGE) {
        continue;
      }
    }
  }
        for(int p = 0; p < nstates; p++){
          alpha(k, i) = log_sum(alpha(k, i), alpha(p, i-d)
        + _all_states[p]->transition()->log_probability_of(k)
        + _all_states[k]->duration_probability(d)
        + _all_states[k]->observation()->prefix_sum_array_compute(i-d+1, i, _all_states[k]->getInputPhase()));
        }
      }
    }
  }

  a = alpha;

  double sum = alpha(0, size-1) + _terminal_probabilities->log_probability_of(0);
  for(int k = 1; k < nstates; k++)
    sum = log_sum(sum, alpha(k, size-1) + _terminal_probabilities->log_probability_of(k));

  printf("forward: %f\n\n", sum);

  return sum;

}




//! Backward algorithm
double GeneralizedHiddenMarkovModel::backward(const Sequence & s, Matrix &b) const {
  int size = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);

  Matrix beta (nstates, size);

  std::vector< std::list<int> > valid_positions;
  valid_positions.resize(nstates);
  std::list <int> e;
  for(int k = 0; k <nstates; k++)
      valid_positions[k] = e;

  // initialization
  for (int k = 0; k < nstates; k++) {
    beta(k, size-1) = _terminal_probabilities->log_probability_of(k);
    if(beta(k,size-1) > -HUGE){
      if(!_all_states[k]->isGeometricDuration())
  valid_positions[k].push_back(size-1);
    }
  }
  for (int i = size-2; i >= 0; i--){
    for (int k = 0; k < nstates; k++){
      beta(k,i) = -HUGE;
      for(int p = 0; p < (int)(_all_states[k]->successors()).size(); p++){
  int id = _all_states[k]->successors()[p];
  beta(k,i) = log_sum(beta(k,i), _all_states[k]->transition()->log_probability_of(id) + _all_states[id]->backwardSum(beta,s,i,valid_positions));
      }
      if(beta(k,i) > -HUGE){
  if(!_all_states[k]->isGeometricDuration())
      valid_positions[k].push_front(i);
      }
    }
  }

  b = beta;

  double sum = -HUGE;
  for(int k = 0; k < nstates; k++)
    sum = log_sum(sum, getInitialProbabilities()->log_probability_of(k) + _all_states[k]->backwardSum(beta,s,-1,valid_positions)) ;

  return sum;
}

  void GeneralizedHiddenMarkovModel::initialize_prefix_sum_arrays(const Sequence & s) const {
#if 0
    struct timeval start, stop;
    gettimeofday(&start, (struct timezone *) NULL);
#endif

    for (int i = 0; i < (int) _all_states.size(); i++) {
      _all_states[i]->observation()->initialize_prefix_sum_array(s);
    }
#if 0
    gettimeofday(&stop, (struct timezone *)NULL);
    stop.tv_sec -= start.tv_sec;
    stop.tv_usec -= start.tv_usec;
    if(stop.tv_usec  < 0){
      stop.tv_sec --;
      stop.tv_usec += 1000000;
    }
    fprintf(stderr, "PSA Elapsed time %ld%c%02d seconds\n", stop.tv_sec, '.', stop.tv_usec/1000);
#endif

  }



void GeneralizedHiddenMarkovModel::fixStatesPredecessorSuccessor() {
  for (int i = 0; i < (int) _all_states.size(); i++)
    {
      if (_all_states[i] != NULL)
        {
          for (int j = 0; j < (int) _all_states.size(); j++) {
            if ((!close(exp(_all_states[i]->transition()->log_probability_of(j)), 0.0, 1e-1)) &&  (_all_states[j] != NULL)) {
              _all_states[j]->addPredecessor(i);
            }

            if ((_all_states[j] != NULL) && (!close(exp( _all_states[i]->transition()->log_probability_of(j)),
                                                   0.0, 1e-1)))
              _all_states[i]->addSuccessor(j);
          }
        }
    }
  for (int i = 0; i < (int) _all_states.size(); i++)
    {
    if (_all_states[i] != NULL)
      {
        _all_states[i]->fixTransitionDistribution();
        _all_states[i]->clearPredecessorSuccessor();
      }
    }
  for (int i = 0; i < (int) _all_states.size(); i++)
    {
      if (_all_states[i] != NULL)
        {

          for (int j = 0; j < (int) _all_states.size(); j++) {
            double transprob = exp(_all_states[i]->transition()->log_probability_of(j));
            if ((!close(transprob, 0.0, 1e-1)) &&  (_all_states[j] != NULL)) {
              _all_states[j]->addPredecessor(i);
              if((!_all_states[j]->isGeometricDuration())  && (!_all_states[i]->isGeometricDuration()))
                {
                  std::cerr << "WARNING: Transitions between two non-geometric run-length states make viterbi decoding slow: " << _all_states[i]->name() << "->" << _all_states[j]->name() << ": "  << transprob << std::endl;
                }
            }

            if ((_all_states[j] != NULL) && (!close(transprob,0.0, 1e-1))) {
              _all_states[i]->addSuccessor(j);
              if((!_all_states[i]->isGeometricDuration())  && (!_all_states[j]->isGeometricDuration()))
                {
                  std::cerr << "WARNING: Transitions between two non-geometric run-length states make viterbi decoding slow: " << _all_states[i]->name() << "->" << _all_states[j]->name() << ": "  << transprob << std::endl;
                }
            }
          }
        }
    }

}


//! Inefficient Viterbi algorithm
double GeneralizedHiddenMarkovModel::_viterbi(const Sequence &s, Sequence &path,
                Matrix & g) const {
  int size = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);

  Matrix gamma(nstates, size);
  Matrix psi(nstates, size);
  IntMatrix psilen(nstates, size);

  // initialization
  for (int k = 0; k < nstates; k++) {
    gamma(k, 0) = getInitialProbabilities()->log_probability_of(k)
      + _all_states[k]->observation()->prefix_sum_array_compute(0, 0);
  }

  for(int i = 1; i < size; i++){
    for(int k = 0; k < nstates; k++){
      gamma(k, i) = - HUGE;
      for(int d = i; d > 0; d--){
        double gmax = gamma(0, i-d) + _all_states[0]->transition()->log_probability_of(k);
        int pmax = 0;
        for(int p = 1; p < nstates; p++){
          double g = gamma(p, i-d) + _all_states[p]->transition()->log_probability_of(k);
          if(gmax < g){
            gmax = g;
            pmax = p;
          }
        }
        gmax = gmax + _all_states[k]->duration_probability(d) + _all_states[k]->observation()->prefix_sum_array_compute(i-d+1, i);
        if(gamma(k, i) < gmax){
          gamma(k, i) = gmax;
          psi(k, i) = pmax;
          psilen(k, i) = d;
        }
      }
    }
  }

  //backtracing
  path.resize(size);
  int L = size-1;
  int state = 0;
  double max = gamma(0, L);
  for(int k = 1; k < nstates; k++){
    if(max < gamma(k, L)){
      max = gamma(k, L);
      state = k;
    }
  }
  while(L > 0){
    int d = psilen(state, L);
    int p = psi(state, L);
    for(int i = 0; i < d; i++){
      path[L] = state;
      L--;
    }
    state = p;
  }
  return max;
}


//! Viterbi algorithm
double GeneralizedHiddenMarkovModel::viterbi(const Sequence &s, Sequence &path,
                Matrix & g) const {
  int size = s.size();
  int nstates = _all_states.size();

  initialize_prefix_sum_arrays(s);

  Matrix gamma(nstates, size);
  Matrix psi(nstates, size);
  IntMatrix psilen(nstates, size);

  std::map < int, std::list<int>  > valid_positions;
  std::list <int> e;
  for(int k = 0; k <nstates; k++)
      valid_positions[k] = e;


  std::vector<bool> possible_path;
  possible_path.resize(nstates);

  // initialization
  for (int k = 0; k < nstates; k++) {
    if (_all_states[k]->isGeometricDuration())
      gamma(k, 0) = getInitialProbabilities()->log_probability_of(k) + _all_states[k]->observation()->prefix_sum_array_compute(0, 0);
    else 
      gamma(k, 0) = getInitialProbabilities()->log_probability_of(k) + _all_states[k]->observation()->prefix_sum_array_compute(0, 0, _all_states[k]->getInputPhase());

      possible_path[k] = 1;
      if(gamma(k,0) <= -HUGE)
	possible_path[k] = 0;
         
  }


  for(int i = 0; i < (int)possible_path.size(); i++)
      {
          if(!possible_path[i])
              continue;
          std::vector <int> next_states = _all_states[i]->successors();
	  if(!_all_states[i]->isGeometricDuration())
	    (valid_positions.find(i)->second).push_back(-1);
          for(int p = 0; p < (int)next_states.size(); p++)
              {
                  int succ = next_states[p];
                  if (!_all_states[succ]->isGeometricDuration()) {
                      (valid_positions.find(succ)->second).push_back(-1);
                  }
              }
      }
 

  for(int i = 1; i < size; i++){
      for(int k = 0; k < nstates; k++){
          gamma(k, i) = -HUGE;
          possible_path[k] = 1;
          _all_states[k]->findBestPredecessor (gamma, psi, psilen,  s, i, _all_states, valid_positions);
          if(gamma(k,i) <= -HUGE)
	    possible_path[k] = 0;
      }
      for(int s = 0; s < (int)possible_path.size(); s++)
          {
              std::vector <int> next_states = _all_states[s]->successors();
              if(!possible_path[s])
                  continue;
              for(int p = 0; p < (int)next_states.size(); p++)
                  {
                      int succ = next_states[p];
                      if (!_all_states[succ]->isGeometricDuration()) {
                          (valid_positions.find(succ)->second).push_back(i);
                      }
                  }
          }

  }
  int L = size-1;

  if(_terminal_probabilities != NULL){
      for(int k = 0; k < nstates; k++)
          gamma(k,L) += _terminal_probabilities->log_probability_of(k);
  }
  //backtracing
  path.resize(size);

  int state = 0;
  double max = gamma(0, L);
  for(int k = 1; k < nstates; k++){
    if(max < gamma(k, L)){
        max = gamma(k, L) ;
        state = k;
    }
  }
#if 0
  for(int i = 0; i < size; i++) {
      std::cerr << "i: " << i << std::endl;
      for(int k = 0; k < nstates; k++){
          std::cerr << " "  << getStateName(k) << " " << gamma(k, i) << " " << getStateName(psi(k, i)) << " " << psilen(k,i) << std::endl;
      }

  }
#endif
  while(1){
      int d = psilen(state, L);
      int p = psi(state, L);
      if (L == 0) {
	path[0] = state;
	break;
      }
      for(int i = 0; i < d; i++){
          path[L] = state;
	  if(L==0) break;
          L--;
      }
      if(L != 0 && d == 0)
          {
              std::cerr << "Something wrong: [ predicted state duration equals to " << d << "]" << std::endl;
              break;
          }
     state = p;
  }
#if 0
  for(int i = 0; i < size ; i ++) {
      std::cerr << "[" << i << " " << alphabet()->getSymbol(s[i])->name() << "]" << std::endl; ;
      for(int k = 0; k < nstates; k++)
          {
	    int d = psilen(k, i);
	    assert (d >= 0);
	    assert (d <= s.size());
	    std::cerr << " " << _all_states[k] -> name() << " " << gamma(k,i) << " " << _all_states[psi(k, i)]->name() << " " << d << std::endl;
          }
  }
#endif
  return max;

}

//! Choose the observation given a state
Sequence & GeneralizedHiddenMarkovModel::chooseObservation(Sequence & h, int i,
                int state) const {
  assert(state >= 0 && state < (int)_all_states.size());
  int d = _all_states[state]->chooseDuration();
  int iphase = _all_states[state]->getInputPhase();
  _all_states[state]->observation()->chooseWithHistory(h, i, iphase, d);
  return h;
}

  //! Choose a state
  int GeneralizedHiddenMarkovModel::chooseState(int state) const {
      return _all_states[state]->transition()->choose();
  }

  //! Choose the initial state
  int GeneralizedHiddenMarkovModel::chooseFirstState() const {
    return _initial_probabilities->choose();
  }

  //! Get state name
  std::string GeneralizedHiddenMarkovModel::getStateName(int state) const {
    return _all_states[state]->name();
  }


  //! Get the state names
  AlphabetPtr GeneralizedHiddenMarkovModel::getStateNames() const {
    return _state_names;
  }

  ProbabilisticModelCreatorPtr GeneralizedHiddenMarkovModel::getFactory() const {
    return GeneralizedHiddenMarkovModelCreatorPtr(
                                                  new GeneralizedHiddenMarkovModelCreator());
  }




  std::string GeneralizedHiddenMarkovModel::str() const {
    int nstates = getStateNames()->size();
    std::stringstream out;
    out << "model_name = \"" << model_name() << "\"" << std::endl;
    out << "state_names = (";
    int i = 0;
    if (i < nstates) {
      out << "\"" << getStateName(i) << "\"";
      for (i = 1; i < nstates; i++)
        out << ",\"" << getStateName(i) << "\"";
      out << ")" << std::endl;
    }

    out << "observation_symbols = (";
    out << "\"" << alphabet()->getSymbol(0)->name() << "\"";
    for (int i = 1; i < (int) alphabet()->size(); i++)
      out << ",\"" << alphabet()->getSymbol(i)->name() << "\"";
    out << ")" << std::endl;

    out << "transitions = (";
    if(!close(exp(_all_states[0]->transition()->log_probability_of(0)), 0, 1e-10))
      out << "\"" << getStateName(0) << "\" | \"" << getStateName(0) << "\": "
          << exp(_all_states[0]->transition()->log_probability_of(0));
    for (int i = 0; i < nstates; i++)
      for (int j = 0; j < nstates; j++)
        if (((i != 0) || (j != 0)) && (!close(exp( _all_states[i]->transition()->log_probability_of(j)), 0, 1e-10) ))
          out << ";\n \"" << getStateName(j) << "\" | \""
              << getStateName(i) << "\": " << exp( _all_states[i]->transition()->log_probability_of(j));
    out << ")" << std::endl;

    out << "initial_probabilities = (";
    if(!close(exp(_initial_probabilities->log_probability_of(0)), 0, 1e-10))
      out << "\"" << getStateName(0) << "\":  " << exp(_initial_probabilities->log_probability_of(0));
    for (int i = 1; i < nstates; i++)
      {
        if(!close(exp(_initial_probabilities->log_probability_of(i)), 0.0, 1e-10))
          out << ";\n \"" << getStateName(i) << "\": " << exp(
                                                              _initial_probabilities->log_probability_of(i));
      }
    out << ")" << std::endl;

    if(_terminal_probabilities != NULL) {
        out << "terminal_probabilities = (";
        if(!close(exp(_terminal_probabilities->log_probability_of(0)), 0, 1e-10))
            out << "\"" << getStateName(0) << "\":  " << exp(_terminal_probabilities->log_probability_of(0));
        for (int i = 1; i < nstates; i++)
            {
                if(!close(exp(_terminal_probabilities->log_probability_of(i)), 0.0, 1e-10))
                    out << ";\n \"" << getStateName(i) << "\": " << exp(_terminal_probabilities->log_probability_of(i));
            }
        out << ")" << std::endl;
    }

    std::map <std::string, ProbabilisticModelPtr> ::const_iterator it;

    for(it = _models.begin(); it != _models.end(); it++)
      {
        out << it->first << " = [ "  << (it->second)->str() << " ] " << std::endl;
      }

    for (int i = 0; i < nstates; i++)
      out << _all_states[i]->str();

    return out.str();
  }

    void GeneralizedHiddenMarkovModel::buildDoubleParameterValue(DiscreteIIDModelPtr distr, ProbabilisticModelParameters & answer, const char * name) const
    {
        int nstates = getStateNames()->size();
        double sum = 0.0;
        std::vector <double> probs;
        probs.resize(nstates);
        for(int i = 0; i < nstates; i++){
            probs[i] = exp(distr->log_probability_of(i));
            sum += probs[i];
        }
        std::map <std::string, double> aux;
        std::stringstream out5;
        out5 << getStateName(0);
        aux[out5.str()] =  probs[0]/sum;
        for(int i = 0; i < nstates; i++)
            for(int j = 0; j < (int)alphabet()->size(); j++)
                if((i != 0) || (j != 0)){
                    std::stringstream out6;
                    out6 << getStateName(i) ;
                    aux[out6.str()] = probs[i]/sum;
                }
        answer.add(name, DoubleMapParameterValuePtr(new DoubleMapParameterValue(aux)));

    }

  ProbabilisticModelParameters GeneralizedHiddenMarkovModel::parameters() const
  {
    int nstates = getStateNames()->size();
    ProbabilisticModelParameters answer;

    answer.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name())));
    answer.add("state_names", _state_names->getParameterValue());
    answer.add("observation_symbols", alphabet()->getParameterValue());

    std::map <std::string, double> trans;
    std::stringstream out;
    out << getStateName(0) << "|" << getStateName(0);
    trans[out.str()] =  exp(_all_states[0]->transition()->log_probability_of(0)) ;
    for(int i = 0; i < nstates; i++)
      for(int j = 0; j < nstates; j++)
        if((i != 0) || (j != 0)) {
          std::stringstream out2;
          out2 <<  getStateName(j) << "|" << getStateName(i) ;
          trans[out2.str()] = exp(_all_states[i]->transition()->log_probability_of(j));
        }

    answer.add("transitions", DoubleMapParameterValuePtr(new DoubleMapParameterValue(trans)));

    buildDoubleParameterValue(_initial_probabilities, answer, "initial_probabilities");
    if(_terminal_probabilities != NULL)
        buildDoubleParameterValue(_terminal_probabilities, answer, "terminal_probabilities");
    std::map <std::string, ProbabilisticModelPtr> ::const_iterator it;

    for(it = _models.begin(); it != _models.end(); it++)
      {
        answer.add(it->first, ProbabilisticModelParameterListValuePtr(new ProbabilisticModelParameterListValue((it->second)->parameters())));
      }

    for (int i = 0; i < nstates; i++)
      {
        answer.add(_all_states[i]->name(), ProbabilisticModelParameterListValuePtr(new ProbabilisticModelParameterListValue(_all_states[i]->parameters())));
      }

    return answer;
  }

  void GeneralizedHiddenMarkovModel::initialize(const ProbabilisticModelParameters & parameters)
  {

    ProbabilisticModelParameterValuePtr state_names_par =
      parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr initial_probabilities_par =
      parameters.getMandatoryParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr transitions_par =
      parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr observation_symbols_par =
      parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr terminal_probabilities_par =
        parameters.getOptionalParameterValue("terminal_probabilities");


    std::vector<std::string> state_names = state_names_par->getStringVector();
    AlphabetPtr states = AlphabetPtr(new Alphabet());
    states->initializeFromVector(state_names);
    setStateNames(states);
    AlphabetPtr observation_symbols = AlphabetPtr(new Alphabet());
    observation_symbols->initializeFromVector(observation_symbols_par->getStringVector());

    DiscreteIIDModelPtr pi = DiscreteIIDModelPtr(new DiscreteIIDModel());
    pi->initializeFromMap(initial_probabilities_par->getDoubleMap(), states);

    if(terminal_probabilities_par != NULL){
        DiscreteIIDModelPtr terminalprob = DiscreteIIDModelPtr(new DiscreteIIDModel());
        terminalprob->initializeFromMap(terminal_probabilities_par->getDoubleMap(), states);
        setTerminalProbability(terminalprob);
    }


    std::map<std::string, double> transpar = transitions_par->getDoubleMap();
    std::map<std::string, DoubleVector> trans;
    std::map<std::string, double>::const_iterator it;
    boost::regex separator("\\s*\\|\\s*");

    for (it = transpar.begin(); it != transpar.end(); it++) {
      std::vector<std::string> splited;
      split_regex(it->first, splited, separator);
      if(splited.size() == 1) {
        splited.push_back("");
      }
      std::string from (splited[1]);
      std::string to ( splited[0]);
      if(!states->has(from) ) {
         std::cerr << "ERROR: The state '" << from << "' is not in state list !\n" << std::endl;
         exit(-1);
      }
      if(!states->has(to) ) {
         std::cerr << "ERROR: The state '" << to << "' is not in state list !\n" << std::endl;
         exit(-1);
      }

      if (trans.find(from) == trans.end()) {
        int id = states->getSymbol(to)->id();
        DoubleVector probs;
        probs.resize(states->size() );
        trans[from] = probs;
        if (id < (int) trans[from].size())
          trans[from][id] = it->second;
      } else {
        int id = states->getSymbol(to)->id();
        if (id < (int) trans[from].size())
          trans[from][id] = it->second;
      }
    }

    setAlphabet(observation_symbols);
    int nclasses = -1;
    for (int i = 0; i < (int) state_names.size(); i++) {
      trim_spaces(state_names[i]);
      int id;
      ProbabilisticModelParameterValuePtr statepar =
        parameters.getOptionalParameterValue(state_names[i]);
      if (statepar == NULL) {
        std::cerr << "ERROR: Missing configuration for the state: "
                  << state_names[i] << std::endl;
        return;
      }
      ConfigurationReader reader;
      std::string config = statepar->getString();
      if(config[0] == '[')
        config = config.substr(1, config.size() - 2);
      if (!reader.load(config)) {
        std::cerr << "ERROR: Configuring " << state_names[i] << "!" << std::endl;
        std::cerr << config << std::endl;
        return;
      } else {
        ProbabilisticModelParametersPtr statepars = reader.parameters();
        ProbabilisticModelParameterValuePtr observationpar =
          statepars->getOptionalParameterValue("observation");
        ProbabilisticModelParameterValuePtr durationpar =
          statepars->getOptionalParameterValue("duration");
        ProbabilisticModelParameterValuePtr lengthpar =
          statepars->getOptionalParameterValue("sequence_length");
        ProbabilisticModelParameterValuePtr outputphasepar =
          statepars->getOptionalParameterValue("output_phase");
        ProbabilisticModelParameterValuePtr inputphasepar =
          statepars->getOptionalParameterValue("input_phase");

	ProbabilisticModelParameterValuePtr extendEmissionpar =
          statepars->getOptionalParameterValue("extend_emission");
	ProbabilisticModelParameterValuePtr classespar =
	  statepars->getOptionalParameterValue("classes");


        if (observationpar == NULL) {
          std::cerr << "ERROR: Missing  observation model for state "
                    << state_names[i] << std::endl;
          return;
        }

        std::string model_name = observationpar->getString();
        restore_model(model_name, parameters);

        DiscreteIIDModelPtr transition =
          DiscreteIIDModelPtr(
                                        new DiscreteIIDModel(
                                                                       trans[state_names[i]]));

        int iphase = -1;
        int ophase = -1;
  std::vector<int> classes;
  if(classespar != NULL){
    std::vector<double> cl = classespar->getDoubleVector();
    classes.resize(cl.size());
    for(int c = 0; c < (int)cl.size(); c++){
      classes[c] = (int)cl[c];
      if(classes[c] > nclasses)
        nclasses = classes[c];
    }
  }
        if(inputphasepar != NULL)
            iphase = inputphasepar->getInt();
        if(outputphasepar != NULL)
            ophase = outputphasepar->getInt();
        if (durationpar == NULL) {
            if (lengthpar == NULL) {
        id = configureGeometricDurationState(model_name, transition, state_names[i], iphase, ophase, classes);
            } else {
                id = configureSignalState(model_name,
                                     transition,
                                     lengthpar->getDouble(),
            state_names[i], iphase, ophase, classes);
            }

        } else {

            // Explicit duration state
            std::string duration_model_name = durationpar->getString();
            restore_model(duration_model_name,  parameters);
            id = configureExplicitDurationState(model_name, transition, duration_model_name, state_names[i], iphase, ophase, classes);
            if(extendEmissionpar != NULL) {
                int startEmissionOffset = (int)(extendEmissionpar->getDoubleVector())[0];
                int endEmissionOffset = (int)(extendEmissionpar->getDoubleVector())[1];
                _all_states[id]->setStart(startEmissionOffset);
                _all_states[id]->setStop(endEmissionOffset);
            } else {
                _all_states[id]->setStart(0);
                _all_states[id]->setStop(0);
	    }
        }
      }
    }
    nclasses++;
    setNumClasses(nclasses);
    setObservationSymbols(observation_symbols);
    setInitialProbability(pi);

    fixStatesPredecessorSuccessor();

    for (int i = 0; i < (int)_all_states.size(); i++)
        if (_all_states[i]->transition()->size() <= 0)
            {
                std::cerr << "ERROR: GHMM initialization, state " << _all_states[i]->name() << " has outdegree equals to zero !\n";
                exit(-1);
            }
  }


}

