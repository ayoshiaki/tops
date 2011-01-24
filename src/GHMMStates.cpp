/*
 *       GHMMStates.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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
#include "MultinomialDistribution.hpp"
#include "Symbol.hpp"
namespace tops{
    GHMMState::GHMMState() {
        _start = 0;
        _stop = 0;
        isRightJoinable(0);
        isLeftJoinable(0);
    }

    GHMMState:: ~GHMMState() {
    }

    void GHMMState::findBestPredecessor (Matrix & gamma, Matrix &psi, Matrix &psilen, const Sequence & s, int base, const GHMMStates & all_states){
        int d = 1;
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

        if(gamma(id(), base) < gmax){
            gamma(id(), base) = gmax;
            psi(id(), base) = pmax;
            psilen(id(), base) = d;
        }
    }
    void GHMMState::observationModelName(std::string name) {
        _observationModelName = name;
    }
  void GHMMState::durationModelName(std::string name) {

  }

  void GHMMState::nullModelName(std::string name) {
  }
  std::string GHMMState::observationModelName() const {
    return _observationModelName;
  }
  std::string GHMMState::durationModelName() const {
    std::string n;
    return n;
  }
  std::string GHMMState::nullModelName() const {
    std::string n;
    return n;
  }



  void GHMMState::setObservation(ProbabilisticModelPtr obs) {
    _observation = obs;
  }
  ProbabilisticModelPtr GHMMState::observation() const {
    return _observation;
  }
  void GHMMState::setTransition(MultinomialDistributionPtr trans) {
    _transition = trans;
  }
  MultinomialDistributionPtr GHMMState::transition() const {
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
  ProbabilisticModelPtr GHMMState::nullModel()  const {
    ProbabilisticModelPtr m;
    return m;
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
  void GHMMSignalState::setNullModel(ProbabilisticModelPtr n) {
    _nullModel = n;
  }
  ProbabilisticModelPtr GHMMSignalState::nullModel() const {
    return _nullModel;
  }

  void GHMMSignalState::nullModelName(std::string name) {
    _nullModelName = name;
  }
  std::string GHMMSignalState::nullModelName() const{
    return  _nullModelName;
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
    out << "null_model = " << nullModelName() << std::endl;
    out << "threshold = " << _threshold << std::endl;
    out << "sequence_length = " << _size << "]" << std::endl;

    return out.str();
  }

  ProbabilisticModelParameters GHMMSignalState::parameters() const{
    ProbabilisticModelParameters answer;
    answer.add("observation", StringParameterValuePtr(new StringParameterValue(GHMMState::observationModelName())));
    answer.add("null_model", StringParameterValuePtr(new StringParameterValue(nullModelName())));
    answer.add("threshold", DoubleParameterValuePtr(new DoubleParameterValue(_threshold)));
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



    void GHMMSignalState::findBestPredecessor (Matrix & gamma, Matrix &psi, Matrix &psilen, const Sequence & s, int base, const GHMMStates & all_states){
        int d = size();
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
#if 0
        std::cerr << "id: " << id() << " " << base - d + 1 << " " << base << std::endl;
        std::cerr << " duration: " << duration_probability(d) << std::endl;
        std::cerr << " emission: " << observation()->prefix_sum_array_compute(base-d+1, base , phase) << std::endl;
        std::cerr << " from: " << from << std::endl;
        std::cerr << " gamma: " << gamma(from, base-d) << std::endl;
        std::cerr << " duration: " << d << std::endl;
        std::cerr << " gmax: " << gmax << std::endl;
#endif
        if(gamma(id(), base) < gmax){
            gamma(id(), base) = gmax;
            psi(id(), base) = pmax;
            psilen(id(), base) = d;

        }
    }

  void GHMMSignalState::fixTransitionDistribution () const {
    MultinomialDistributionPtr trans = transition();
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

    void GHMMExplicitDurationState::findBestPredecessor (Matrix & gamma, Matrix &psi, Matrix &psilen, const Sequence & s, int base, const GHMMStates & all_states){
        int diff = 0;
        if(_number_of_phases  > 1)
            diff = mod(getOutputPhase() - getInputPhase(),_number_of_phases);
        if(_number_of_phases <= 0)
            _number_of_phases = 1;
        int offset = duration()->size();

        if(offset > 15000)
            offset = 15000;
        int minbase = (base - diff - 15000) ;

        if(minbase < 0) minbase = 0;
        for (int d = base - diff; d > minbase; d-=_number_of_phases)
            {
                int from = predecessors()[0];
                if((base - d ) < 0)
                    return;
                double gmax = gamma(from, d-1) + all_states[from]->transition()->log_probability_of(id());
                int pmax = from;
                for (int p = 1; p < (int)predecessors().size();p++){
                    int from = predecessors()[p];
                    double g = gamma(from, d-1) + all_states[from]->transition()->log_probability_of(id());
                    if(gmax < g){
                        gmax = g;
                        pmax = from;
                    }
                }
                int phase = getInputPhase();

                double emission = observation()->prefix_sum_array_compute(d, base, phase);

                if(emission <= -HUGE)
                    return;

#if 0
        std::cerr << name() << " " << d  << " " << base << std::endl;
        std::cerr << "iphase: " << getInputPhase() << std::endl;
        std::cerr << "ophase: " << getOutputPhase() << std::endl;
        std::cerr << " duration: " << duration_probability(base-d+1) << std::endl;
        std::cerr << " emission: " << observation()->prefix_sum_array_compute(d, base , phase) << std::endl;
        std::cerr << " number_of_phases: " << _number_of_phases << std::endl;
        std::cerr << " from: " << from << std::endl;
        std::cerr << " gamma: " << gamma(from, d-1) << std::endl;
        std::cerr << " d: " << base-d+1 << std::endl;
        std::cerr << " gmax: " << gmax << std::endl;
#endif
                gmax = gmax + duration_probability(base-d+1) + observation()->prefix_sum_array_compute(d, base, phase);
                if(gamma(id(), base) < gmax){
                    gamma(id(), base) = gmax;
                    psi(id(), base) = pmax;
                    psilen(id(), base) = base-d+1;
                }
            }
    }
  void GHMMExplicitDurationState::fixTransitionDistribution () const {
    MultinomialDistributionPtr trans = transition();
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
