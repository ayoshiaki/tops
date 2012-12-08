#ifndef TOPS_LANG_PROBABILITYMAPNODE_HPP
#define TOPS_LANG_PROBABILITYMAPNODE_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"
#include "ProbabilityNode.hpp"

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    
    class ProbabilityMapNode: public ValueNode {
    public:
      ProbabilityMapNode(std::vector<PProbabilityNode> probs):_probs(probs) {}
      virtual void accept(ToPSLangVisitor& visitor);
      void addProbability(PProbabilityNode prob);
      virtual std::string str();
    private:
      std::vector<PProbabilityNode> _probs;
    };
    
    typedef boost::shared_ptr<ProbabilityMapNode> PProbabilityMapNode;
  }
}

#endif
