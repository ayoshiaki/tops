#include "ConditionNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ConditionNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitConditionNode(this);
    }
    
    std::string ConditionNode::str() {
      return std::string("(ConditionNode " + _str1->str() + " " + _str2->str() + ")");
    }
  }
}