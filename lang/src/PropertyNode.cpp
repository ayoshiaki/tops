#include "PropertyNode.hpp"

#include "KeyNode.hpp"
#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    std::string PropertyNode::str() {
      return std::string("(PropertyNode " + _key->str() + " " + _value->str() + ")");
    }
  }
}