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
    template<typename T>
    class ListNode: public ValueNode {
    public:
      ListNode(std::vector<T> v):_v(v) {}

      void addElement(T e) {
        _v.push_back(e);
      }

      virtual std::string str() {
        std::string str = "(ListNode";
        typename std::vector<T>::iterator it;
        for (it = _v.begin() ; it != _v.end(); ++it) {
          str += " " + (*it)->str();
        }
        str += ")";
        return str;
      }
    private:
      std::vector<T> _v;
    };
    
    typedef boost::shared_ptr<ListNode<PIntegerNode> > PListOfIntegersNode;
    typedef boost::shared_ptr<ListNode<PStringNode> > PListOfStringsNode;
    typedef boost::shared_ptr<ListNode<PFloatNode> > PListOfFloatsNode;
    typedef boost::shared_ptr<ListNode<PValueNode> > PListNode;
  }
}

#endif
