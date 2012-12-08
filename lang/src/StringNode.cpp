#include "StringNode.hpp"

namespace tops {
  namespace lang {
    std::string StringNode::str() {
      return std::string("(StringNode " + _value + ")");
    }
  }
}