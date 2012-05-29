/*
 *       FixedSequenceAtPosition.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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

#ifndef FIXED_SEQUENCE_AT_POSITION_HPP
#define FIXED_SEQUENCE_AT_POSITION_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelDecorator.hpp"
#include "DiscreteIIDModel.hpp"
namespace tops {
  //! A decorator that forces the emission of the same sequence at a fixed position of the sequence.
  class DLLEXPORT FixedSequenceAtPosition : public ProbabilisticModelDecorator {
  private:
    int _position;
    Sequence _sequence;
    DiscreteIIDModelPtr _probabilities;
    std::vector<bool> _matchSeq;
    void initializeMatchedPositions(const Sequence & s) ;
    void addSequence(Sequence & h) const;
  public:
    FixedSequenceAtPosition(ProbabilisticModelPtr m) : ProbabilisticModelDecorator(m) {};
    virtual ~FixedSequenceAtPosition(){};
    virtual void initialize(int position,Sequence sequence, DiscreteIIDModelPtr distr);
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual Sequence & choose(Sequence & h, int size) const ;
    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const;
    virtual double prefix_sum_array_compute(int begin , int end) ;
    virtual double prefix_sum_array_compute(int begin , int end, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s) ;
    virtual std::string str() const;
    std::string model_name () const;
  };
  typedef boost::shared_ptr<FixedSequenceAtPosition> FixedSequenceAtPositionPtr;
}

#endif
