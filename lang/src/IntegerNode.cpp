#include "IntegerNode.hpp"

#include <sstream>

namespace tops {
  namespace lang {
    std::string IntegerNode::str() {
      return std::string("(IntegerNode " + _str_value + ")");
    }
  }
}
