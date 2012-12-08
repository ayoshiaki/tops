#ifndef TOPS_LANG_STRINGNODE_HPP
#define TOPS_LANG_STRINGNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    
    class StringNode: public ValueNode {
    public:
      StringNode(std::string value):_value(value) {}
      virtual void accept(ToPSLangVisitor& visitor);
      virtual std::string str();
    private:
      std::string _value;
    };
    
    typedef boost::shared_ptr<StringNode> PStringNode;
  }
}

#endif