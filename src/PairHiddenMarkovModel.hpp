#ifndef PAIR_HIDDEN_MARKOV_MODEL
#define PAIR_HIDDEN_MARKOV_MODEL

#include "HiddenMarkovModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ContextTree.hpp"

#include "HiddenMarkovModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops{

  class PairHiddenMarkovModel :   public DecodableModel{
  private:
    HiddenMarkovModelPtr _hmm; 
    int _gap_id;
    void scale(std::vector<double> & in, int t);
    std::vector<double> iterate(Sequence & obs);

  public:
    
    PairHiddenMarkovModel() {
    };

    PairHiddenMarkovModel( std::vector <HMMStatePtr> states, MultinomialDistributionPtr initial_probability, AlphabetPtr state_names, AlphabetPtr observation_symbols) {
      _hmm = HiddenMarkovModelPtr(new HiddenMarkovModel(states, initial_probability, state_names, observation_symbols));
      tops::ProbabilisticModel::setAlphabet(observation_symbols);
    }

    virtual ~PairHiddenMarkovModel(){}

    void setStates(std::vector<HMMStatePtr> states) {
      _hmm->setStates(states);
    }

    //! Choose the observation given a state 
    //virtual Sequence &  chooseObservation ( Sequence & h, int i,  int state) const ;

    //! Choose a state
    //virtual int chooseState(int state ) const ;

    //! Choose first state
    //virtual int chooseFirstState() const ;

    virtual AlphabetPtr getStateNames() const {
      return _hmm->getStateNames();
    }

    virtual std::string getStateName(int state) const;
    
    //virtual std::string str () const ;
    
    virtual void setState (int id, HMMStatePtr state)
    {
      _hmm->setState(id, state);
    }

    virtual HMMStatePtr getState(int id) const 
    {
      return _hmm->getState(id);
    }

    //! Forward algorithm
    virtual double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a) const;
    
    //! Backward algorithm
    //virtual double backward(const Sequence & s, Matrix &beta) const;
    
    //! Viterbi algorithm
    //virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const ;
    
    //! Posterior Probabilities: P(yi=k|x)
    //virtual void posteriorProbabilities (const Sequence &s, Matrix & probabilities) const;

    //! Posterior Decoding: ^yi = argmax_k P(yi=k|x)
    //virtual void posteriorDecoding (const Sequence &s, Sequence &path, Matrix & probabilities) const;

    virtual double forward(const Sequence & s, Matrix &alpha) const{
      cerr << "For pairHMM this algorithm needs two input sequences." << endl; 
      exit(-1);
    }

    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const {
      cerr << "For pairHMM this algorithm needs two input sequences." << endl;
      exit(-1);
    }

    //! Viterbi algorithm
    virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const {
      cerr << "For pairHMM this algorithm needs two input sequences." << endl;
      exit(-1);
    }
    
    //! Posterior Probabilities: P(yi=k|x)
    virtual void posteriorProbabilities (const Sequence &s, Matrix & probabilities) const {
      cerr << "For pairHMM this algorithm needs two input sequences." << endl;
      exit(-1);
    }
    
    //! Posterior Decoding: ^yi = argmax_k P(yi=k|x)
    virtual void posteriorDecoding (const Sequence &s, Sequence &path, Matrix & probabilities) const {
      cerr << "For pairHMM this algorithm needs two input sequences." << endl;
      exit(-1);
    }

    virtual std::string model_name() const {
      return "PairHiddenMarkovModel";
    }
    /*virtual ProbabilisticModelCreatorPtr getFactory() const {
      return PairHiddenMarkovModelCreatorPtr(new PairHiddenMarkovModelCreator());
      }*/
    virtual DecodableModel * decodable()  {
      return this;
    }
    //virtual void trainBaumWelch (SequenceList & training_set, int maxiterations, double diff) ;
    
    virtual void initialize(const ProbabilisticModelParameters & par) ;

    //virtual ProbabilisticModelParameters parameters() const ;

    void setInitialProbability(MultinomialDistributionPtr initial) ;
    void setObservationSymbols(AlphabetPtr obs) ;
    void setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) ;

    virtual double getEmission(const Sequence & seq1, const Sequence & seq2, int state_id, int seq1Pos, int seq2Pos, int* state_type) const;

  };

  typedef boost::shared_ptr<PairHiddenMarkovModel> PairHiddenMarkovModelPtr;
}

#endif

