#include "GHMMStates.hpp"
#include "ProbabilisticModel.hpp"
#include "FiniteDiscreteDistribution.hpp"
#include "Symbol.hpp" 
namespace tops{
  GHMMState::GHMMState() {
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
      out << "duration = " << durationModelName() << "]" << std::endl;
      
      return out.str();
    }

  ProbabilisticModelParameters GHMMExplicitDurationState::parameters() const {
      ProbabilisticModelParameters answer;
      answer.add("observation" , StringParameterValuePtr(new StringParameterValue(GHMMState::observationModelName())));
      answer.add("duration", StringParameterValuePtr(new StringParameterValue(durationModelName())));
       return answer;
    }

}
