#include "MultipleSequentialModelCreator.hpp"
#include "MultipleSequentialModel.hpp"
namespace tops {
  ProbabilisticModelPtr MultipleSequentialModelCreator::create(ProbabilisticModelParameters & parameters) const {
    MultipleSequentialModelPtr model = MultipleSequentialModelPtr(new MultipleSequentialModel());
    model->initialize(parameters);
    return model;
  }
}
