#ifndef REVERSE_COMPLEMENT_HPP
#define REVERSE_COMPLEMENT_HPP

#include "ProbabilisticModelDecorator.hpp"
#include "MultinomialDistribution.hpp"
#include "Symbol.hpp"
namespace tops { 

  class ReverseComplementDNA : public ProbabilisticModelDecorator {
  private:
    AlphabetPtr revAlphabet;
    int _seqLength;
    void revcomp(Sequence & revCompSeq, const Sequence & s, int begin,  int end) const;
  public:
    ReverseComplementDNA(ProbabilisticModelPtr m) : ProbabilisticModelDecorator(m) 
    {
      revAlphabet =  AlphabetPtr(new Alphabet());
      AlphabetPtr a = m->alphabet();
      std::string A("A");
      std::string C("C");
      std::string G("G");
      std::string T("T");
      for(int i = 0; i < (int)a->size(); i++){
	SymbolPtr s = a->getSymbol(i);
	if(s->name() == A) {
	  revAlphabet->createSymbol(T);
	} else if (s->name() == G) {
	  revAlphabet->createSymbol(C);
	} else if (s->name() == C) {
	  revAlphabet->createSymbol(G); 
	} else if (s->name() == T) {
	  revAlphabet->createSymbol(A);
	}
      }
    };
    virtual ~ReverseComplementDNA(){};
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual Sequence & choose(Sequence & h, int size) const ;
    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const;
    virtual double prefix_sum_array_compute(int begin , int end) ;
    virtual double prefix_sum_array_compute(int begin , int end, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s) ;
    virtual std::string str() const;
    std::string model_name () const;
    virtual ProbabilisticModelParameters parameters() const ;
    virtual void initialize(const ProbabilisticModelParameters & p) ;

  };
  typedef boost::shared_ptr<ReverseComplementDNA> ReverseComplementDNAPtr;
}

#endif
