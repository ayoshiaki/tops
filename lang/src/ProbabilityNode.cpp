#include "ProbabilityNode.hpp"

namespace tops {
  namespace lang {
    std::string ProbabilityNode::str() {
      return std::string("(ProbabilityNode " + _cond->str() + " " + _value->str() + ")");
    }
  }
}
