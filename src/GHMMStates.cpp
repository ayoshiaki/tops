/*
 *       GHMMStates.cpp
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

#include "GHMMStates.hpp"
#include "ProbabilisticModel.hpp"
#include "DiscreteIIDModel.hpp"
#include "InhomogeneousFactorableModel.hpp"
#include "Symbol.hpp"
#include <list>

namespace tops{
    GHMMState::GHMMState() {
        _start = 0;
        _stop = 0;
        isRightJoinable(0);
        isLeftJoinable(0);
    }

    GHMMState:: ~GHMMState() {
    }

    void GHMMState::findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions){
        int d = 1;

	gamma(id(), base) = -HUGE;
	psi(id(), base) = 0;
	psilen(id(), base) = 0;
        if(predecessors().size() <= 0)
            return;

        int from = _predecessors[0];
        double gmax = gamma(from, base-d) + all_states[from]->transition()->log_probability_of(id());
        int pmax = from;
        for (int p = 1; p < (int)_predecessors.size();p++){
            int from = _predecessors[p];
            double g = gamma(from, base-d) + all_states[from]->transition()->log_probability_of(id());
            if(gmax < g){
                gmax = g;
                pmax = from;
            }
        }
        int phase = getInputPhase();
        gmax = gmax + duration_probability(d) + observation()->prefix_sum_array_compute(base-d +1, base, phase);

	if(gmax > gamma(id(), base)) {
	  gamma(id(), base) = gmax;
	  psi(id(), base) = pmax;
	  psilen(id(), base) = d;
	}

    }

    void GHMMState::choosePredecessor (Matrix & alpha, int base, int & state, int & position, const GHMMStates & all_states) {
        double sum = 0;


        double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
#if 0
        std::cerr << base << " " << std::endl;
        for (int k = 0; k < (int)predecessors().size(); k++)
            {
                int p = predecessors()[k];
                std::cerr << " " << all_states[p]->name () << " " << base - 1 << " " << exp(alpha(p, base - 1) + all_states[p]->transition()->log_probability_of(id()) + all_states[id()]->observation()->prefix_sum_array_compute(base, base, all_states[id()]->getInputPhase()) - alpha(id(), base)) << " "  << exp(alpha(p, base -1 )) << std::endl;

            }
#endif
        for (int k = 0; k < (int)predecessors().size(); k++)
            {
                int choosed = predecessors()[k];
                sum += exp(alpha(choosed, base - 1) + all_states[choosed]->transition()->log_probability_of(id()) + observation()->prefix_sum_array_compute(base, base, getInputPhase()) - alpha(id(), base));
                if(sum >= random){
                    state = choosed;
                    position = base - 1;
                    return;
                }
            }
    }


    void GHMMState::observationModelName(std::string name) {
        _observationModelName = name;
    }
  void GHMMState::durationModelName(std::string name) {

  }

  std::string GHMMState::observationModelName() const {
    return _observationModelName;
  }
  std::string GHMMState::durationModelName() const {
    std::string n;
    return n;
  }




  void GHMMState::setObservation(ProbabilisticModelPtr obs) {
    _observation = obs;
  }
  ProbabilisticModelPtr GHMMState::observation() const {
    return _observation;
  }
  void GHMMState::setTransition(DiscreteIIDModelPtr trans) {
    _transition = trans;
  }
  DiscreteIIDModelPtr GHMMState::transition() const {
    return _transition;
  }
  int GHMMState::chooseDuration() const {
    return 1;
  }
  std::string GHMMState::name() const {
    return _name->name();
  }
  int GHMMState::id() const {
    return _name->id();
  }
  void GHMMState::addPredecessor(int id) {
    _predecessors.push_back(id);
  }
  std::vector<int> & GHMMState::predecessors() {
    return _predecessors;
  }
  void GHMMState::addSuccessor(int id) {
    _successors.push_back(id);
  }
  std::vector<int> & GHMMState::successors() {
    return _successors;
  }
  std::vector<int> & GHMMState::classes() {
    return _classes;
  }
  double GHMMState::duration_probability(int l) const {
    if (l == 1)
      return 0.0;
    else
      return -HUGE;
  }
  bool GHMMState::isGeometricDuration() const {
    return true;
  }
  std::string GHMMState::str() const {
    std::stringstream out;
    out << name() << " = [ observation = " << observationModelName() << "]" << std::endl;
    return out.str();
  }

  ProbabilisticModelParameters GHMMState::parameters() const{
    ProbabilisticModelParameters answer;
    answer.add("observation", StringParameterValuePtr(new StringParameterValue(observationModelName())));
    return answer;
  }

  int GHMMState::getInputPhase() const {
    return _inputPhase;
  }

  void GHMMState::setInputPhase(int _inputPhase) {
    this->_inputPhase = _inputPhase;
  }


  int GHMMState::getStart() const {
    return _start;
  }

  void GHMMState::setStart(int start) {
    this->_start = start;
  }

  void GHMMState::setClasses(std::vector<int> &classes){
    _classes = classes;
  }

  int GHMMState::getStop() const {
    return _stop;
  }

  void GHMMState::setStop(int stop) {
    this->_stop = stop;
  }

    int GHMMState::getOutputPhase() const {
    return _outputPhase;
  }

  void GHMMState::setOutputPhase(int _outputPhase) {
    this->_outputPhase = _outputPhase;
  }



  GHMMSignalState::GHMMSignalState() {
    setStart(0);
    setStop(0);
    isRightJoinable(0);
    isLeftJoinable(0);
  }
  int GHMMSignalState::size() const {
    return _size;
  }
  void GHMMSignalState::setSize(int s) {
    _size = s;
  }


  bool GHMMSignalState::isGeometricDuration() const {
    return true;
  }




  int GHMMSignalState::chooseDuration() const {
    return _size;
  }
  double GHMMSignalState::getThreshold() const {
    return _threshold;
  }
  void GHMMSignalState::setThreshold(double threshold) {
    _threshold = threshold;
  }
  double GHMMSignalState::duration_probability(int l) const {
    if (l == _size)
        return 0.0;
    else
      return -HUGE;
  }
  std::string GHMMSignalState::str() const {
    std::stringstream out;
    out << name() << " = [\n observation = " << GHMMState::observationModelName() << std::endl;
    out << "sequence_length = " << _size << "]" << std::endl;

    return out.str();
  }

  ProbabilisticModelParameters GHMMSignalState::parameters() const{
    ProbabilisticModelParameters answer;
    answer.add("observation", StringParameterValuePtr(new StringParameterValue(GHMMState::observationModelName())));
    answer.add("sequence_length", IntParameterValuePtr(new IntParameterValue(_size)));
    return answer;
  }


  GHMMExplicitDurationState::~GHMMExplicitDurationState() {
  }
  GHMMExplicitDurationState::GHMMExplicitDurationState() {
    setStart(0);
    setStop(0);
  }
  void GHMMExplicitDurationState::durationModelName(std::string name){
    _durationModelName = name;
  }
  std::string GHMMExplicitDurationState::durationModelName() const {
    return _durationModelName;
  }

  void GHMMExplicitDurationState::setDuration(ProbabilisticModelPtr d) {
      _duration = d;
      ProbabilisticModelParameters p = d->parameters();
      ProbabilisticModelParameterValuePtr par = p.getOptionalParameterValue("number_of_phases");
      if(par != NULL)
          _number_of_phases = par->getInt();
      else
          _number_of_phases = 1;
    }
   ProbabilisticModelPtr GHMMExplicitDurationState::duration() const {
     return _duration;
   }

    int GHMMExplicitDurationState::chooseDuration() const {
      return _duration->choose();
    }
     bool GHMMExplicitDurationState::isGeometricDuration() const {
      return false;
    }
    double GHMMExplicitDurationState::duration_probability(int l) const {
      return _duration->log_probability_of(l);
    }

    std::string GHMMExplicitDurationState::str() const {
      std::stringstream out;
      out << name() << " = [\n observation = " << GHMMState::observationModelName() << std::endl;
      if((getStart() > 0) || (getStop() > 0)) {
        out << "extend_emission = 1" << std::endl;
        out << "start = " << getStart() <<  std::endl;
        out << "stop = " << getStop() << std::endl;
      }
      if(isLeftJoinable()) {
        out << "left_joinable = " << isLeftJoinable() << std::endl;
      }
      if(isRightJoinable()) {
        out << "right_joinable = " << isRightJoinable() << std::endl;
      }

      out << "duration = " << durationModelName() << "]" << std::endl;

      return out.str();
    }

  ProbabilisticModelParameters GHMMExplicitDurationState::parameters() const {
      ProbabilisticModelParameters answer;
      answer.add("observation" , StringParameterValuePtr(new StringParameterValue(GHMMState::observationModelName())));
      answer.add("duration", StringParameterValuePtr(new StringParameterValue(durationModelName())));
      return answer;
    }
  void GHMMState::isLeftJoinable(int joinable){
    this->_left_joinable = joinable;
  }
  int GHMMState::isLeftJoinable() const {
    return this->_left_joinable;
  }

  void GHMMState::isRightJoinable(int joinable){
    this->_right_joinable = joinable;
  }
  int GHMMState::isRightJoinable() const {
    return this->_right_joinable;
  }



    void GHMMSignalState::findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions){
        int d = size();

	gamma(id(), base) = -HUGE;
	psi(id(), base) = 0;
	psilen(id(), base) = 0;
        if(predecessors().size() <= 0)
            return;

        int from = predecessors()[0];
        if((base - d ) < 0)
            return;
        double gmax = gamma(from, base-d) + all_states[from]->transition()->log_probability_of(id());
        int pmax = from;
        for (int p = 1; p < (int)predecessors().size();p++){
            int from = predecessors()[p];
            double g = gamma(from, base-d) + all_states[from]->transition()->log_probability_of(id());
            if(gmax < g){
                gmax = g;
                pmax = from;
            }
        }
        int phase = getInputPhase();
	gmax = gmax + duration_probability(d) + observation()->prefix_sum_array_compute(base-d +1, base, phase);
	if(gmax > gamma(id(), base)) {
	  gamma(id(), base) = gmax;
	  psi(id(), base) = pmax;
	  psilen(id(), base) = d;
	}
    }




    void GHMMSignalState::choosePredecessor (Matrix & alpha, int base, int & state, int & position, const GHMMStates & all_states) {
        double sum = 0;
        double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
        position = base - size() ;
        if(position < 0)
            position = 0;
        for(int k  = 0; k < (int)predecessors().size(); k++)
            {
                int choosed = predecessors()[k];
                sum += exp(alpha(choosed, position) + all_states[choosed]->transition()->log_probability_of(id()) + observation()->prefix_sum_array_compute(position + 1, base, getInputPhase())  - alpha(id(), base)+ duration_probability(base - position) ) ;
                if(sum >= random){
                    state = choosed;
                    return;
                }
            }
    }




  void GHMMSignalState::fixTransitionDistribution () const {
    DiscreteIIDModelPtr trans = transition();
    DoubleVector probabilities = (trans->parameters()).getMandatoryParameterValue("probabilities")->getDoubleVector();
    int j = id();
    if(probabilities.size() <= 0) {
      return;
    }
    probabilities[j] = 0.0;
    double sum = 0.0;
    for(int i = 0; i < (int)probabilities.size(); i++)
      {
          if (i == j)
              continue;
          sum += probabilities[i];
      }
    for(int i = 0; i < (int)probabilities.size(); i++)
      {
          if (i == j)
              continue;
          probabilities[i]  = probabilities[i]/sum;
      }
    trans->setProbabilities(probabilities);
  }

    void GHMMExplicitDurationState::choosePredecessor (Matrix & alpha, int base, int & state, int & position, const GHMMStates & all_states) {
        double sum = 0;
        double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
        int diff  = 0;
        if(_number_of_phases  > 1)
            diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
        if(_number_of_phases <= 0)
            _number_of_phases = 1;
        int offset = duration()->size();

        if(offset > 15000)
            offset = 15000;
        int minbase = (base - diff - offset) ;
        if(minbase < 0) minbase = 0;

        for (int d = base - diff; d > minbase; d-=_number_of_phases)
            {
                position = d - 1;
                for(int k  = 0; k < (int)predecessors().size(); k++)
                    {
                        int choosed = predecessors()[k];
                        sum += exp(alpha(choosed, position) + all_states[choosed]->transition()->log_probability_of(id()) + observation()->prefix_sum_array_compute(d, base, getInputPhase())  - alpha(id(), base) + duration_probability(base - d + 1 ) ) ;
                        if(sum >= random){
                            state = choosed;
                            return;
                        }
                    }
            }
    }

    void GHMMExplicitDurationState::findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions){
      int diff = 0;
      if(_number_of_phases  > 1)
	diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
      if(_number_of_phases <= 0)
	_number_of_phases = 1;
      int offset = duration()->size();
      if(offset > 15000)
	offset = 15000;

	gamma(id(), base) = -HUGE;
	psi(id(), base) = 0;
	psilen(id(), base) = 0;
#if 0
      bool toContinue = false;
      for(int suc = 0; suc < (int)successors().size(); suc++)
      {
	if(!all_states[successors()[suc]]->isGeometricDuration()){
	  toContinue = true;
	  break;
	}
      }
      if(base == s.size()-1)
	toContinue = true;

      if(!toContinue){
	return;
      }

#endif 
      std::list<int>::iterator it;
      it =  (valid_positions.find(id())->second).begin();
      //std::cerr << base << " " << all_states[id()]->name() << " " << (valid_positions.find(id())->second).size() << std::endl;
      while(it != (valid_positions.find(id())->second).end()) {
	int d = (*it) + 1;
	int from = id();
	if (d > 1) {
	  if(predecessors().size() <= 0)
	    return;
	   from = predecessors()[0];
	  if((base - d ) < 0)
	    return;
	  if(base-d+1 >= offset)
	  {
	    it = (valid_positions.find(id())->second).erase(it);
	    continue;
	  }
	}

	double dur = duration_probability(base - d + 1);
	if(base == s.size()-1) 
	  for (int aux = 0; aux <= 10; aux++)
	    dur = log_sum(dur, duration_probability(base - d + 1 + aux));
	if( dur <= -HUGE) {
	  it++;
	  continue;
	}
        	

	double emission = observation()->prefix_sum_array_compute(d, base, getInputPhase());
	if(emission <= -HUGE) {
	  it = (valid_positions.find(id())->second).erase(it);
	  continue;
	}

	// check if it can emmit the current state given the boundaries
	int nphase = getInputPhase();
	if(observation()->inhomogeneous() != NULL)
	  nphase = observation()->inhomogeneous()->maximumTimeValue() + 1;
	if(getStart() > 0 && getStop() > 0) {
	  if((d-getStart() >= 0) && (base + getStop () < s.size())) {
	    double joinable = observation()->prefix_sum_array_compute(d-getStart(),base+getStop(), mod(getInputPhase()-getStart(), nphase));
	    //std::cerr << all_states[id()]->name() << " " << base << " " << d-getStart() << " " << base + getStop() << " " << getInputPhase() << " " << mod(getInputPhase() - getStart(), nphase) << " " << nphase << std::endl;
	    if(joinable <= -HUGE) {
	      it++;
	      continue;
	    }
	  }
	}

	double gmax ;
	if (d == 0) 
	  gmax = 0;
	else 
	  gmax = gamma(from, d - 1) + all_states[from]->transition()->log_probability_of(id());

	int pmax = from;
	for (int p = 1; p < (int)predecessors().size();p++){
	  int from = predecessors()[p];
	  double g = gamma(from, d-1) + all_states[from]->transition()->log_probability_of(id());
	  if(gmax < g){
	    gmax = g;
	    pmax = from;
	  }
	}

	gmax = gmax + dur + observation()->prefix_sum_array_compute(d, base, getInputPhase()); 

	if (gmax > gamma(id(), base)) {
	  gamma(id(), base) = gmax;
	  psi(id(), base) = pmax;
	  psilen(id(), base) = base-d+1;
	  assert (base - d + 1 >= 0);
	  assert (base - d + 1 <= s.size());
	}

	it++;
      }
    }

  double GHMMState::backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions){
    int phase = getInputPhase();
    double result = _observation->prefix_sum_array_compute(base+1, base+1, phase) + beta(id(), base+1);
    return result;
  }

  double GHMMSignalState::backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions){
    int d = size();
    if(base+d >= (int)s.size())
      return -HUGE;
    int phase = getInputPhase();
    double result = _observation->prefix_sum_array_compute(base+1,base+d,phase) + beta(id(),base+d);
    return result;
  }

  double GHMMExplicitDurationState::backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions){
    int diff = 0;
    if(_number_of_phases  > 1)
      diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
    if(_number_of_phases <= 0)
      _number_of_phases = 1;
    int offset = _duration->size();
    if(offset > 15000)
      offset = 15000;
    int maxbase = (base + diff + offset) ;
    if(maxbase > (int)s.size()-1) maxbase = s.size()-1;
    int phase = getInputPhase();

    std::list<int>::iterator it;
    it = valid_positions[id()].begin();
    double sum = -HUGE;
    while(it != valid_positions[id()].end()){
      if((*it) > maxbase){
  it = valid_positions[id()].erase(it);
  continue;
      }
      if((*it) < base + diff){
  it++;
  continue;
      }
      double duration = duration_probability((*it)-base);
      if(duration <= -2e20){
  it++;
  continue;
      }
      if(_observation->inhomogeneous() != NULL)
  phase = _observation->inhomogeneous()->maximumTimeValue() + 1;
      if(getStart() > 0 && getStop() > 0) {
  if((base+1-getStart() >= 0) && ((*it) + getStop() < s.size())) {
    double joinable = _observation->prefix_sum_array_compute(base+1-getStart(),(*it)+getStop(), mod(getInputPhase()-getStart(), phase));
    if(joinable <= -2e20) {
      it = valid_positions[id()].erase(it);
      continue;
    }
  }
      }
      sum = log_sum(sum, _observation->prefix_sum_array_compute(base+1,(*it),getInputPhase()) + duration + beta(id(),(*it)));
      it++;
    }
    return sum;
  }

  void GHMMState::forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions){
    alpha(id(), base) = -HUGE;
    if(predecessors().size() <= 0)
      return;
    int from = predecessors()[0];
    int phase = getInputPhase();
    double emission = observation()->prefix_sum_array_compute(base, base, phase);
    alpha(id(), base) =  alpha(from, base-1) + all_states[from]->transition()->log_probability_of(id()) + emission;
    for (int k = 1; k < (int)predecessors().size(); k++)
      {
  from = predecessors()[k];
  alpha(id(), base) =  log_sum(alpha(from, base - 1) + all_states[from]->transition()->log_probability_of(id()) +  emission, alpha(id(), base));
      }
  }

  void GHMMSignalState::forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions){
    alpha(id(), base)  = -HUGE;
    int d = size();
    if(predecessors().size() <= 0)
      return;

    int from = predecessors()[0];
    if((base - d ) < 0)
      return;
    int phase = getInputPhase();
    double emission = observation()->prefix_sum_array_compute(base - d+ 1, base, phase);

    alpha(id(), base) =  alpha(from, base-d) + all_states[from]->transition()->log_probability_of(id())  + emission;
    for (int k = 1; k < (int)predecessors().size(); k++)
      {
  from = predecessors()[k];
  alpha(id(), base) =  log_sum(alpha(from, base -d) + all_states[from]->transition()->log_probability_of(id()) + emission, alpha(id(), base));
      }
    if(alpha(id(),base) > -HUGE){
      std::vector<int> succ = successors();
      for(int p = 0; p < (int)succ.size(); p++){
  int id = succ[p];
  if(!all_states[id]->isGeometricDuration())
    valid_positions[id].push_front(base);
      }
    }
  }


  void GHMMExplicitDurationState::forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions){
    alpha(id(), base) = -HUGE;
    if(predecessors().size() <= 0)
      return;
    int diff = 0;
    if(_number_of_phases  > 1)
      diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
    if(_number_of_phases <= 0)
      _number_of_phases = 1;
    int offset = duration()->size();
    if(offset > 15000)
      offset = 15000;
    int minbase = (base - diff - offset) ;
    if(minbase < 0) minbase = 0;
    int phase = getInputPhase();

    std::list<int>::iterator it;
    it = valid_positions[id()].begin();
    while(it != valid_positions[id()].end()){
      if((*it) < minbase){
  it = valid_positions[id()].erase(it);
  continue;
      }
      if((*it) > base - diff){
  it++;
  continue;
      }
      double duration = duration_probability(base-(*it));
      if(duration <= -HUGE){
  it++;
  continue;
      }
      if(observation()->inhomogeneous() != NULL)
  phase = observation()->inhomogeneous()->maximumTimeValue() + 1;
      if(getStart() > 0 && getStop() > 0) {
  if(((*it)+1-getStart() >= 0) && (base + getStop() < (int)s.size())) {
    double joinable = observation()->prefix_sum_array_compute((*it)+1-getStart(),base+getStop(), mod(getInputPhase()-getStart(), phase));
    if(joinable <= -HUGE) {
      it = valid_positions[id()].erase(it);
      continue;
    }
  }
      }
      int from = predecessors()[0];
      double emission = observation()->prefix_sum_array_compute((*it)+1, base, getInputPhase());
      alpha(id(), base) = log_sum(alpha(from, (*it)) + all_states[from]->transition()->log_probability_of(id()) + duration + emission, alpha(id(), base));
      for (int k = 1; k < (int)predecessors().size(); k++){
    from = predecessors()[k];
    alpha(id(), base) =  log_sum(alpha(from, (*it)) + all_states[from]->transition()->log_probability_of(id()) + duration + emission, alpha(id(), base));
      }
      it++;
    }
  }

  void GHMMState::posteriorSum (Matrix & alpha, Matrix & beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber){
    alpha(id(), base) = -HUGE;
    if(predecessors().size() <= 0)
      return;
    int phase = getInputPhase();
    double emission = _observation->prefix_sum_array_compute(base, base, phase);
    for (int k = 0; k < (int)predecessors().size(); k++)
      {
  int from = predecessors()[k];
  alpha(id(), base) =  log_sum(alpha(from, base - 1) + all_states[from]->transition()->log_probability_of(id()) +  emission, alpha(id(), base));
      }
    if(stateNumber == -1){
      for(int c = 0; c < (int)classes().size(); c++){
  postProbs(base,classes()[c]) += exp(alpha(id(),base) + beta(id(),base) - prob);
      }
    }
    else if(stateNumber != -1)
      postProbs(base,id()) = exp(alpha(id(),base) + beta(id(),base) - prob);
  }

  void GHMMSignalState::posteriorSum (Matrix & alpha, Matrix & beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber){
    alpha(id(), base)  = -HUGE;
    int d = size();
    if(predecessors().size() <= 0)
      return;

    if((base - d ) < 0)
      return;
    int phase = getInputPhase();
    double emission = _observation->prefix_sum_array_compute(base - d+ 1, base, phase);
    if(emission <= -HUGE)
      return;

    for (int k = 0; k < (int)predecessors().size(); k++)
      {
  int from = predecessors()[k];
  double w = alpha(from, base -d) + all_states[from]->transition()->log_probability_of(id()) + emission;
  alpha(id(), base) =  log_sum(w, alpha(id(), base));
  if(w > -HUGE && stateNumber == -1){
    int c = 0;
    float pp = exp(w + beta(id(),base) - prob);
    for(int i = base-d+1; i <= base; i++){
      postProbs(i,classes()[c]) += pp;
      c++;
      if(c == (int)classes().size())
        c = 0;
    }
  }
  else if(w > -HUGE && stateNumber != -1){
    float pp = exp(w + beta(id(),base) - prob);
    for(int i = base-d+1; i <= base; i++){
      postProbs(i,id()) += pp;
    }
  }
      }
    if(alpha(id(),base) > -HUGE){
      std::vector<int> succ = successors();
      for(int p = 0; p < (int)succ.size(); p++){
  int id = succ[p];
  if(!all_states[id]->isGeometricDuration())
    valid_positions[id].push_front(base);
      }
    }
  }

  void GHMMExplicitDurationState::posteriorSum (Matrix & alpha, Matrix &beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber){
    alpha(id(), base) = -HUGE;
    if(predecessors().size() <= 0)
      return;
    int diff = 0;
    if(_number_of_phases  > 1)
      diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
    if(_number_of_phases <= 0)
      _number_of_phases = 1;
    int offset = duration()->size();
    if(offset > 15000)
      offset = 15000;
    int minbase = (base - diff - offset) ;
    if(minbase < 0) minbase = 0;
    int phase = getInputPhase();

    std::list<int>::iterator it;
    it = valid_positions[id()].begin();
    while(it != valid_positions[id()].end()){
      if((*it) < minbase){
  it = valid_positions[id()].erase(it);
  continue;
      }
      if((*it) > base - diff){
  it++;
  continue;
      }
      double duration = duration_probability(base-(*it));
      if(duration <= -HUGE){
  it++;
  continue;
      }
      if(observation()->inhomogeneous() != NULL)
  phase = observation()->inhomogeneous()->maximumTimeValue() + 1;
      if(getStart() > 0 && getStop() > 0) {
  if(((*it)+1-getStart() >= 0) && (base + getStop() < (int)s.size())) {
    double joinable = observation()->prefix_sum_array_compute((*it)+1-getStart(),base+getStop(), mod(getInputPhase()-getStart(), phase));
    if(joinable <= -HUGE) {
      it = valid_positions[id()].erase(it);
      continue;
    }
  }
      }
      double emission = observation()->prefix_sum_array_compute((*it)+1, base, getInputPhase());
      for (int k = 0; k < (int)predecessors().size(); k++){
  int from = predecessors()[k];
  double w = alpha(from, (*it)) + all_states[from]->transition()->log_probability_of(id()) + duration + emission;
  alpha(id(), base) = log_sum(w, alpha(id(), base));
  if(w > -HUGE && stateNumber == -1){
    int c = 0;
    float pp = exp(w + beta(id(),base) - prob);
    for(int i = (*it)+1; i <= base; i++){
      postProbs(i,classes()[c]) += pp;
      c++;
      if(c == (int)classes().size())
        c = 0;
    }
  }
  else if(w > -HUGE && stateNumber != -1){
    float pp = exp(w + beta(id(),base) - prob);
    for(int i = (*it)+1; i <= base; i++){
      postProbs(i,id()) += pp;
    }
  }
      }
      it++;
    }
  }

  void GHMMExplicitDurationState::fixTransitionDistribution () const {
    DiscreteIIDModelPtr trans = transition();
    DoubleVector probabilities = (trans->parameters()).getMandatoryParameterValue("probabilities")->getDoubleVector();
    int j = id();
    if(probabilities.size() <= 0) {
      return;
    }
    probabilities[j] = 0.0;
    double sum = 0.0;
    for(int i = 0; i < (int)probabilities.size(); i++)
      {
        if (i == j)
          continue;
        sum += probabilities[i];
      }
    for(int i = 0; i <(int) probabilities.size(); i++)
      {
        if (i == j)
          continue;
        probabilities[i]  = probabilities[i]/sum;
      }
    trans->setProbabilities(probabilities);
  }
}
