#ifndef TOPS_LANG_TOPSLANGVISITOR_HPP
#define TOPS_LANG_TOPSLANGVISITOR_HPP

#include <vector>
#include <string>
#include <sstream>

#include "ProbabilisticModelParameter.hpp"

namespace tops {
  namespace lang {
    
    class ASTNode;
    class ConditionalProbabilityMapNode;
    class ConditionalProbabilityNode;
    class ConditionNode;
    class ConfigurationNode;
    class FloatNode;
    class IntegerNode;
    class KeyNode;
    class ListNode;
    class ProbabilityMapNode;
    class ProbabilityNode;
    class PropertyNode;
    class StringNode;
    class ValueNode;
    
    class ToPSLangVisitor {
    public:
      void visitConditionalProbabilityMapNode(ConditionalProbabilityMapNode* node);
      void visitConditionalProbabilityNode(ConditionalProbabilityNode* node);
      void visitConditionNode(ConditionNode* node);
      void visitConfigurationNode(ConfigurationNode* node);
      void visitFloatNode(FloatNode* node);
      void visitIntegerNode(IntegerNode* node);
      void visitKeyNode(KeyNode* node);
      void visitListNode(ListNode* node);
      void visitProbabilityMapNode(ProbabilityMapNode* node);
      void visitProbabilityNode(ProbabilityNode* node);
      void visitPropertyNode(PropertyNode* node);
      void visitStringNode(StringNode* node);
      
    private:
      ProbabilisticModelParameters _parameters;
      std::vector<ProbabilisticModelParameterValuePtr> _value_stack;
      std::vector<std::string> _key_name_stack;
    };
    
    class StringDoubleValue: public ProbabilisticModelParameterValue {
    public:
      StringDoubleValue(){}
      StringDoubleValue(std::string key, double value):_key(key),_value(value) {}
      ~StringDoubleValue(){}
      virtual double getDouble() const {
        return _value;
      }
      virtual std::string getString() const {
        return _key;
      }
      virtual std::string parameter_type () const {
        return "StringDoubleValue";
      }
      virtual std::string str() const {
        std::stringstream ss;
        ss << _value;
        return _key + " : " + ss.str();
      }
    private:
      std::string _key;
      double _value;
    };
    
    typedef boost::shared_ptr <StringDoubleValue> StringDoubleValuePtr;
  }
}

#endif
