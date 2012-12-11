#include "IntegerNode.hpp"

#include <sstream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void IntegerNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitIntegerNode(this);
    }
    
    int IntegerNode::value() {
      int number;
      std::istringstream ss(_str_value);
      ss >> number;
      return number;
    }
    
    std::string IntegerNode::str() {
      return std::string("(IntegerNode " + _str_value + ")");
    }
  }
}
