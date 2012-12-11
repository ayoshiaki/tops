#include "ConditionalProbabilityNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ConditionalProbabilityNode::accept(ToPSLangVisitor& visitor) {
      _cond->accept(visitor);
      _value->accept(visitor);
      visitor.visitConditionalProbabilityNode(this);
    }
    
    std::string ConditionalProbabilityNode::str() {
      return std::string("(ConditionalProbabilityNode " + _cond->str() + " " + _value->str() + ")");
    }
  }
}