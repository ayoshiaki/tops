#include "ValueNode.hpp"

#include <iostream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ValueNode::accept(ToPSLangVisitor& visitor) {
      std::cerr << "error: ASTNode should be an abstract node" << std::endl;
      exit(0);
    }
    
    std::string ValueNode::str() {
      return std::string("(ValueNode)");
    }
  }
}