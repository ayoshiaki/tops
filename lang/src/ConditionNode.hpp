#ifndef TOPS_LANG_CONDITIONNODE_HPP
#define TOPS_LANG_CONDITIONNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ASTNode.hpp"
#include "StringNode.hpp"

namespace tops {
  namespace lang {
    class ConditionNode: public ASTNode {
    public:
      ConditionNode(PStringNode str1, PStringNode str2):_str1(str1), _str2(str2) {}
      virtual std::string str();
    private:
      PStringNode _str1;
      PStringNode _str2;
    };
    
    typedef boost::shared_ptr<ConditionNode> PConditionNode;
  }
}

#endif
