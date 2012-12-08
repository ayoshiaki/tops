#ifndef TOPS_LANG_KEYNODE_HPP
#define TOPS_LANG_KEYNODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    class KeyNode: public ValueNode {
    public:
      KeyNode(std::string name):_name(name) {}
      virtual std::string str();
    private:
      std::string _name;
    };
    
    typedef boost::shared_ptr<KeyNode> PKeyNode;
  }
}

#endif
