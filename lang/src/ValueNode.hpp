#ifndef TOPS_LANG_VALUENODE_HPP
#define TOPS_LANG_VALUENODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ASTNode.hpp"

namespace tops {
  namespace lang {
    class ValueNode: public ASTNode {
    public:
      virtual std::string str();
    };
    
    typedef boost::shared_ptr<ValueNode> PValueNode;
  }
}

#endif
