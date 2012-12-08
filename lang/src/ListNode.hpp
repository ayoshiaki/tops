#ifndef TOPS_LANG_LISTNODE_HPP
#define TOPS_LANG_LISTNODE_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"
#include "IntegerNode.hpp"
#include "StringNode.hpp"
#include "FloatNode.hpp"

namespace tops {
  namespace lang {
    class ListNode: public ValueNode {
    public:
      ListNode(std::vector<PValueNode> v):_v(v) {}

      void addElement(PValueNode e);

      virtual std::string str();
    private:
      std::vector<PValueNode> _v;
    };
    typedef boost::shared_ptr<ListNode> PListNode;
  }
}

#endif
