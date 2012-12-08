#include <gtest/gtest.h>

#include "ASTNode.hpp"
#include "PropertyNode.hpp"
#include "KeyNode.hpp"
#include "ValueNode.hpp"
#include "StringNode.hpp"
#include "IntegerNode.hpp"
#include "FloatNode.hpp"
#include "ConditionNode.hpp"
#include "ConditionalProbabilityNode.hpp"
#include "ConditionalProbabilityMapNode.hpp"
#include "ConfigurationNode.hpp"
#include "ListNode.hpp"
#include "ProbabilityMapNode.hpp"
#include "ProbabilityNode.hpp"

using namespace tops::lang;

TEST(AST, Node) {
  PASTNode node = PASTNode(new ASTNode());
  EXPECT_EQ("(ASTNode)", node->str());
}

TEST(AST, Property) {
  PPropertyNode property = PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName")), 
    PValueNode(new ValueNode())));
  EXPECT_EQ("(PropertyNode (KeyNode KeyName) (ValueNode))", property->str());
  
  property = PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName")), 
    PStringNode(new StringNode("StringValue"))));
  EXPECT_EQ("(PropertyNode (KeyNode KeyName) (StringNode StringValue))", property->str());
  
  property = PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName")), 
    PIntegerNode(new IntegerNode("123"))));
  EXPECT_EQ("(PropertyNode (KeyNode KeyName) (IntegerNode 123))", property->str());
  
  property = PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName")), 
    PFloatNode(new FloatNode("1.23"))));
  EXPECT_EQ("(PropertyNode (KeyNode KeyName) (FloatNode 1.23))", property->str());
  
  std::vector<PPropertyNode> props;
  props.push_back(PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName1")), 
    PStringNode(new StringNode("StringValue1")))));
  props.push_back(PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName2")), 
    PStringNode(new StringNode("StringValue2")))));
  PConfigurationNode config = PConfigurationNode(new ConfigurationNode(props));
  property = PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName")),
    config));
  EXPECT_EQ("(PropertyNode (KeyNode KeyName) (ConfigurationNode (PropertyNode (KeyNode KeyName1) (StringNode StringValue1)) (PropertyNode (KeyNode KeyName2) (StringNode StringValue2))))", property->str());
}

TEST(AST, String) {
  PStringNode str = PStringNode(new StringNode("StringValue"));
  EXPECT_EQ("(StringNode StringValue)", str->str());
}

TEST(AST, Integer) {
  PIntegerNode integer = PIntegerNode(new IntegerNode("123"));
  EXPECT_EQ("(IntegerNode 123)", integer->str());
}

TEST(AST, Float) {
  PFloatNode floatnumber = PFloatNode(new FloatNode("1.23"));
  EXPECT_EQ("(FloatNode 1.23)", floatnumber->str());
}

TEST(AST, Condition) {
  PConditionNode cond = PConditionNode(new ConditionNode(
    PStringNode(new StringNode("StringValue1")), 
    PStringNode(new StringNode("StringValue2"))));
  EXPECT_EQ("(ConditionNode (StringNode StringValue1) (StringNode StringValue2))", cond->str());
}

TEST(AST, ConditionalProbability) {
  PConditionNode cond = PConditionNode(new ConditionNode(
    PStringNode(new StringNode("StringValue1")), 
    PStringNode(new StringNode("StringValue2"))));
  PFloatNode floatnumber = PFloatNode(new FloatNode("1.23"));
  PConditionalProbabilityNode condprob = PConditionalProbabilityNode(
    new ConditionalProbabilityNode(cond, floatnumber));
  EXPECT_EQ("(ConditionalProbabilityNode (ConditionNode (StringNode StringValue1) (StringNode StringValue2)) (FloatNode 1.23))", condprob->str());
}

