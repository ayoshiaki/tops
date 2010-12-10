#include "GeneralizedHiddenMarkovModel.hpp"
#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"
#include "GHMMStates.hpp"
#include "util.hpp"
#include <sys/types.h>
#include <sys/time.h>

namespace tops {


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
		MultinomialDistributionPtr init) {
	_initial_probabilities = init;
}

void GeneralizedHiddenMarkovModel::configureSignalState(
							std::string observation_model_name, std::string null_model_name,
							MultinomialDistributionPtr transition_distr, double threshold,
							int size, std::string state_name, int iphase, int ophase){
  SymbolPtr symbol = _state_names->getSymbol(state_name);
  ProbabilisticModelPtr model = _models[observation_model_name];
  ProbabilisticModelPtr null = _models[null_model_name];
  GHMMSignalStatePtr signal = GHMMSignalStatePtr(new GHMMSignalState(model,transition_distr, symbol));
  signal->setSize(size);
  signal->setNullModel(null);
  signal->nullModelName(null_model_name);
  signal->observationModelName(observation_model_name);
  signal->setThreshold(threshold);
  signal->setInputPhase(iphase);
  signal->setOutputPhase(ophase);
  _all_states[symbol->id()] = signal;
  _signal_states.push_back(signal);
}

void GeneralizedHiddenMarkovModel::configureGeometricDurationState(
								   std::string model_name,
								   MultinomialDistributionPtr transition_distr, std::string state_name, int iphase, int ophase) {
  ProbabilisticModelPtr model = _models[model_name];
  SymbolPtr symbol = _state_names->getSymbol(state_name);
  GHMMStatePtr state = GHMMStatePtr(new GHMMState(model, transition_distr,
						  symbol));
  state->setInputPhase(iphase);
  state->setOutputPhase(ophase);
  _all_states[symbol->id()] = state;
  state->observationModelName(model_name);
  _geometric_duration_states.push_back(state);
}


void GeneralizedHiddenMarkovModel::configureExplicitDurationState(
								  std::string observation_model_name,
		MultinomialDistributionPtr transition_distr,
								  std::string duration_model_name, std::string state_name, int iphase, int ophase, int start, int stop, int leftJoinable, int rightJoinable) 
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
  state->setStart(start);
  state->setStop(stop);
  state->isLeftJoinable(leftJoinable);
  state->isRightJoinable(rightJoinable);
  _all_states[symbol->id()] = state;
  _explicit_duration_states.push_back(state);

}


double GeneralizedHiddenMarkovModel::forward(const Sequence & s, Matrix &a) const {
  int size = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);

  Matrix alpha (nstates, size);

  // initialization
  for (int k = 0; k < nstates; k++) {
    alpha(k, 0) = getInitialProbabilities()->log_probability_of(k)
      + _all_states[k]->observation()->prefix_sum_array_compute(0, 0);
  }

  for (int i = 1; i < size; i++) {
    for(int k = 0; k < nstates; k++) {
      alpha(k, i) = alpha(0, 0) + _all_states[0]->transition()->log_probability_of(k)
                  + _all_states[k]->duration_probability(i)
                  + _all_states[k]->observation()->prefix_sum_array_compute(0, i);
      for(int d = i; d > 0; d--){
        for(int p = 0; p < nstates; p++){
          alpha(k, i) = log_sum(alpha(k, i), alpha(p, i-d)
                      + _all_states[p]->transition()->log_probability_of(k)
                      + _all_states[k]->duration_probability(d)
                      + _all_states[k]->observation()->prefix_sum_array_compute(i-d+1, i));
        }
      }
    }
  }

  a = alpha;
  double sum = alpha(0, size-1);
  for(int k = 1; k < nstates; k++)
    sum = log_sum(sum, alpha(k, size-1));

  return sum;
}

