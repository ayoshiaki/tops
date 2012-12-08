#ifndef TOPS_LANG_PROPERTYNODE_HPP
#define TOPS_LANG_PROPERTYNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ASTNode.hpp"
#include "KeyNode.hpp"
#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    
    class PropertyNode: public ASTNode {
    public:
      PropertyNode(PKeyNode key, PValueNode value):_key(key), _value(value) {}
      virtual void accept(ToPSLangVisitor& visitor);
      virtual std::string str();
    private:
      PKeyNode _key;
      PValueNode _value;
    };
    
    typedef boost::shared_ptr<PropertyNode> PPropertyNode;
  }
}

#endif