TEST(AST, ConditionalProbabilityMap) {
  PConditionNode cond = PConditionNode(new ConditionNode(
    PStringNode(new StringNode("StringValue1")), 
    PStringNode(new StringNode("StringValue2"))));
  PFloatNode floatnumber = PFloatNode(new FloatNode("1.23"));
  PConditionalProbabilityNode condprob = PConditionalProbabilityNode(
    new ConditionalProbabilityNode(cond, floatnumber));
    
  PConditionNode cond2 = PConditionNode(new ConditionNode(
    PStringNode(new StringNode("StringValue1")), 
    PStringNode(new StringNode("StringValue2"))));
  PFloatNode floatnumber2 = PFloatNode(new FloatNode("1.23"));
  PConditionalProbabilityNode condprob2 = PConditionalProbabilityNode(
    new ConditionalProbabilityNode(cond2, floatnumber2));
  
  std::vector<PConditionalProbabilityNode> conds;
  conds.push_back(condprob),
  conds.push_back(condprob2);
  
  PConditionalProbabilityMapNode condmap = PConditionalProbabilityMapNode(new ConditionalProbabilityMapNode(conds));
  
  EXPECT_EQ("(ConditionalProbabilityMapNode (ConditionalProbabilityNode (ConditionNode (StringNode StringValue1) (StringNode StringValue2)) (FloatNode 1.23)) (ConditionalProbabilityNode (ConditionNode (StringNode StringValue1) (StringNode StringValue2)) (FloatNode 1.23)))", condmap->str());
}

TEST(AST, Configuration) {
  std::vector<PPropertyNode> props;
  props.push_back(PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName1")), 
    PStringNode(new StringNode("StringValue1")))));
  props.push_back(PPropertyNode(new PropertyNode(
    PKeyNode(new KeyNode("KeyName2")), 
    PStringNode(new StringNode("StringValue2")))));
  PConfigurationNode config = PConfigurationNode(new ConfigurationNode(props));
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode KeyName1) (StringNode StringValue1)) (PropertyNode (KeyNode KeyName2) (StringNode StringValue2)))", config->str());
}

TEST(AST, ListOfIntegers) {
  std::vector<PValueNode> integers;
  integers.push_back(PIntegerNode(new IntegerNode("123")));
  integers.push_back(PIntegerNode(new IntegerNode("345")));
  PListNode list = PListNode(new ListNode(integers));
  EXPECT_EQ("(ListNode (IntegerNode 123) (IntegerNode 345))", list->str());
}

TEST(AST, ListOfFloats) {
  std::vector<PValueNode> floats;
  floats.push_back(PFloatNode(new FloatNode("1.23")));
  floats.push_back(PFloatNode(new FloatNode("34.5")));
  PListNode list = PListNode(new ListNode(floats));
  EXPECT_EQ("(ListNode (FloatNode 1.23) (FloatNode 34.5))", list->str());
}

TEST(AST, ListOfString) {
  std::vector<PValueNode> strings;
  strings.push_back(PStringNode(new StringNode("str1")));
  strings.push_back(PStringNode(new StringNode("str2")));
  PListNode list = PListNode(new ListNode(strings));
  EXPECT_EQ("(ListNode (StringNode str1) (StringNode str2))", list->str());
}

TEST(AST, List) {
  std::vector<PValueNode> values;
  values.push_back(PValueNode(new StringNode("str1")));
  values.push_back(PValueNode(new IntegerNode("222")));
  PListNode list = PListNode(new ListNode(values));
  EXPECT_EQ("(ListNode (StringNode str1) (IntegerNode 222))", list->str());
}

TEST(AST, Probability) {
  PProbabilityNode prob = PProbabilityNode(new ProbabilityNode(
    PStringNode(new StringNode("a")), 
    PFloatNode(new FloatNode("1.2"))));
  EXPECT_EQ("(ProbabilityNode (StringNode a) (FloatNode 1.2))", prob->str());
}

TEST(AST, ProbabilityMap) {
  std::vector<PProbabilityNode> probs;
  probs.push_back(PProbabilityNode(new ProbabilityNode(
    PStringNode(new StringNode("a")), 
    PFloatNode(new FloatNode("1.2")))));
  probs.push_back(PProbabilityNode(new ProbabilityNode(
    PStringNode(new StringNode("b")), 
    PFloatNode(new FloatNode("2.3")))));
  PProbabilityMapNode probmap = PProbabilityMapNode(new ProbabilityMapNode(probs));
  EXPECT_EQ("(ProbabilityMapNode (ProbabilityNode (StringNode a) (FloatNode 1.2)) (ProbabilityNode (StringNode b) (FloatNode 2.3)))", probmap->str());
}

