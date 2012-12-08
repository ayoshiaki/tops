#include "FloatNode.hpp"

#include <sstream>

namespace tops {
  namespace lang {
    std::string FloatNode::str() {
      return std::string("(FloatNode " + _str_value + ")");
    }
  }
}
