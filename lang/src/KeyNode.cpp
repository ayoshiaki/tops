#include "KeyNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void KeyNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitKeyNode(this);
    }
    
    std::string KeyNode::name() {
      return _name;
    }
    
    std::string KeyNode::str() {
      return std::string("(KeyNode " + _name + ")");
    }
  }
}