#include <gtest/gtest.h>

#include "ToPSLang.hpp"

using namespace tops::lang;

TEST(AST, Parser) {
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode training_algorithm) (StringNode \"PhasedMarkovChain\")))", 
            parse("training_algorithm=\"PhasedMarkovChain\"")->str());
             
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode alphabet) (ListNode (StringNode \"A\") (StringNode \"C\") (StringNode \"G\") (StringNode \"T\"))))", 
            parse("alphabet=(\"A\", \"C\", \"G\", \"T\")")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode order) (IntegerNode 4)))", 
            parse("order=4")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode begin) (ProbabilityMapNode (ProbabilityNode (StringNode \"order\") (IntegerNode 0)))))", 
            parse("begin=(\"order\":0)")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode p0) (ConditionalProbabilityMapNode (ConditionalProbabilityNode (ConditionNode (StringNode \"A\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"C\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"G\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"T\") (StringNode \"\")) (FloatNode 0.4)))))", 
            parse("p0 = (\"A\" | \"\" : 0.2; \"C\" | \"\" : 0.2; \"G\" | \"\" : 0.2; \"T\" | \"\" : 0.4;)")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode p0) (ConditionalProbabilityMapNode (ConditionalProbabilityNode (ConditionNode (StringNode \"A\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"C\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"G\") (StringNode \"\")) (FloatNode 0.2)) (ConditionalProbabilityNode (ConditionNode (StringNode \"T\") (StringNode \"\")) (FloatNode 0.4)))))", 
            parse("p0 = (\"A\" | \"\" : 0.2; \"C\" | \"\" : 0.2; \"G\" | \"\" : 0.2; \"T\" | \"\" : 0.4)")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode probabilities) (ListNode (IntegerNode 0) (FloatNode 0.00253024) (FloatNode 0.0113397) (FloatNode 0.0186961) (FloatNode 9.66823e-05) (FloatNode 9.66316e-05))))", 
            parse("probabilities = (0, 0.00253024, 0.0113397, 0.0186961, 9.66823e-05, 9.66316e-05)")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode transitions) (ConditionalProbabilityMapNode (ConditionalProbabilityNode (ConditionNode (StringNode \"NonCoding\") (StringNode \"NonCoding\")) (FloatNode 0.999)) (ConditionalProbabilityNode (ConditionNode (StringNode \"Start\") (StringNode \"NonCoding\")) (FloatNode 0.001)) (ConditionalProbabilityNode (ConditionNode (StringNode \"Coding\") (StringNode \"Start\")) (FloatNode 1.0)) (ConditionalProbabilityNode (ConditionNode (StringNode \"Stop\") (StringNode \"Coding\")) (FloatNode 1.0)) (ConditionalProbabilityNode (ConditionNode (StringNode \"NonCoding\") (StringNode \"Stop\")) (FloatNode 1.0)))))", 
            parse("transitions = ( \"NonCoding\" | \"NonCoding\": 0.999; \"Start\" | \"NonCoding\": 0.001; \"Coding\" | \"Start\": 1.0; \"Stop\" | \"Coding\": 1.0; \"NonCoding\" | \"Stop\": 1.0 )")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode noncoding_model) (ConfigurationNode (PropertyNode (KeyNode model_name) (StringNode \"MultinomialDistribution\")) (PropertyNode (KeyNode alphabet) (ListNode (StringNode \"A\") (StringNode \"C\") (StringNode \"G\") (StringNode \"T\"))) (PropertyNode (KeyNode probabilities) (ListNode (FloatNode 0.25) (FloatNode 0.25) (FloatNode 0.25) (FloatNode 0.25))))))", 
            parse("noncoding_model = [ model_name=\"MultinomialDistribution\" alphabet = (\"A\", \"C\", \"G\", \"T\") probabilities = (0.25, 0.25, 0.25, 0.25) ]")->str());
     
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode p0) (ConditionalProbabilityMapNode (ConditionalProbabilityNode (ConditionNode (StringNode \"A\") (StringNode \"\")) (FloatNode 0)) (ConditionalProbabilityNode (ConditionNode (StringNode \"C\") (StringNode \"\")) (FloatNode 0.181818)) (ConditionalProbabilityNode (ConditionNode (StringNode \"G\") (StringNode \"\")) (FloatNode 0.636364)) (ConditionalProbabilityNode (ConditionNode (StringNode \"T\") (StringNode \"\")) (FloatNode 0.181818)))))", 
           parse("p0 = (\"A\" | \"\" : 0; # leaf \n \"C\" | \"\" : 0.181818; # leaf \n \"G\" | \"\" : 0.636364; # leaf \n \"T\" | \"\" : 0.181818; # leaf \n )")->str());
  
  EXPECT_EQ("(ConfigurationNode (PropertyNode (KeyNode model_probabilities) (ProbabilityMapNode (ProbabilityNode (StringNode \"CPG\") (FloatNode 0.5)) (ProbabilityNode (StringNode \"NONCPG\") (FloatNode 0.5)))))", 
            parse("model_probabilities = (\"CPG\": 0.5; \"NONCPG\": 0.5)")->str());
}
