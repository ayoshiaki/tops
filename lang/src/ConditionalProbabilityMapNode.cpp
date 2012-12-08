#include "ConditionalProbabilityMapNode.hpp"

namespace tops {
  namespace lang {

    void ConditionalProbabilityMapNode::addConditionalProbability(PConditionalProbabilityNode cond) {
      _conds.push_back(cond);
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
