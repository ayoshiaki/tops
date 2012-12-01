/*
 *       FactorableModel.hpp
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

#ifndef FACTORABLE_MODEL_HPP
#define FACTORABLE_MODEL_HPP

#include "crossplatform.hpp"

#include "Sequence.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>
#include <iostream>

namespace tops {
  //! Abstract class defining models in which the likelihood of the sequence is factorable in the sense that they can be expressed as a product of terms evaluated at each position in a sequence.
  class DLLEXPORT FactorableModel : public ProbabilisticModel {
  private:
    double _dummy;
    DoubleVector _alpha;
    DoubleVector _precision;
  public:
     FactorableModel() {};
    virtual ~FactorableModel(){};

    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;

    virtual Sequence & choose(Sequence & h, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i, int size) const;

    //! Evaluate the position i of the sequence s.
    virtual double evaluatePosition(const Sequence & s, unsigned int i) const = 0;

    //! Choose the position i of the sequence s given the subsequence before the position i.
    virtual double choosePosition(const Sequence & s, int i) const = 0;



    virtual  double prefix_sum_array_compute(int begin, int end) ;

    virtual bool initialize_prefix_sum_array(const Sequence & s) ;

    virtual FactorableModel* factorable() {
      return this;
    }
  };
  typedef boost::shared_ptr<FactorableModel> FactorableModelPtr;
}

#endif
