#include "KeyNode.hpp"

namespace tops {
  namespace lang {
    std::string KeyNode::str() {
      return std::string("(KeyNode " + _name + ")");
    }
  }
}