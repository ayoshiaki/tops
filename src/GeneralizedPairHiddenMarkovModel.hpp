#ifndef GENERALIZED_PAIR_HIDDEN_MARKOV_MODEL
#define GENERALIZED_PAIR_HIDDEN_MARKOV_MODEL

#include "HiddenMarkovModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "ProbabilisticModel.hpp"
#include "PairDecodableModel.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
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

  class GPHMMState: public PairDecodableState {

  private:
    int _maxEmissionSeq1, _maxEmissionSeq2, _minEmissionSeq1, _minEmissionSeq2;
    ProbabilisticModelPtr _duration;

  public:
    GPHMMState(){}

    GPHMMState(int id, 
	       SymbolPtr name, 
	       ProbabilisticModelPtr emission,  
	       MultinomialDistributionPtr transitions, 
	       ProbabilisticModelPtr duration,
	       IntVector iTransitions, 
	       IntVector oTransitions, 
	       int maxSeq1, int minSeq1, 
	       int maxSeq2, int minSeq2)
    {
      _id = id;
      _name = name;
      _emission = emission;
      _transitions = transitions;
      _duration = duration;
      _incomingTransitions = iTransitions;
      _outgoingTransitions = oTransitions;
      _maxEmissionSeq1 = maxSeq1;
      _minEmissionSeq1 = minSeq1;
      _maxEmissionSeq2 = maxSeq2;
      _minEmissionSeq2 = minSeq2;
    }

    int maxSeq1(){
      return _maxEmissionSeq1;
    }

    int minSeq1(){
      return _minEmissionSeq1;
    }
   
    int maxSeq2(){
      return _maxEmissionSeq2;
    }
    int minSeq2(){
      return _minEmissionSeq2;
    }

    ProbabilisticModelPtr duration(){
      return _duration;
    }

    /*double emission(Sequence &s1, Sequence &s2, int i, int j, int ne1, int ne2, int gap_id){
      if(maxEmissionSeq1 == 1 && maxEmissionSeq2 == 1)
	return log_probability_of_pair(s1[i-1], s2[j-1]);
      if(maxEmissionSeq1 == 1 && maxEmissionSeq2 == 0)
	return log_probability_of_pair(s1[i-1], gap_id);
      if(maxEmissionSeq1 == 1 && maxEmissionSeq2 == 1)
	return log_probability_of_pair(gap_id, s2[j-1]);
      vector<Matrix> f;
      return _emission->pairDecodable()->forward(sub_seq(s1, i-ne1-1, i-1),sub_seq(s1, i-ne1-1, i-1),f);
      }*/

  };

  typedef boost::shared_ptr <GPHMMState> GPHMMStatePtr;
  
  class GeneralizedPairHiddenMarkovModel : public PairDecodableModel{
    
  private:
    MultinomialDistributionPtr _initialProbabilities;
    int _end_id, _gap_id;

  public:
    
    GeneralizedPairHiddenMarkovModel() {};
    
    virtual void initialize(const ProbabilisticModelParameters & par) ;

    virtual std::string model_name() const {
      return "GeneralizedPairHiddenMarkovModel";
    }

    virtual PairDecodableModel * pairDecodable() {
      return this;
    }

    void setInitialProbabilities(MultinomialDistributionPtr init){
      _initialProbabilities = init;
    }

    MultinomialDistributionPtr initialProbabilities(){
      return _initialProbabilities;
    }

    //Forward, Backward and Viterbi algorithms. We assume there are no cycles of silent states.
    double forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a);

    virtual double viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & path, Sequence & al1, Sequence & al2, vector<Matrix> &a);

    /*virtual void posteriorProbabilities (const Sequence &seq1, const Sequence &seq2, vector<Matrix> & probabilities);

    virtual void generateSequence(Sequence &seq1, Sequence &seq2, Sequence &path);

    virtual void trainBaumWelch(SequenceList & sample1, SequenceList & sample2, int maxiterations, double diff_threshold);*/

    /*virtual ProbabilisticModelCreatorPtr getFactory() const {
      return GeneralizedPairHiddenMarkovModelCreatorPtr(new GeneralizedPairHiddenMarkovModelCreator());
      }*/

    //std::string str () const;


  };
  typedef boost::shared_ptr <GeneralizedPairHiddenMarkovModel> GeneralizedPairHiddenMarkovModelPtr;

};
#endif
