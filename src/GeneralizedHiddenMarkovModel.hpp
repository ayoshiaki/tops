#ifndef GENERALIZED_HIDDEN_MARKOV_MODEL_H
#define GENERALIZED_HIDDEN_MARKOV_MODEL_H
#include <set>

#include "MultinomialDistribution.hpp"
#include "ProbabilisticModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "DecodableModel.hpp"
#include "GHMMStates.hpp"
#include "util.hpp"

namespace tops {
  //! This class is used by the GHMM to store the optimal predecessor
  class OptimalPredecessor {
  private:
    int _from;
    int _nextExplicitStateId;
    int _prevExplicitStateId;
    int _begin;
    int _endOfExplicitStateId;
  public:
    OptimalPredecessor(int from, int nextEid, int prevEid, int begin, int end) {
      _from = from;
      _nextExplicitStateId = nextEid;
      _prevExplicitStateId = prevEid;
      _begin = begin;
      _endOfExplicitStateId = end;
    }
    int from() {
      return _from;
    }
    int nextExplicitStateId() {
      return _nextExplicitStateId;
    }
    int prevExplicitStateId() {
      return _prevExplicitStateId;
    }
    int begin() {
      return _begin;
    }
    int endOfExplicitStateId() {
      return _endOfExplicitStateId;
    }
  };
  typedef boost::shared_ptr<OptimalPredecessor> OptimalPredecessorPtr;

  //! This class stores the  candidate signal data
  class CandidateSignal {
  private:
    int _id; // Signal id
    int _begin;
    int _end;
    int _begin_of_next_state;
    double _prob;
    double _null_model_prob;
  public:
    CandidateSignal(int id, int begin, int end, int begin_of_next_state,
		    double prob, double null_model_prob) {
      _id = id;
      _begin = begin;
      _end = end;
      _begin_of_next_state = begin_of_next_state;
      _prob = prob;
      _null_model_prob = null_model_prob;
    }
    int stateId() {
      return _id;
    }
    int begin() {
      return _begin;
    }
    int end() {
      return _end;
    }
    int begin_of_next_state() {
      return _begin_of_next_state;
    }
    double prob() {
      return _prob;
    }
    double null_model_prob() {
      return _null_model_prob;
    }
  };
  typedef boost::shared_ptr<CandidateSignal> CandidateSignalPtr;

  //! This is a class representing Hidden semi-Markov Models
  class GeneralizedHiddenMarkovModel: public DecodableModel {
  private:
    MultinomialDistributionPtr _initial_probabilities;
    GHMMStates _all_states;
    AlphabetPtr _state_names;
    GHMMStates _geometric_duration_states;
    GHMMSignalStates _signal_states;
    GHMMExplicitDurationStates _explicit_duration_states;
    void initialize_prefix_sum_arrays(const Sequence & s) const;
    void findBestPredecessorGeometricStates(int i, Matrix & gamma, std::vector<std::vector<OptimalPredecessorPtr> > & ptr, const Sequence & sequence) const;
    void findBestPredecessorSignalStates(int i,
					 Matrix & gamma, std::vector<
					 std::vector<OptimalPredecessorPtr> > & ptr,
					 const Sequence & sequence,
					 std::vector<std::set<CandidateSignalPtr> > & predecessors) const;
    void findBestPredecessorSignalStatesFinish(int i,
					       Matrix & gamma, std::vector<
					       std::vector<OptimalPredecessorPtr> > & ptr,
					       const Sequence & sequence,
					       std::vector<std::set<CandidateSignalPtr> > & predecessors) const;
    //! Finds all candidate signals that are  successor of the  toSignal state and inserts the toSignal state as the predecessor candidate signal.
    void addSignalPredecessors(CandidateSignalPtr sig, std::vector<std::set<CandidateSignalPtr> > & predecessors) const;
    void restore_model(std::string & model_name, const ProbabilisticModelParameters & parameters);
    std::map<std::string, ProbabilisticModelPtr> _models;
  public:
    GeneralizedHiddenMarkovModel() {
    }
  
    virtual ~GeneralizedHiddenMarkovModel() {
    }
  
    void fixStatesPredecessorSuccessor();
  
    //! Forward algorithm
    virtual double forward(const Sequence & s, Matrix &alpha) const;
  
    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const;

    //! Viterbi algorithm
    virtual double
    viterbi(const Sequence &s, Sequence &path, Matrix & gamma) const;

    //! Choose the observation given a state
    virtual Sequence & chooseObservation(Sequence & h, int i, int state) const;

    //! Choose a state
    virtual int chooseState(int state) const;

    //! Choose the initial state
    virtual int chooseFirstState() const;

    //! Choose the initial state
    virtual MultinomialDistributionPtr getInitialProbabilities() const {
      return _initial_probabilities;
    }

    //! Get state name
    virtual std::string getStateName(int state) const;

    //! Get the state names
    virtual AlphabetPtr getStateNames() const;
    virtual std::string model_name() const {
      return "GeneralizedHiddenMarkovModel";
    }

    virtual ProbabilisticModelCreatorPtr getFactory() const;
    virtual std::string str() const;

    virtual DecodableModel * decodable() {
      return this;
    }
    void configureSignalState(std::string observation_model_name, 
			      std::string null_model_name, 
			      MultinomialDistributionPtr transition_distr, double threshold,
			      int size, std::string state_name, int iphase, int ophase);
  
    void configureGeometricDurationState(std::string observation_model_name,
					 MultinomialDistributionPtr transition_distr,
					 std::string state_name, int iphase, int ophase);
    void configureExplicitDurationState(std::string observation_model_name,
					MultinomialDistributionPtr transition_distr,
					std::string duration_model_name, std::string state_name, int iphase, int ophase, int start, int stop, int leftJoinable, int rightJoinable);
    void setInitialProbability(MultinomialDistributionPtr init);
    void setObservationSymbols(AlphabetPtr obs) {
      setAlphabet(obs);
    }
    void setStateNames(AlphabetPtr alphabet);
    virtual ProbabilisticModelParameters parameters() const ;
    virtual void initialize(const ProbabilisticModelParameters & p) ;    


  };

  typedef boost::shared_ptr<GeneralizedHiddenMarkovModel>
  GeneralizedHiddenMarkovModelPtr;
}
#endif
