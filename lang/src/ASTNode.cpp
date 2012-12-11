#include "ASTNode.hpp"

#include <iostream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ASTNode::accept(ToPSLangVisitor& visitor) {
      std::cerr << "error: ASTNode should be an abstract node" << std::endl;
      exit(0);
    }
    
    std::string ASTNode::str() {
      return std::string("(ASTNode)");
    }
  }
}