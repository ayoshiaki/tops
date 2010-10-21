#include "GHMMStates.hpp"
#include "ProbabilisticModel.hpp"
#include "FiniteDiscreteDistribution.hpp"
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
  void GHMMState::setTransition(FiniteDiscreteDistributionPtr trans) {
    _transition = trans;
  }
  FiniteDiscreteDistributionPtr GHMMState::transition() const {
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


  void GHMMSignalState::fixTransitionDistribution () const {
    FiniteDiscreteDistributionPtr trans = transition();
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

  void GHMMExplicitDurationState::fixTransitionDistribution () const {
    FiniteDiscreteDistributionPtr trans = transition();
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
