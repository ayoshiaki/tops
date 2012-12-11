#include "StringNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    
    StringNode::StringNode(std::string value) {
      _value = value.substr(1, value.size() - 2);
    }
    
    void StringNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitStringNode(this);
    }
    
    std::string StringNode::value() {
      return _value;
    }
    
    std::string StringNode::str() {
      return std::string("(StringNode " + _value + ")");
    }
  }
}