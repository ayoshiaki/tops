#ifndef TOPS_LANG_ASTNODE_HPP
#define TOPS_LANG_ASTNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    class ASTNode {
    public:
      virtual void accept(ToPSLangVisitor& visitor);
      virtual std::string str();
    };
    
    typedef boost::shared_ptr<ASTNode> PASTNode;
  }
}

#endif
