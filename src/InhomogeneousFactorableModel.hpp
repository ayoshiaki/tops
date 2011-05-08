/*
 *       InhomogeneousFactorableModel.hpp
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

#ifndef IH_FACTORABLE_MODEL_HPP
#define IH_FACTORABLE_MODEL_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"

namespace tops {
  //! Interface defining inhomogeneous models in which the  likelihood of the sequence is factorable in the sense that they can be expressed as a product of terms evaluated at each position in a sequence 
  class DLLEXPORT InhomogeneousFactorableModel : public ProbabilisticModel {
  private:
    ProbabilisticModel * _model;
    std::vector<DoubleVector> _alpha;
    std::vector<DoubleVector> _precision;
    DoubleVector _scores;
    int _phase;
  public:
     InhomogeneousFactorableModel() {};
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const; 

    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end, int phase) const;
    virtual Sequence & choose(Sequence & h, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i, int size) const;

    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const;
    virtual ~ InhomogeneousFactorableModel(){};
    //! Evaluate the position i of the sequence s.
    /*! 
      \param s is the sequence
      \param i is the position of the sequence
      \param t is the considered time 
    */
    virtual double evaluatePosition(const Sequence & s, int i, int t) const = 0;
    //! Choose the position i of the sequence s given the subsequence before the position i.
    /*! 
      \param s is the sequence
      \param i is the position of the sequence
      \param t is the considered time 
    */
    virtual int choosePosition(const Sequence & s, int i, int t) const = 0;

    //! Returns the maximum value of t.
    virtual int maximumTimeValue() const = 0;

    //! Phased 
    virtual bool isPhased() const = 0;

    virtual  double prefix_sum_array_compute(int begin, int end) ;
    virtual  double prefix_sum_array_compute(int begin, int end, int phase) ;
    
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s){
      return initialize_prefix_sum_array(s, 0);
    }

    virtual InhomogeneousFactorableModel * inhomogeneous() {
      return this;
    }

  };
}

#endif
