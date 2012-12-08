#include "ValueNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ValueNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitValueNode(this);
    }
    
    std::string ValueNode::str() {
      return std::string("(ValueNode)");
    }
  }
}