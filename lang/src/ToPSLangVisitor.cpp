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
    
    // TODO
    void ToPSLangVisitor::visitConditionalProbabilityMapNode(ConditionalProbabilityMapNode* node) {
      std::cout << "visitConditionalProbabilityMapNode" << std::endl;
    }
    
    // TODO
    void ToPSLangVisitor::visitConditionalProbabilityNode(ConditionalProbabilityNode* node) {
      std::cout << "visitConditionalProbabilityNode" << std::endl;
    }
    
    // TODO
    void ToPSLangVisitor::visitConditionNode(ConditionNode* node) {
      std::cout << "visitConditionNode" << std::endl;
    }
    
    // TODO
    void ToPSLangVisitor::visitConfigurationNode(ConfigurationNode* node) {
      std::cout << "visitConfigurationNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitFloatNode(FloatNode* node) {
      std::cout << "visitFloatNode" << std::endl;
      _values.push_back(DoubleParameterValue2Ptr(new DoubleParameterValue2(node->value())));
      std::cout << "  * " << _values.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitIntegerNode(IntegerNode* node) {
      std::cout << "visitIntegerNode" << std::endl;
      _values.push_back(IntegerParameterValue2Ptr(new IntegerParameterValue2(node->value())));
      std::cout << "  * " << _values.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitKeyNode(KeyNode* node) {
      std::cout << "visitKeyNode" << std::endl;
      _keys.push_back(node->name());
      std::cout << "  * " << _keys.back() << std::endl;
    }
    
    // TODO: verificar se a lista é uniforme
    void ToPSLangVisitor::visitListNode(ListNode* node) {
      std::cout << "visitListNode" << std::endl;

      std::vector<int> intVector;
      std::vector<double> doubleVector;
      std::vector<std::string> strVector;

      for (int i = 0; i < _values.size(); i ++) {
        if (_values[i]->parameterType() == "IntegerParameterValue2") {
          intVector.push_back(_values[i]->toIntegerParameter()->value());
        } else if (_values[i]->parameterType() == "DoubleParameterValue2") {
          doubleVector.push_back(_values[i]->toDoubleParameter()->value());
        } else if (_values[i]->parameterType() == "StringParameterValue2") {
          strVector.push_back(_values[i]->toStringParameter()->value());
        }
      }
      _values.clear();
      if (intVector.size() > 0) {
        _values.push_back(IntegerListParameterValue2Ptr(new IntegerListParameterValue2(intVector)));
      } else if (doubleVector.size() > 0) {
        _values.push_back(DoubleListParameterValue2Ptr(new DoubleListParameterValue2(doubleVector)));
      } else if (strVector.size() > 0) {
        _values.push_back(StringListParameterValue2Ptr(new StringListParameterValue2(strVector)));
      }
      std::cout << "  * " << _values.back()->str() << std::endl;
    }
    
    // TODO invalid parameter
    void ToPSLangVisitor::visitProbabilityMapNode(ProbabilityMapNode* node) {
      std::cout << "visitProbabilityMapNode" << std::endl;
      std::map<std::string, double> probMap;
      for (int i = 0; i < _values.size(); i += 2) {
        double value;
        if (_values[i+1]->parameterType() == "IntegerParameterValue2") {
          value = _values[i+1]->toIntegerParameter()->value();
        } else if (_values[i+1]->parameterType() == "DoubleParameterValue2") {
          value = _values[i+1]->toDoubleParameter()->value();
        }
        std::string key = _values[i]->toStringParameter()->value();
        probMap[key] = value;
      }
      _values.push_back(ProbabilityParameterValue2Ptr(new ProbabilityParameterValue2(probMap)));
      std::cout << "  * " << _values.back()->str() << std::endl;
    }

    void ToPSLangVisitor::visitProbabilityNode(ProbabilityNode* node) {
      std::cout << "visitProbabilityNode" << std::endl;
      std::cout << "  * " << (*(_values.end()-2))->str() << ": " << _values.back()->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitPropertyNode(PropertyNode* node) {
      std::cout << "visitPropertyNode" <<  std::endl;

      ProbabilisticModelParameterValue2Ptr v = _values.back();
      _values.pop_back();

      std::string k = _keys.back();
      _keys.pop_back();

      add_parameter(k, v);
      std::cout << "  * " << k << " = " << _parameters.back()[k]->str() << std::endl;
    }
    
    void ToPSLangVisitor::visitStringNode(StringNode* node) {
      std::cout << "visitStringNode" << std::endl;
      _values.push_back(StringParameterValue2Ptr(new StringParameterValue2(node->value())));
      std::cout << "  * " << _values.back()->str() << std::endl;
    }

    void ToPSLangVisitor::add_parameter(std::string key, ProbabilisticModelParameterValue2Ptr value) {
      if (_keys.size() == _parameters.size())
        _parameters.push_back(ProbabilisticModelParameterMap());
      _parameters.back()[key] = value;
    }
  }
}
