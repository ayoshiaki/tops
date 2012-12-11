#include "ProbabilityMapNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    
    void ProbabilityMapNode::accept(ToPSLangVisitor& visitor) {
      std::vector<PProbabilityNode>::iterator it;
      for (it = _probs.begin() ; it != _probs.end(); ++it) {
        (*it)->accept(visitor);
      }
      visitor.visitProbabilityMapNode(this);
    }

    void ProbabilityMapNode::addProbability(PProbabilityNode prob) {
      _probs.push_back(prob);
    }

    std::string ProbabilityMapNode::str() {
      std::string str = "(ProbabilityMapNode";
      std::vector<PProbabilityNode>::iterator it;
      for (it = _probs.begin() ; it != _probs.end(); ++it) {
        str += " " + (*it)->str();
      }
      str += ")";
      return str;
    }
  }
}
