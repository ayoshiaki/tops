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
      std::cout << "visitConditionalProbabilityMapNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitConditionalProbabilityNode(ConditionalProbabilityNode* node) {
      std::cout << "visitConditionalProbabilityNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitConditionNode(ConditionNode* node) {
      std::cout << "visitConditionNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitConfigurationNode(ConfigurationNode* node) {
      std::cout << "visitConfigurationNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitFloatNode(FloatNode* node) {
      std::cout << "visitFloatNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitIntegerNode(IntegerNode* node) {
      std::cout << "visitIntegerNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitKeyNode(KeyNode* node) {
      std::cout << "visitKeyNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitListNode(ListNode* node) {
      std::cout << "visitListNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitProbabilityMapNode(ProbabilityMapNode* node) {
      std::cout << "visitProbabilityMapNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitProbabilityNode(ProbabilityNode* node) {
      std::cout << "visitProbabilityNode" << std::endl;
    }
    
    void ToPSLangVisitor::visitPropertyNode(PropertyNode* node) {
      std::cout << "visitPropertyNode" <<  std::endl;
    }
    
    void ToPSLangVisitor::visitStringNode(StringNode* node) {
      std::cout << "visitStringNode" << std::endl;
    }
  }
}
