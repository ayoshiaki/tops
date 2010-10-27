#ifndef HIDDEN_MARKOV_MODEL_HPP
#define HIDDEN_MARKOV_MODEL_HPP


#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"

#include "HiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  class HMMState {
  private:
    int _id;
    SymbolPtr _name;
    MultinomialDistributionPtr _emission;
    MultinomialDistributionPtr _transitions;
  public:
    HMMState(){}
    HMMState (int id, SymbolPtr name, MultinomialDistributionPtr emission,  MultinomialDistributionPtr transitions) : _id(id), _name(name), _emission(emission), _transitions(transitions) {}
    void setName (SymbolPtr name) {
      _name = name;
    }
    void setEmissions (MultinomialDistributionPtr e)
    {
      _emission = e;
    }
    void setTransition (MultinomialDistributionPtr t) 
    {
      _transitions = t;
    }
    MultinomialDistributionPtr emission() {
      return _emission;
    }
    MultinomialDistributionPtr transitions() {
      return _transitions;
    }
    bool isSilent() {
      return (_emission == NULL);
    }
    SymbolPtr getName() const {
      return _name;
    }
    int getId() {
      return _id;
    }
    void setId(int i) {
      _id = i;
    }
      
    
  };
  typedef boost::shared_ptr <HMMState> HMMStatePtr;

  //! This is represents a hidden markov model
  class HiddenMarkovModel :   public DecodableModel
  {
  public:
    
    HiddenMarkovModel() {
    };

    HiddenMarkovModel( std::vector <HMMStatePtr> states, MultinomialDistributionPtr initial_probability, AlphabetPtr state_names, AlphabetPtr observation_symbols) :    _states(states) , _initial_probability(initial_probability), _state_names (state_names) {
      tops::ProbabilisticModel::setAlphabet(observation_symbols);


    }


    void setStates(std::vector<HMMStatePtr> states) {
      _states = states;
    }

    
    virtual ~HiddenMarkovModel(){}

    //! Choose the observation given a state 
    virtual Sequence &  chooseObservation ( Sequence & h,int i,  int state) const ;
    //! Choose a state
    virtual int chooseState(int state ) const ;
    //! Choose first state
    virtual int chooseFirstState() const ;
    virtual AlphabetPtr getStateNames() const {
      return _state_names;
    }
    virtual std::string getStateName(int state) const;
    
    virtual std::string str () const ;
    
    virtual void setState (int id, HMMStatePtr state)
    {
      if(_states.size() < _state_names->size()) 
	_states.resize(_state_names->size());
      _states[id] = state;
      state->setId(id);
    }

    virtual HMMStatePtr getState(int id) const 
    {
      return _states[id];
    }
    //! Forward algorithm
    virtual double forward(const Sequence & s, Matrix &alpha) const;
    
    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const;
    
    //! Viterbi algorithm
    virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const ;
    virtual std::string model_name() const {
      return "HiddenMarkovModel";
    }
    virtual ProbabilisticModelCreatorPtr getFactory() const {
      return HiddenMarkovModelCreatorPtr(new HiddenMarkovModelCreator());
    }
    virtual DecodableModel * decodable()  {
      return this;
    }
    virtual void trainBaumWelch (SequenceList & training_set, int maxiterations, double diff) ;
    
    virtual void initialize(const ProbabilisticModelParameters & par) ;

    virtual ProbabilisticModelParameters parameters() const ;

    void setInitialProbability(MultinomialDistributionPtr initial) ;
    void setObservationSymbols(AlphabetPtr obs) ;
    void setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) ;



  private:
    std::vector <HMMStatePtr> _states;
    MultinomialDistributionPtr _initial_probability;
    std::vector<double> _ctFactors;
    AlphabetPtr _state_names;
    void scale(std::vector<double> & in, int t);
    std::vector<double> iterate(Sequence & obs);
  };
  
  typedef boost::shared_ptr<HiddenMarkovModel> HiddenMarkovModelPtr;
}


#endif