//! Backward algorithm
double GeneralizedHiddenMarkovModel::backward(const Sequence & s, Matrix &beta) const {
  std::cerr << "Backward not implemented" << std::endl;
  return 0.0;
}

  void GeneralizedHiddenMarkovModel::initialize_prefix_sum_arrays(const Sequence & s) const {
#if 0			
    struct timeval start, stop;
    gettimeofday(&start, (struct timezone *) NULL);
#endif
    
    for (int i = 0; i < (int) _all_states.size(); i++) {
      if(_all_states[i] ->nullModel() != NULL) 
	_all_states[i]->nullModel()->initialize_prefix_sum_array(s);
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
    fprintf(stderr, "Elapsed time %ld%c%02d seconds\n", stop.tv_sec, '.', stop.tv_usec/1000);
#endif
    
  }
  
  void GeneralizedHiddenMarkovModel::findBestPredecessorGeometricStates(int i,
								      Matrix & gamma, std::vector<
								      std::vector<OptimalPredecessorPtr> > & ptr,
								      const Sequence & sequence) const {
   
  for (int k = 0; k < (int) _geometric_duration_states.size(); k++) {
    int to = _geometric_duration_states[k]->id();
    gamma(i, to) = -HUGE;
    std::vector<int> pred = _geometric_duration_states[k]->predecessors();

    for (int l = 0; l < (int) pred.size(); l++) {
      int from = pred[l];
      if (_all_states[from]->isGeometricDuration()) {
	double e = 0.0;
	if (i < (int) sequence.size())
	  e = _all_states[to]->observation()->prefix_sum_array_compute(i, i);
	assert (e <= 0);
	double v = gamma(i - 1, from)
	  + _all_states[from]->transition()->log_probability_of(to) + e;
	if (v > gamma(i, to)) {
	  gamma(i, to) = v;
	  int eid = ptr[i-1][from]->prevExplicitStateId();
	  int end = ptr[i-1][from]->endOfExplicitStateId();
	  ptr[i][to] = OptimalPredecessorPtr(new OptimalPredecessor(from, -1, eid,  i - 1, end));
	}
      }
    }
  }
}

//! Finds all candidate signals that are  successor of the  toSignal state and inserts the toSignal state as the predecessor candidate signal.
void GeneralizedHiddenMarkovModel::addSignalPredecessors(
		CandidateSignalPtr sig,
		std::vector<std::set<CandidateSignalPtr> > & predecessors) const {
	int toSignal = sig->stateId();
	std::vector<int> nextStates = _all_states[toSignal]->successors();
	for (int s = 0; s < (int) nextStates.size(); s++) {
	  if (!_all_states[nextStates[s]] ->isGeometricDuration()) 
	    predecessors[nextStates[s]].insert(sig);
	}
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
		  std::cerr << "WARNING: Transitions between two non-geometric run-length states are not supported: " << _all_states[i]->name() << "->" << _all_states[j]->name() << ": "  << transprob << std::endl;
		}
	    }
	    
	    if ((_all_states[j] != NULL) && (!close(transprob,0.0, 1e-1))) {
	      _all_states[i]->addSuccessor(j);
	      if((!_all_states[i]->isGeometricDuration())  && (!_all_states[j]->isGeometricDuration()))
		{
		  std::cerr << "WARNING: Transitions between two non-geometric run-length states are not supported: " << _all_states[i]->name() << "->" << _all_states[j]->name() << ": "  << transprob << std::endl;
		}
	    }
	  }
	}
    }

}

