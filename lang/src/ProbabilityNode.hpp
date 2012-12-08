#ifndef TOPS_LANG_PROBABILITYNODE_HPP
#define TOPS_LANG_PROBABILITYNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ASTNode.hpp"
#include "FloatNode.hpp"
#include "StringNode.hpp"

namespace tops {
  namespace lang {
    class ProbabilityNode: public ASTNode {
    public:
      ProbabilityNode(PStringNode cond, PValueNode value):_cond(cond), _value(value) {}
      virtual std::string str();
    private:
      PStringNode _cond;
      PValueNode _value;
    };
    
    typedef boost::shared_ptr<ProbabilityNode> PProbabilityNode;
  }
}

#endif
