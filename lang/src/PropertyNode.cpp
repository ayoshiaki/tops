#include "PropertyNode.hpp"

#include "KeyNode.hpp"
#include "ValueNode.hpp"
#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void PropertyNode::accept(ToPSLangVisitor& visitor) {
      _key->accept(visitor);
      _value->accept(visitor);
      visitor.visitPropertyNode(this);
    }
    
    std::string PropertyNode::str() {
      return std::string("(PropertyNode " + _key->str() + " " + _value->str() + ")");
    }
  }
}