#include "ConditionNode.hpp"

namespace tops {
  namespace lang {
    std::string ConditionNode::str() {
      return std::string("(ConditionNode " + _str1->str() + " " + _str2->str() + ")");
    }
  }
}