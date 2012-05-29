#ifndef PAIR_DECODABLE_MODEL_HPP
#define PAIR_DECODABLE_MODEL_HPP


#include "ProbabilisticModel.hpp"
#include "Symbol.hpp"
#include "Sequence.hpp"
#include "util.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  
  class PairDecodableState {
    
  protected:
    
    int _id;
    SymbolPtr _name;
    IntVector _incomingTransitions, _outgoingTransitions;
    MultinomialDistributionPtr _transitions;
    ProbabilisticModelPtr _emission;
    
  public:
    
    void not_implemented(std::string method){
      cerr << "The state type " << state_type() << " does not implement the method: " << method << endl;
      exit(-1);
    }

    virtual std::string state_type(){
      return "PairDecodableState";
    }

    virtual int eSeq1(){
      not_implemented("int eSeq1()");
      return 0;
    }
    
    virtual int eSeq2(){
      not_implemented("int eSeq2()");
      return 0;
    }
    
    virtual int maxSeq1(){
      not_implemented("int maxSeq1()");
      return 0;
    }
    
    virtual int minSeq1(){
      not_implemented("int minSeq1()");
      return 0;
    }
    
    virtual int maxSeq2(){
      not_implemented("int maxSeq2()");
      return 0;
    }
    
    virtual int minSeq2(){
      not_implemented("int minSeq2()");
      return 0;
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
    
    MultinomialDistributionPtr transitions() {
      return _transitions;
    }

    virtual ProbabilisticModelPtr emission(){
      return _emission;
    }

    virtual ProbabilisticModelPtr duration(){
      not_implemented("ProbabilisticModelPtr duration()");
      ProbabilisticModelPtr a = ProbabilisticModelPtr(new ProbabilisticModel());
      return a;
    }

    SymbolPtr getName() const {
      return _name;
    }
    
    int getId() {
      return _id;
    }
  };
  
  typedef boost::shared_ptr <PairDecodableState> PairDecodableStatePtr;

//! Interface defining probabilistic model with the viterbi, forward and backward algorithm for pairs of sequences
  class PairDecodableModel : public ProbabilisticModel  {
  
  protected:
    std::vector<PairDecodableStatePtr> _states;
    AlphabetPtr _state_names;

  public:
    PairDecodableModel(){};

    void not_implemented(std::string method) const{
      cerr << "The model " << model_name() << " does not implement the method: " << method << endl;
      exit(-1);
    }

    void setStates(std::vector<PairDecodableStatePtr> states, AlphabetPtr state_names){
      _states = states;
      _state_names = state_names;
    }

    void setObservationSymbols(AlphabetPtr obs){
      tops::ProbabilisticModel::setAlphabet(obs);
    }

    PairDecodableStatePtr getState(int id) const {
      return _states[id];
    }

    AlphabetPtr getStateNames() const  {
      return _state_names;
    }

    virtual PairDecodableModel * pairDecodable() {
      return this;
    }

    virtual std::string model_name() const {
      return "PairDecodableModel";
    }

    virtual string getStateName(int state) const{
      return _state_names->getSymbol(state)->name();
    }

    virtual void setSilentStates(std::vector<PairDecodableStatePtr> states){
      not_implemented("void setSilentStates(std::vector<PairDecodableStatePtr> states)");
    }

    virtual int getSilentId(int k) const {
      not_implemented("int getSilentId(int k) const");
      return 0;
    }

    virtual void silentStatesSort(vector<PairDecodableStatePtr> silStates){
      not_implemented("void silentStatesSort(vector<PairDecodableStatePtr> silStates)");
    }
    
    /////////////////////////////////////////////////////////////////////
    //All the methods below should be pure virtual methods in this class.
    virtual void initialize(const ProbabilisticModelParameters & par) = 0;

    std::string str () const{
      not_implemented("std::string str() const");
      return "";
    }

    virtual double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a){
      not_implemented("double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)");
      return 0.0;
    }

    virtual double backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a){
      not_implemented("double backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)");
      return 0.0;
    }

    virtual double viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & path, Sequence & al1, Sequence & al2, vector<Matrix> &a){
      not_implemented("double viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & path, Sequence & al1, Sequence & al2, vector<Matrix> &a)");
      return 0.0;
    }

    virtual void posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, vector<Matrix> & probabilities){
      not_implemented("void posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, vector<Matrix> & probabilities)");
    }

    virtual void generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path){
      not_implemented("void generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path)");
    }

    virtual void trainBaumWelch(SequenceList & sample1, SequenceList & sample2, int maxiterations, double diff_threshold){
      not_implemented("void trainBaumWelch(SequenceList & sample1, SequenceList & sample2, int maxiterations, double diff_threshold)");
    }

    virtual ProbabilisticModelCreatorPtr getFactory() const {
      not_implemented("ProbabilisticModelCreatorPtr getFactory() const");
      ProbabilisticModelCreatorPtr a;
      return a;
    }

  };
    
  typedef boost::shared_ptr <PairDecodableModel> PairDecodableModelPtr;
}
#endif
