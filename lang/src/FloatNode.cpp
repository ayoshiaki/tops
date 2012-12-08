#include "FloatNode.hpp"

#include <sstream>

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void FloatNode::accept(ToPSLangVisitor& visitor) {
      visitor.visitFloatNode(this);
    }
    
    std::string FloatNode::str() {
      return std::string("(FloatNode " + _str_value + ")");
    }
  }
}
