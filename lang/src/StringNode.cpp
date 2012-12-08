#include "StringNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void StringNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitStringNode(this);
    }
    
    std::string StringNode::str() {
      return std::string("(StringNode " + _value + ")");
    }
  }
}