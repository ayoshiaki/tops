#include "ASTNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ASTNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitASTNode(this);
    }
    
    std::string ASTNode::str() {
      return std::string("(ASTNode)");
    }
  }
}