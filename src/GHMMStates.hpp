#ifndef GHMM_STATES_HPP
#define GHMM_STATES_HPP
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>
#include "ProbabilisticModelParameter.hpp"
namespace tops {
  class ProbabilisticModel;
  typedef boost::shared_ptr <ProbabilisticModel> ProbabilisticModelPtr;

  class MultinomialDistribution;
  typedef boost::shared_ptr <MultinomialDistribution> MultinomialDistributionPtr;
  class Symbol;
  typedef boost::shared_ptr<Symbol> SymbolPtr;

  //! Represents a GHMM State
  class GHMMState {
  public:
    GHMMState();
    GHMMState(ProbabilisticModelPtr observation,
	      MultinomialDistributionPtr transition, SymbolPtr name) :
      _observation(observation), _transition(transition), _name(name){};
    virtual ~GHMMState();
    virtual void setObservation(ProbabilisticModelPtr obs) ;
    virtual ProbabilisticModelPtr observation() const ;
    virtual void setTransition(MultinomialDistributionPtr trans) ;
    virtual MultinomialDistributionPtr transition() const ;
    virtual int chooseDuration() const ;
    virtual std::string name() const ;
    virtual int id() const ;
    virtual void addPredecessor(int id) ;
    virtual std::vector<int> & predecessors() ;
    virtual void addSuccessor(int id) ;
    virtual void clearPredecessorSuccessor() {
      _successors.clear();
      _predecessors.clear();
    }
    virtual std::vector<int> & successors() ;
    virtual double duration_probability(int l) const ;
    virtual bool isGeometricDuration() const ;
    virtual std::string str() const ;
    virtual int getInputPhase() const ;
    virtual void setInputPhase(int _inputPhase) ;
    virtual int getOutputPhase() const ;
    virtual void setOutputPhase(int _outputPhase) ;

    virtual int getStart() const ;
    virtual void setStart(int start) ;
    virtual int getStop() const ;
    virtual void setStop(int stop) ;

    virtual void isLeftJoinable(int joinable);
    virtual int isLeftJoinable() const;
    
    virtual void isRightJoinable(int joinable);
    virtual int isRightJoinable() const;


    virtual ProbabilisticModelPtr nullModel()  const ;
    virtual void observationModelName(std::string name) ;
    virtual void durationModelName(std::string name) ;
    virtual void nullModelName(std::string name) ;
    virtual std::string observationModelName() const;
    virtual std::string durationModelName() const;
    virtual std::string nullModelName() const;
    virtual void fixTransitionDistribution () const {} ;
    virtual ProbabilisticModelParameters parameters() const;
  private:
    ProbabilisticModelPtr _observation;
    MultinomialDistributionPtr _transition;
    SymbolPtr _name;
    std::vector<int> _predecessors;
    std::vector<int> _successors;
    int _inputPhase; 
    int _outputPhase;
    int _start;
    int _stop;
    bool _left_joinable;
    bool _right_joinable;
    std::string _observationModelName;
  };
  //! GHMM signal states
  class GHMMSignalState: public GHMMState {
  public:
    GHMMSignalState() ;
    GHMMSignalState(ProbabilisticModelPtr observation,
		    MultinomialDistributionPtr transition, SymbolPtr name) :
      GHMMState(observation, transition, name) {};

    virtual int size() const ;
    virtual void setSize(int s);
    virtual void setNullModel(ProbabilisticModelPtr n) ;
    virtual ProbabilisticModelPtr nullModel() const ;
    virtual int chooseDuration() const ;
    virtual double getThreshold() const ;
    virtual void setThreshold(double threshold) ;
    virtual double duration_probability(int l) const ;
    virtual std::string str() const ;

    virtual void nullModelName(std::string name) ;
    virtual std::string nullModelName() const;
    virtual ProbabilisticModelParameters parameters() const;
    virtual void fixTransitionDistribution () const ;

  private:
    int _size;
    double _threshold;
    ProbabilisticModelPtr _nullModel;
    std::string _nullModelName;
    
  };
  //! GHMM Explicit duration state
  class GHMMExplicitDurationState: public GHMMState {
  public:
    virtual ~GHMMExplicitDurationState() ;

    GHMMExplicitDurationState() ;

    GHMMExplicitDurationState(ProbabilisticModelPtr observation,
			      MultinomialDistributionPtr transition, SymbolPtr name) :
      GHMMState(observation, transition, name) {};

    
    virtual void setDuration(ProbabilisticModelPtr d) ;
    virtual ProbabilisticModelPtr duration() const ;
    virtual int chooseDuration() const ;
    virtual bool isGeometricDuration() const ;
    virtual double duration_probability(int l) const ;
    virtual std::string str() const ;

    virtual void durationModelName(std::string name); 
    virtual std::string durationModelName() const ;
    virtual void fixTransitionDistribution () const;

    virtual ProbabilisticModelParameters parameters() const;
  private:
    ProbabilisticModelPtr _duration;
    std::string _durationModelName;
    
  };
  
  typedef boost::shared_ptr<GHMMState> GHMMStatePtr;
  typedef boost::shared_ptr<GHMMSignalState> GHMMSignalStatePtr;
  typedef boost::shared_ptr<GHMMExplicitDurationState>
  GHMMExplicitDurationStatePtr;

  typedef std::vector<GHMMStatePtr> GHMMStates;
  typedef std::vector<GHMMSignalStatePtr> GHMMSignalStates;
  typedef std::vector<GHMMExplicitDurationStatePtr> GHMMExplicitDurationStates;

}

#endif
