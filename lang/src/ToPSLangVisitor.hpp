#ifndef TOPS_LANG_TOPSLANGVISITOR_HPP
#define TOPS_LANG_TOPSLANGVISITOR_HPP

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
      void visitASTNode(ASTNode* node);
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
      void visitValueNode(ValueNode* node);
    };
  }
}

#endif
