/*
 *       PhasedRunLengthDistribution.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#ifndef PHASED_RUN_LENGTH_DISTRIBUTION_HPP
#define PHASED_RUN_LENGTH_DISTRIBUTION_HPP

#include "ProbabilisticModelDecorator.hpp"
#include "MultinomialDistribution.hpp"
namespace tops {
  //! Provides mechanisms to control the phase of a probabilistic model
  class PhasedRunLengthDistribution : public ProbabilisticModelDecorator {
  private:
    int _delta;
    int _iphase;
    int _ophase;
    int _nphases;
    double _normfactor;
    std::string _submodelname;
  public:
    PhasedRunLengthDistribution(ProbabilisticModelPtr m) : ProbabilisticModelDecorator(m) {};
    virtual ~PhasedRunLengthDistribution(){};
    virtual void initialize(int delta, int input_phase, int output_phase, int nphases);
    virtual double choose() const;
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual double  log_probability_of(int s) const;
    virtual Sequence & choose(Sequence & h, int size) const;
      virtual int size() const ;
    virtual ProbabilisticModelParameters parameters() const ;
    virtual void initialize(const ProbabilisticModelParameters & p) ;


#if 0
    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int size) const;
    virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int size) const;
    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const;
#endif;
    virtual std::string str() const;
    std::string model_name () const;
  };
  typedef boost::shared_ptr<PhasedRunLengthDistribution> PhasedRunLengthDistributionPtr;
}

#endif
