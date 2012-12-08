#include "IntegerNode.hpp"

#include <sstream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void IntegerNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitIntegerNode(this);
    }
    
    std::string IntegerNode::str() {
      return std::string("(IntegerNode " + _str_value + ")");
    }
  }
}
