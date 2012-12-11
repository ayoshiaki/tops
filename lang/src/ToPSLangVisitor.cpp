#include "ToPSLangVisitor.hpp"

#include "ASTNode.hpp"
#include "ConditionalProbabilityMapNode.hpp"
#include "ConditionalProbabilityNode.hpp"
#include "ConditionNode.hpp"
#include "ConfigurationNode.hpp"
#include "FloatNode.hpp"
#include "IntegerNode.hpp"
#include "KeyNode.hpp"
#include "ListNode.hpp"
#include "ProbabilityMapNode.hpp"
#include "ProbabilityNode.hpp"
#include "PropertyNode.hpp"
#include "StringNode.hpp"
#include "ValueNode.hpp"

#include <iostream>

namespace tops {
  namespace lang {
    
    void ToPSLangVisitor::visitConditionalProbabilityMapNode(ConditionalProbabilityMapNode* node) {
      std::map<std::string, double > doubleProbmap;
      
      std::vector<ProbabilisticModelParameterValuePtr>::iterator it;
      for (it = _value_stack.begin() ; it != _value_stack.end(); ++it) {
        doubleProbmap[(*it)->getString()] = (*it)->getDouble();
      }
      _value_stack.clear();
      _value_stack.push_back(DoubleMapParameterValuePtr(new DoubleMapParameterValue(doubleProbmap)));
      std::cout << "visitConditionalProbabilityMapNode: " << _value_stack.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitConditionalProbabilityNode(ConditionalProbabilityNode* node) {
      double prob = _value_stack.back()->getDouble();
      _value_stack.pop_back();
      std::string cond = _value_stack.back()->getString();
      _value_stack.pop_back();
      StringDoubleValuePtr value = StringDoubleValuePtr(new StringDoubleValue(cond, prob));
      _value_stack.push_back(value);
      std::cout << "visitConditionalProbabilityNode: " << _value_stack.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitConditionNode(ConditionNode* node) {
      std::string str = _value_stack.back()->getString();
      _value_stack.pop_back();
      str = _value_stack.back()->getString() + "|" + str;
      _value_stack.pop_back();
      StringParameterValuePtr strValue = StringParameterValuePtr(new StringParameterValue(str));
      _value_stack.push_back(strValue);
      std::cout << "visitConditionNode: " << _value_stack.back()->str() << std::endl;
    }
    
    // TODO
    void ToPSLangVisitor::visitConfigurationNode(ConfigurationNode* node) {
      std::cout << "visitConfigurationNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitFloatNode(FloatNode* node) {
      std::cout << "visitFloatNode: " << node->value() << std::endl;
      _value_stack.push_back(DoubleParameterValuePtr(new DoubleParameterValue(node->value())));
    }
    
    void ToPSLangVisitor::visitIntegerNode(IntegerNode* node) {
      std::cout << "visitIntegerNode: " << node->value() << std::endl;
      _value_stack.push_back(IntParameterValuePtr(new IntParameterValue(node->value())));
    }
    
    void ToPSLangVisitor::visitKeyNode(KeyNode* node) {
      std::cout << "visitKeyNode: " << node->name() << std::endl;
      _key_name_stack.push_back(node->name());
    }
    
    void ToPSLangVisitor::visitListNode(ListNode* node) {
      std::cout << "visitListNode" << std::endl;
      
      std::vector<int> intValues;
      std::vector<double> doubleValues;
      std::vector<std::string> stringValues;
      
      std::vector<ProbabilisticModelParameterValuePtr>::iterator it;
      for (it = _value_stack.begin() ; it != _value_stack.end(); ++it) {
        if ((*it)->parameter_type() == "IntParameterValue") {
          intValues.push_back((*it)->getInt());
        } else if ((*it)->parameter_type() == "DoubleParameterValue") {
          doubleValues.push_back((*it)->getDouble());
        } else if ((*it)->parameter_type() == "StringParameterValue") {
          stringValues.push_back((*it)->getString());
        }
      }
      _value_stack.clear();
      if (intValues.size() > 0) {
        IntVectorParameterValuePtr intVector = IntVectorParameterValuePtr(new IntVectorParameterValue());
        intVector->initialize(intValues);
        _value_stack.push_back(intVector);
      } else if (doubleValues.size() > 0) {
        DoubleVectorParameterValuePtr doubleVector = DoubleVectorParameterValuePtr(new DoubleVectorParameterValue());
        doubleVector->initialize(doubleValues);
        _value_stack.push_back(doubleVector);
      } else if (stringValues.size() > 0) {
        StringVectorParameterValuePtr strVector = StringVectorParameterValuePtr(new StringVectorParameterValue());
        strVector->initialize(stringValues);
        _value_stack.push_back(strVector);
      }
      
    }
    
    void ToPSLangVisitor::visitProbabilityMapNode(ProbabilityMapNode* node) {
      std::cout << "visitProbabilityMapNode" << std::endl;
      
      std::map<std::string, double > doubleProbmap;
      std::map<std::string,std::string> strProbmap;
      
      std::vector<ProbabilisticModelParameterValuePtr>::iterator it;
      for (int i = 0; i < _value_stack.size(); i++) {
        if (_value_stack[i]->parameter_type() == "StringDoubleValue") {
          doubleProbmap[_value_stack[i]->getString()] = _value_stack[i]->getDouble();
        } else if (_value_stack[i]->parameter_type() == "StringParameterValue") {
          strProbmap[_value_stack[i]->getString()] = _value_stack[i+1]->getString();
          i++;
        }
      }
      _value_stack.clear();
      
      if (doubleProbmap.size() > 0) {
        _value_stack.push_back(DoubleMapParameterValuePtr(new DoubleMapParameterValue(doubleProbmap)));
      } else if (strProbmap.size() > 0) {
        _value_stack.push_back(StringMapParameterValuePtr(new StringMapParameterValue(strProbmap)));
      }
    }
    
    void ToPSLangVisitor::visitProbabilityNode(ProbabilityNode* node) {
      if (_value_stack.back()->parameter_type() == "IntParameterValue") {
        double value = _value_stack.back()->getInt();
        _value_stack.pop_back();
        std::string cond = _value_stack.back()->getString();
        _value_stack.pop_back();
        _value_stack.push_back(StringDoubleValuePtr(new StringDoubleValue(cond, value)));
      } else if (_value_stack.back()->parameter_type() == "DoubleParameterValue") {
        double value = _value_stack.back()->getDouble();
        _value_stack.pop_back();
        std::string cond = _value_stack.back()->getString();
        _value_stack.pop_back();
        _value_stack.push_back(StringDoubleValuePtr(new StringDoubleValue(cond, value)));
      } else if (_value_stack.back()->parameter_type() == "StringParameterValue") {
        // continue...
      }
      std::cout << "visitProbabilityNode: " << _value_stack.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitPropertyNode(PropertyNode* node) {
      std::cout << "visitPropertyNode: " << _key_name_stack.back() << " = " << _value_stack.back()->str() << std::endl;
      _parameters.add(_key_name_stack.back(), _value_stack.back());
      _key_name_stack.pop_back();
      _value_stack.pop_back();
    }
    
    void ToPSLangVisitor::visitStringNode(StringNode* node) {
      std::cout << "visitStringNode: " << node->value() << std::endl;
      _value_stack.push_back(StringParameterValuePtr(new StringParameterValue(node->value())));
    }
  }
}
