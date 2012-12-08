#ifndef TOPS_LANG_INTEGERNODE_HPP
#define TOPS_LANG_INTEGERNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    class IntegerNode: public ValueNode {
    public:
      IntegerNode(std::string str_value):_str_value(str_value) {}
      virtual std::string str();
    private:
      std::string _str_value;
    };
    
    typedef boost::shared_ptr<IntegerNode> PIntegerNode;
  }
}

#endif
