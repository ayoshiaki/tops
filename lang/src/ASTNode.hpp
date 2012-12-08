#ifndef TOPS_LANG_ASTNODE_HPP
#define TOPS_LANG_ASTNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

namespace tops {
  namespace lang {
    class ASTNode {
    public:
      virtual std::string str();
    };
    
    typedef boost::shared_ptr<ASTNode> PASTNode;
  }
}

#endif
