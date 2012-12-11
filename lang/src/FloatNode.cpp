#include "FloatNode.hpp"

#include <sstream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void FloatNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitFloatNode(this);
    }
    
    double FloatNode::value() {
      double number;
      std::istringstream ss(_str_value);
      ss >> number;
      return number;
    }
    
    std::string FloatNode::str() {
      return std::string("(FloatNode " + _str_value + ")");
    }
  }
}
