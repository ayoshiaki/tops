#include "ListNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    
    void ListNode::accept(ToPSLangVisitor& visitor) {
      std::vector<PValueNode>::iterator it;
      for (it = _v.begin() ; it != _v.end(); ++it) {
        (*it)->accept(visitor);
      }
      visitor.visitListNode(this);
    }
    
    void ListNode::addElement(PValueNode e) {
      _v.push_back(e);
    }

    std::string ListNode::str() {
      std::string str = "(ListNode";
      std::vector<PValueNode>::iterator it;
      for (it = _v.begin() ; it != _v.end(); ++it) {
        str += " " + (*it)->str();
      }
      str += ")";
      return str;
    }
  }
}