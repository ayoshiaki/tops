/*
 *       PhasedFactorableModelEvaluationAlgorithm.hpp
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

#ifndef PHASED_FACTORABLE_EVALUATION_ALGORITHM_H
#define PHASED_FACTORABLE_EVALUATION_ALGORITHM_H

#include "crossplatform.hpp"

#include "EvaluationAlgorithm.hpp" 
#include "Sequence.hpp"
#include "ProbabilisticModel.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace tops {

  //! Evaluation algorithm for factorable models
  class DLLEXPORT PhasedFactorableModelEvaluationAlgorithm : public  EvaluationAlgorithm {
  private:
    std::vector<DoubleVector> _alpha;
    std::vector<DoubleVector> _precision;
    int _phase;
  public:
    PhasedFactorableModelEvaluationAlgorithm() {};
    virtual ~PhasedFactorableModelEvaluationAlgorithm(){};
    virtual double compute(int begin, int end) const;
    virtual double compute(int begin, int end, int phase) const;
    virtual void initialize(const Sequence & s, const ProbabilisticModel *m) ;
    virtual void initialize(const Sequence & s, const ProbabilisticModel *m, int phase);
  };
  typedef boost::shared_ptr <PhasedFactorableModelEvaluationAlgorithm> PhasedFactorableModelEvaluationAlgorithmPtr;  

}
#endif
