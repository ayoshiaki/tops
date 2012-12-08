#include "ConditionalProbabilityNode.hpp"

namespace tops {
  namespace lang {
    std::string ConditionalProbabilityNode::str() {
      return std::string("(ConditionalProbabilityNode " + _cond->str() + " " + _value->str() + ")");
    }
  }
}