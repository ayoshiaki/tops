#include "ConditionalProbabilityMapNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {

    void ConditionalProbabilityMapNode::addConditionalProbability(PConditionalProbabilityNode cond) {
      _conds.push_back(cond);
    }
    
    void ConditionalProbabilityMapNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitConditionalProbabilityMapNode(this);
    }

    std::string ConditionalProbabilityMapNode::str() {
      std::string str = "(ConditionalProbabilityMapNode";
      std::vector<PConditionalProbabilityNode>::iterator it;
      for (it = _conds.begin() ; it != _conds.end(); ++it) {
        str += " " + (*it)->str();
      }
      str += ")";
      return str;
    }
  }
}