void GeneralizedHiddenMarkovModel::findBestPredecessorSignalStates(int i,
								   Matrix & gamma, std::vector<
								   std::vector<OptimalPredecessorPtr> > & ptr,
								   const Sequence & sequence,
								   std::vector<std::set<CandidateSignalPtr> > & predecessors) const {
  for (int s = 0; s < (int) _signal_states.size(); s++) {
    if (i < (_signal_states[s]->size() - 1))
      continue;
    double prob = _all_states[_signal_states[s]->id()]->observation()->prefix_sum_array_compute(i-_signal_states[s]->size() + 1, i);
    assert(prob <= 0);
    double null_prob = 0;

    null_prob = _signal_states[s]->nullModel()->prefix_sum_array_compute(i- _signal_states[s]->size() + 1, i);
    double score = prob - null_prob;    
    if(prob < -1e38)
      continue;
    if (score < _signal_states[s]->getThreshold())
      continue;


    CandidateSignalPtr sig = CandidateSignalPtr(new CandidateSignal( _signal_states[s]->id(), 
								     i - _signal_states[s]->size() + 1, i,
								     i + 1, prob, null_prob));
    
    int toSignal = sig->stateId();
    
    addSignalPredecessors(sig, predecessors);
    
    if (sig->begin() == 0)
      continue;
    
    gamma(i, toSignal) = -HUGE;
    
    std::vector<int> pred = _all_states[toSignal]->predecessors();

    
    for (int l = 0; l < (int) pred.size(); l++) {
      
      int from = pred[l];
      int begin = sig->begin();

      if (_all_states[from]->isGeometricDuration()) {
	double v = gamma(begin - 1, from)
	  + _all_states[from]->transition()->log_probability_of(toSignal) + prob;
	if (v > gamma(i, toSignal)) {
	  gamma(i, toSignal) = v;
	  int eid = ptr[begin-1][from]->prevExplicitStateId();
	  int end = ptr[begin-1][from]->endOfExplicitStateId();
	  ptr[i][toSignal] = OptimalPredecessorPtr(new OptimalPredecessor(from, -1, eid, begin - 1, end));
	}
      } else {
	std::set<CandidateSignalPtr>::iterator it;
	std::set<CandidateSignalPtr> toRemove;
	int explicitId = from;

	for (it = predecessors[explicitId].begin(); it
	       != predecessors[explicitId].end(); it++) {

	  CandidateSignalPtr predecessor = *it;
	  int begin = predecessor->begin_of_next_state();
	  int end = sig->begin() - 1;

	  if(begin > end) 
	    continue;
	  int fromSignal = predecessor->stateId();

	  

	  double emission = 0.0;
	  int duration = end - begin + 1;
	  int phase = _all_states[explicitId]->getInputPhase();
	  emission = _all_states[explicitId]->observation()->prefix_sum_array_compute(begin, end, phase);
	  int offsetStart = _all_states[explicitId]->getStart();
	  int offsetEnd = _all_states[explicitId]->getStop();

	  assert(emission <= 0);
	  double duration_prob =
	    _all_states[explicitId]->duration_probability(duration);

	  double check_cds = _all_states[explicitId]->observation()->prefix_sum_array_compute(begin-offsetStart, end+offsetEnd, phase);
	  if(close(exp(check_cds), 0.0, 1e-10)) {
	    toRemove.insert(predecessor); 
	    continue;
	  }
	  if(close(exp(emission), 0.0, 1e-10)) {
	    toRemove.insert(predecessor); 
	    continue;
	  }

	  if (close(exp(duration_prob), 0.0, 1e-10)) {
	    continue;
	  }

	  double v =	gamma(begin - 1, fromSignal)
	    + _all_states[fromSignal]->transition()->log_probability_of(explicitId) + duration_prob
	    + emission
	    + _all_states[explicitId]->transition()->log_probability_of(toSignal) + prob;


	  if (v > gamma(i, toSignal)) {
	    bool valid = true;
	    if(_all_states[explicitId]->isLeftJoinable()) {

	      int f = fromSignal;
	      int k = begin -1;
	      int previousEid = ptr[k][f]->prevExplicitStateId();
	      if(_all_states[previousEid]->isRightJoinable()) 
		{
		  Sequence joinedSeq;
		  int leftstart = ptr[k][f]->endOfExplicitStateId() + 1;
		  int leftend = ptr[k][f]->endOfExplicitStateId() + _all_states[previousEid]->getStop();
		  int rightstart = begin - _all_states[explicitId]->getStart();
		  int rightend = begin - 1;
		  for(int l = leftstart; l <= leftend; l++)
		    {
		      joinedSeq.push_back(sequence[l]);
		    }
		  for(int l = rightstart; l <= rightend ; l++)
		    {
		      joinedSeq.push_back(sequence[l]);
		    }
		  
		  double check =  _all_states[explicitId]->observation()->evaluate(joinedSeq, 0, joinedSeq.size() -1, phase);
		  
		  if(close(exp(check), 0.0, 1e-10)) {
		    valid = false;
		    toRemove.insert(predecessor); 
		  } 
		}
	    }
	    if(valid){
	      gamma(i, toSignal) = v;
	      ptr[i][toSignal] = OptimalPredecessorPtr(new OptimalPredecessor(fromSignal, explicitId, explicitId, begin - 1, begin + duration -1));
	    }
	    
	  }
	  
	}
	for(it = toRemove.begin(); it != toRemove.end(); it++)
	  predecessors[explicitId].erase(predecessors[explicitId].find(*it));
	
      }
    }
  }
}



