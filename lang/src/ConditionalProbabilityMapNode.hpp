#ifndef TOPS_LANG_CONDITIONALPROBABILITYMAPNODE_HPP
#define TOPS_LANG_CONDITIONALPROBABILITYMAPNODE_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"
#include "ConditionalProbabilityNode.hpp"

namespace tops {
  namespace lang {
    class ConditionalProbabilityMapNode: public ValueNode {
    public:
      ConditionalProbabilityMapNode(std::vector<PConditionalProbabilityNode> conds):_conds(conds) {}
      void addConditionalProbability(PConditionalProbabilityNode cond);
      virtual std::string str();
    private:
      std::vector<PConditionalProbabilityNode> _conds;
    };
    
    typedef boost::shared_ptr<ConditionalProbabilityMapNode> PConditionalProbabilityMapNode;
  }
}

#endif
