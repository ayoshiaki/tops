#ifndef TOPS_LANG_FLOAT_HPP
#define TOPS_LANG_FLOAT_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"

namespace tops {
  namespace lang {
    class ToPSLangVisitor;
    
    class FloatNode: public ValueNode {
    public:
      FloatNode(std::string str_value):_str_value(str_value) {}
      virtual void accept(ToPSLangVisitor& visitor);
      virtual std::string str();
    private:
      std::string _str_value;
    };
    
    typedef boost::shared_ptr<FloatNode> PFloatNode;
  }
}

#endif