void GeneralizedHiddenMarkovModel::findBestPredecessorSignalStatesFinish(int i,
								   Matrix & gamma, std::vector<
								   std::vector<OptimalPredecessorPtr> > & ptr,
								   const Sequence & sequence,
								   std::vector<std::set<CandidateSignalPtr> > & predecessors) const {
  for (int s = 0; s < (int) _signal_states.size(); s++) {
    if ((i - _signal_states[s]->size()+1) != (int)sequence.size())
      continue;
    int toSignal =  _signal_states[s]->id(); 
    gamma(i, toSignal) = -HUGE;

    std::vector<int> pred = _all_states[toSignal]->predecessors();
    for (int l = 0; l < (int) pred.size(); l++) {
      int from = pred[l];
      int begin = i;
      if (_all_states[from]->isGeometricDuration()) {
	double v = gamma(begin - 1, from) + _all_states[from]->transition()->log_probability_of(toSignal) ;
	if (v > gamma(i, toSignal)) {
	  gamma(i, toSignal) = v;
	  int eid = ptr[begin-1][from]->prevExplicitStateId();
	  int end = ptr[begin-1][from]->endOfExplicitStateId();
	  ptr[i][toSignal] = OptimalPredecessorPtr(new OptimalPredecessor(from, -1,eid, begin - 1, end));
	}
      } else {
	std::set<CandidateSignalPtr>::iterator it;
	std::set<CandidateSignalPtr> toRemove;
	int explicitId = from;
	for (it = predecessors[explicitId].begin(); it
	       != predecessors[explicitId].end(); it++) {
	  CandidateSignalPtr predecessor = *it;
	  int begin = predecessor->begin_of_next_state();
	  int end = sequence.size() - 1;
	  int fromSignal = predecessor->stateId();

	  double emission = 0.0;
	  int duration = end - begin + 1;
	  int phase = _all_states[explicitId]->getInputPhase();
	  emission = _all_states[explicitId]->observation()->prefix_sum_array_compute(begin, end, phase);
	  

	  double duration_prob =
	    _all_states[explicitId]->duration_probability(duration);
	  
	  double v =	gamma(begin - 1, fromSignal)
	    + _all_states[fromSignal]->transition()->log_probability_of(explicitId) + duration_prob
	    + emission
	    + _all_states[explicitId]->transition()->log_probability_of(toSignal) ;




#if 1
	  if(emission > 0) {
	  std::cerr << i << " " ;
	  std::cerr << _all_states[fromSignal]->name() << "->";
	  std::cerr << _all_states[explicitId]->name() << "->";
	  std::cerr << _all_states[toSignal]->name() << ": ";
	  std::cerr << "begin = " << begin  << ", ";
	  std::cerr << "end = " << end  << ", ";
	  std::cerr << "length = " << duration << ", ";
	  std::cerr << "duration=" << duration_prob << ", ";
	  std::cerr << _all_states[fromSignal]->name() << "->";
	  std::cerr << _all_states[explicitId]->name() << " = " << exp(_all_states[fromSignal]->transition()->log_probability_of(explicitId)) << ", ";
	  std::cerr << _all_states[explicitId]->name() << "->"; 
	  std::cerr << _all_states[toSignal]->name() << " = " << exp(_all_states[explicitId]->transition()->log_probability_of(toSignal)) << ", ";
	  std::cerr << "emission=" << emission  << ", "; 
	  std::cerr << "gamma =" << gamma(begin -1, fromSignal)  << ", "; 
	  std::cerr << "OPT =" << (v > gamma(i, toSignal))  << std::endl;
	  }
#endif
	  assert(emission <= 0);
	  if (v > gamma(i, toSignal)) {
	    gamma(i, toSignal) = v;
	    ptr[i][toSignal] = OptimalPredecessorPtr(
						     new OptimalPredecessor(fromSignal, explicitId, explicitId, begin - 1, begin + duration - 1));
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
  Matrix psilen(nstates, size);

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
  int L = s.size();
  int nstates = _all_states.size();
  initialize_prefix_sum_arrays(s);
  std::vector<std::set<CandidateSignalPtr> > signal_predecessors;
  signal_predecessors.resize(nstates);
  int max_signal_length  = 0;
  for (int i = 0; i < (int) _signal_states.size(); i++) 
    if(max_signal_length < _signal_states[i]->size())
      max_signal_length = _signal_states[i]->size();

  int M = L + max_signal_length;

  Matrix gamma(M+1, nstates + 1);
  
  std::vector<std::vector<OptimalPredecessorPtr> > ptr;
  ptr.resize(M+1);
  for (int i = 0; i <= M; i++)
    ptr[i].resize(nstates + 1);
  for (int i = 0; i < nstates; i++)
    for (int k = 0; k <= M; k++)
      gamma(k, i) = -HUGE;
  
  // initialization
  for (int i = 0; i < nstates; i++) {
    gamma(0, i) = getInitialProbabilities()->log_probability_of(i)
      + _all_states[i]->observation()->prefix_sum_array_compute(0, 0);
    ptr[0][i] = OptimalPredecessorPtr(new OptimalPredecessor(0, -1, -1, -1, -1));
  }
  
  for (int i = 1; i <= L; i++) {
    findBestPredecessorGeometricStates(i, gamma, ptr, s);
    findBestPredecessorSignalStates(i, gamma, ptr, s,signal_predecessors);
  }
  for(int i = L+1; i <= M; i++){
    findBestPredecessorGeometricStates(i, gamma, ptr, s);
    findBestPredecessorSignalStatesFinish(i, gamma, ptr, s, signal_predecessors);
  }

#if 0
  for (int i = 0; i <= M; i++) {
    if (i < (int) s.size())
      std::cerr << i << " " << alphabet()->getSymbol(s[i])->name() << ":"
		<< std::endl;
    else
      std::cerr << i << ":" << std::endl;
    for (int k = 0; k < (int) _all_states.size(); k++) {
      if(!_all_states[k]->isGeometricDuration()) 
	continue;
      std::cerr << " " << _state_names->getSymbol(k)->name() << ":"
		<< gamma(i, k);
      if (ptr[i][k] != NULL) {
	std::cerr << " "
		  << _state_names->getSymbol(ptr[i][k]->from())->name()
		  << " " << ptr[i][k]->nextExplicitStateId() << " "
		  << ptr[i][k]->begin() << std::endl;
      } else
	std::cerr << std::endl;
    }
  }
#endif
  
  //backtracing
  g = gamma;
  double max = -HUGE;
  int i = L;
  path.resize(M+1);
  for(int m = L; m <= M; m++) {
    for (int k = 0; k < nstates; k++) {
      if (ptr[m][k] != NULL)
	if (max < gamma(m, k)) {
	  max = gamma(m, k);
	  path[m] = k;
	  i = m;
	}
    }
  }

  if(ptr[i][path[i]] == NULL)
    {
      std::cerr << "ERROR: This sequence is not from this model !" << std::endl;
      exit(-1);
    }
// Backtracing
  while (i > 0) {
    
    int begin = ptr[i][path[i]]->begin();
    int prev = ptr[i][path[i]]->from();
    int state = ptr[i][path[i]]->nextExplicitStateId();
    if (state > -1) {
      int signal_size = _all_states[path[i]]->chooseDuration();
      for (int k = begin + 1; k < i - signal_size + 1; k++)
	path[k] = state;
      for (int k = i - signal_size + 1; k <= i; k++)
	path[k] = path[i];
      path[begin] = prev;
    } else {
      for (int k = begin + 1; k < i; k++)
	path[k] = path[i];
      path[begin] = prev;
    }
    i = begin;
  }
  path.resize(L);
  return max;
  
}

//! Posterior Probabilities: P(yi=k|x)
void GeneralizedHiddenMarkovModel::posteriorProbabilities (const Sequence &s, Matrix & probabilities) const{
  std::cerr << "posteriorProbabilities not implemented" << std::endl;
}

//! Posterior Decoding: ^yi = argmax_k P(yi=k|x)
void GeneralizedHiddenMarkovModel::posteriorDecoding (const Sequence &s, Sequence &path, Matrix & probabilities) const{
  std::cerr << "posteriorDecoding not implemented" << std::endl;
}

//! Choose the observation given a state
Sequence & GeneralizedHiddenMarkovModel::chooseObservation(Sequence & h, int i,
		int state) const {
  assert(state < (int)_all_states.size());
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
    std::map <std::string, ProbabilisticModelPtr> ::const_iterator it;
    
    for(it = _models.begin(); it != _models.end(); it++) 
      {
	out << it->first << " = [ "  << (it->second)->str() << " ] " << std::endl;
      }
    
    for (int i = 0; i < nstates; i++)
      out << _all_states[i]->str();
    
    return out.str();
    
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
    
    double sum = 0.0;
    std::vector <double> probs;
    probs.resize(nstates);
    for(int i = 0; i < nstates; i++){
      probs[i] = exp(_initial_probabilities->log_probability_of(i));
      sum += probs[i];
    }
    std::map <std::string, double> initial;
    std::stringstream out5;
    out5 << getStateName(0);
    initial[out5.str()] =  probs[0]/sum;
    for(int i = 0; i < nstates; i++)
      for(int j = 0; j < (int)alphabet()->size(); j++)
	if((i != 0) || (j != 0)){
	  std::stringstream out6;
	  out6 << getStateName(i) ;
	  initial[out6.str()] = probs[i]/sum;
	}
    answer.add("initial_probabilities", DoubleMapParameterValuePtr(new DoubleMapParameterValue(initial)));
    
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

    std::vector<std::string> state_names = state_names_par->getStringVector();
    AlphabetPtr states = AlphabetPtr(new Alphabet());
    states->initializeFromVector(state_names);
    setStateNames(states);
    AlphabetPtr observation_symbols = AlphabetPtr(new Alphabet());
    observation_symbols->initializeFromVector(observation_symbols_par->getStringVector());

    MultinomialDistributionPtr pi = MultinomialDistributionPtr(new MultinomialDistribution());
    pi->initializeFromMap(initial_probabilities_par->getDoubleMap(), states);
    
    std::map<std::string, double> transpar = transitions_par->getDoubleMap();
    std::map<std::string, DoubleVector> trans;
    std::map<std::string, double>::const_iterator it;

    for (it = transpar.begin(); it != transpar.end(); it++) {
      std::vector<std::string> splited;
      boost::regex separator("\\|");
      split_regex(it->first, splited, separator);
      
      if(splited.size() == 1) {
	splited.push_back("");
      }
      std::string from (splited[1]);
      std::string to ( splited[0]);
	
      
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
    for (int i = 0; i < (int) state_names.size(); i++) {
      trim_spaces(state_names[i]);
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
	ProbabilisticModelParameterValuePtr nullmodelpar =
	  statepars->getOptionalParameterValue("null_model");
	ProbabilisticModelParameterValuePtr thresholdpar =
	  statepars->getOptionalParameterValue("threshold");
	ProbabilisticModelParameterValuePtr durationpar =
	  statepars->getOptionalParameterValue("duration");
	ProbabilisticModelParameterValuePtr lengthpar =
	  statepars->getOptionalParameterValue("sequence_length");
	ProbabilisticModelParameterValuePtr outputphasepar =
	  statepars->getOptionalParameterValue("output_phase");
	ProbabilisticModelParameterValuePtr inputphasepar =
	  statepars->getOptionalParameterValue("input_phase");


	ProbabilisticModelParameterValuePtr extend_emission_par =
	  statepars->getOptionalParameterValue("extend_emission");
	ProbabilisticModelParameterValuePtr start_extend_emission_par =
	  statepars->getOptionalParameterValue("start");
	ProbabilisticModelParameterValuePtr stop_extend_emission_par =
	  statepars->getOptionalParameterValue("stop");
	ProbabilisticModelParameterValuePtr l_joinable_par =
	  statepars->getOptionalParameterValue("left_joinable");
	ProbabilisticModelParameterValuePtr r_joinable_par =
	  statepars->getOptionalParameterValue("right_joinable");




	if (observationpar == NULL) {
	  std::cerr << "ERROR: Missing  observation model for state "
		    << state_names[i] << std::endl;
	  return;
	}
	
	std::string model_name = observationpar->getString();
	restore_model(model_name, parameters);
	
	MultinomialDistributionPtr transition =
	  MultinomialDistributionPtr(
					new MultinomialDistribution(
								       trans[state_names[i]]));
	
	int iphase = -1;
	int ophase = -1;
	if(inputphasepar != NULL)
	  iphase = inputphasepar->getInt();
	if(outputphasepar != NULL)
	  ophase = outputphasepar->getInt();
	if (durationpar == NULL) {
	  if (nullmodelpar == NULL) {
	    
	    configureGeometricDurationState(model_name, transition, state_names[i], iphase, ophase);
	  } else {
	    if (thresholdpar == NULL) {
	      std::cerr
		<< "ERROR: missing threshold parameter of the state : "
		<< state_names[i] << std::endl;
	      return;
	    }
	    if (lengthpar == NULL) {
	      std::cerr
		<< "ERROR: missing sequence_length parameter of the state : "
		<< state_names[i] << std::endl;
	      return ;
	    }
	    // Signal state
	    std::string null_model_name = nullmodelpar->getString();
	    restore_model(null_model_name,  parameters);
	    configureSignalState(model_name,
				 null_model_name, transition,
				 thresholdpar->getDouble(), lengthpar->getDouble(),
				 state_names[i], iphase, ophase);
	  }
	} else {
	  
	  // Explicit duration state
	  std::string duration_model_name = durationpar->getString();
	  restore_model(duration_model_name,  parameters);
	  int l_joinable = 0;
	  int r_joinable = 0;
	  if(l_joinable_par != NULL)
	    l_joinable = l_joinable_par->getInt();
	  if(r_joinable_par != NULL)
	    r_joinable = r_joinable_par->getInt();
	  
	  if(extend_emission_par == NULL) {
	    configureExplicitDurationState(model_name, transition, duration_model_name, state_names[i], iphase, ophase, 0, 0, l_joinable, r_joinable);
	  } else {
	    int start = start_extend_emission_par->getInt();
	    int stop = stop_extend_emission_par->getInt();
	    configureExplicitDurationState(model_name, transition, duration_model_name, state_names[i], iphase, ophase, start, stop, l_joinable, r_joinable);
	  }
	  
	}
	
      }
      
    }
    setObservationSymbols(observation_symbols);
    setInitialProbability(pi);
    fixStatesPredecessorSuccessor();
    

  }
  
}

