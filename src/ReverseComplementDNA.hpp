/*
 *       ReverseComplementDNA.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef REVERSE_COMPLEMENT_HPP
#define REVERSE_COMPLEMENT_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelDecorator.hpp"
#include "DiscreteIIDModel.hpp"
#include "Symbol.hpp"
namespace tops {

  class DLLEXPORT ReverseComplementDNA : public ProbabilisticModelDecorator {
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
