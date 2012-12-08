#ifndef TOPS_LANG_CONDITIONALPROBABILITYNODE_HPP
#define TOPS_LANG_CONDITIONALPROBABILITYNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ASTNode.hpp"
#include "ConditionNode.hpp"
#include "FloatNode.hpp"

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    
    class ConditionalProbabilityNode: public ASTNode {
    public:
      ConditionalProbabilityNode(PConditionNode cond, PFloatNode value):_cond(cond), _value(value) {}
      virtual void accept(ToPSLangVisitor& visitor);
      virtual std::string str();
    private:
      PConditionNode _cond;
      PFloatNode _value;
    };
    
    typedef boost::shared_ptr<ConditionalProbabilityNode> PConditionalProbabilityNode;
  }
}

#endif
