#ifndef PAIR_HIDDEN_MARKOV_MODEL
#define PAIR_HIDDEN_MARKOV_MODEL

#include "HiddenMarkovModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"
#include "Symbol.hpp"

#include "PairHiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops{
  class PHMMState: public HMMState {

  private:
    int _emissionSeq1, _emissionSeq2;
    IntVector _incomingTransitions, _outgoingTransitions;

  public:
    PHMMState(){}

    PHMMState(int id, SymbolPtr name, MultinomialDistributionPtr emission,  MultinomialDistributionPtr transitions, IntVector iTransitions, IntVector oTransitions, int eSeq1, int eSeq2){
      _id = id;
      _name = name;
      _emission = emission;
      _transitions = transitions;
      _incomingTransitions = iTransitions;
      _outgoingTransitions = oTransitions;
      _emissionSeq1 = eSeq1;
      _emissionSeq2 = eSeq2;
    }

    int eSeq1(){
      return _emissionSeq1;
    }

    int eSeq2(){
      return _emissionSeq2;
    }

    void setNumEmissionSeq1(int i){
      _emissionSeq1 = i;
    }

    void setNumemissionSeq2(int i){
      _emissionSeq2 = i;
    }

    vector<int> iTransitions() const{
      return _incomingTransitions;
    }

    int getITransId(int i){
      return _incomingTransitions[i];
    }

    vector<int> oTransitions(){
      return _outgoingTransitions;
    }

    int getOTransId(int i){
      return _outgoingTransitions[i];
    }
  };

  typedef boost::shared_ptr <PHMMState> PHMMStatePtr;

  class PairHiddenMarkovModel : public ProbabilisticModel{
  private:
    int _gap_id;
    int _end_id;
    int _begin_id;
    std::vector<PHMMStatePtr> _states;
    AlphabetPtr _state_names;
    std::vector<PHMMStatePtr> _silent_states;

  public:

    PairHiddenMarkovModel() {
    };

    PairHiddenMarkovModel( std::vector <PHMMStatePtr> states, AlphabetPtr state_names, AlphabetPtr observation_symbols) {
      _states = states;
      _state_names = state_names;
      tops::ProbabilisticModel::setAlphabet(observation_symbols);
    }

    virtual ~PairHiddenMarkovModel(){}

    //Forward, Backward and Viterbi algorithms. We assume there are no cycles of silent states.
    virtual double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & path, Sequence & al1, Sequence & al2, vector<Matrix> &a);

    virtual void posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, vector<Matrix> & probabilities);

    virtual void generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path);

    virtual void trainBaumWelch(SequenceList & sample1, SequenceList & sample2, int maxiterations, double diff_threshold);

    virtual std::string model_name() const {
      return "PairHiddenMarkovModel";
    }

    virtual ProbabilisticModelCreatorPtr getFactory() const {
      return PairHiddenMarkovModelCreatorPtr(new PairHiddenMarkovModelCreator());
    }

    virtual void initialize(const ProbabilisticModelParameters & par) ;

    void setStates(std::vector<PHMMStatePtr> states, AlphabetPtr state_names){
      _states = states;
      _state_names = state_names;
    }

    void setObservationSymbols(AlphabetPtr obs){
      tops::ProbabilisticModel::setAlphabet(obs);
    }

    void setSilentStates(std::vector<PHMMStatePtr> states){
      _silent_states = states;
    }

    PHMMStatePtr getPHMMState(int id) const {
      return _states[id];
    }

    int getSilentId(int k) const {
      return _silent_states[k]->getId();
    }

    virtual AlphabetPtr getStateNames() const  {
      return _state_names;
    }

    virtual string getStateName(int state) const{
      return _state_names->getSymbol(state)->name();
    }

    std::string str () const;

    void silentStatesSort(vector<PHMMStatePtr> silStates);

    virtual PairHiddenMarkovModel * pairDecodable() {
      return this;
    }

  };

  typedef boost::shared_ptr<PairHiddenMarkovModel> PairHiddenMarkovModelPtr;
}

#endif

