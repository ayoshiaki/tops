#include "ProbabilityMapNode.hpp"

namespace tops {
  namespace lang {

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
