#ifndef PHASED_FACTORABLE_EVALUATION_ALGORITHM_H
#define PHASED_FACTORABLE_EVALUATION_ALGORITHM_H

#include "EvaluationAlgorithm.hpp" 
#include "Sequence.hpp"
#include "ProbabilisticModel.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace tops {

  //! Evaluation algorithm for factorable models
  class PhasedFactorableModelEvaluationAlgorithm : public  EvaluationAlgorithm {
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
