%{
#include "lang.hpp"
#include <iostream>
#include <string>

#define YYERROR_VERBOSE 1

PConfigurationNode configurationNode;

void yyerror(const char *s) { 
  printError(s);
}

%}

%union {
  int token;
  std::string *string;
  PConfigurationNode *configuration;
  PPropertyNode *property;
  PValueNode *value;
  PConditionalProbabilityMapNode *conditional_probability_map;
  PConditionalProbabilityNode *conditional_probability;
  PConditionNode *condition;
  PFloatNode *floatnumber;
  PListNode *list;
  PProbabilityMapNode *probability_map;
  PProbabilityNode *probability;
}

%token <string> TIDENTIFIER TSTRING TINTEGER TFLOAT
%token <token>  TEQUAL TPIPE TCOLON TSEMICOLON TCOMMA TLPAREN TRPAREN
%token <token>  TLSBRACKETS TRSBRACKETS

%type <configuration> model properties sub_model
%type <property> property
%type <value> value list_element
%type <conditional_probability_map> conditional_probability_map conditional_probabilities_list conditional_probabilities
%type <conditional_probability> conditional_probability;
%type <condition> condition;
%type <floatnumber> probability_number
%type <list> list list_elements
%type <probability_map> probability_map probabilities_list probabilities
%type <probability> probability

%start model

%%

model : properties { configurationNode = *$1; }
      ;

properties : property { 
               std::vector<PPropertyNode> prop;
               $$ = new PConfigurationNode(new ConfigurationNode(prop));
               (*$$)->addProperty(*$1);
             }
           | properties property {
               (*$1)->addProperty(*$2);
             }
           ;

property : TIDENTIFIER TEQUAL value {
             PKeyNode key = PKeyNode(new KeyNode(*$1));
             $$ = new PPropertyNode(new PropertyNode(key, *$3));
           }
         ;

value : TSTRING {
          $$ = new PValueNode(new StringNode(*$1));
        }
      | TINTEGER {
          $$ = new PValueNode(new IntegerNode(*$1));
        }
      | TFLOAT {
          $$ = new PValueNode(new FloatNode(*$1));
        }
      | list {
          $$ = new PValueNode(*$1);
        }
      | probability_map {
          $$ = new PValueNode(*$1);
        }
      | conditional_probability_map {
        $$ = new PValueNode(*$1);
      }
      | sub_model {
          $$ = new PValueNode(*$1);
        }
      | TIDENTIFIER {
          $$ = new PValueNode(new KeyNode(*$1));
        }
      ;

list : TLPAREN list_elements TRPAREN {
         $$ = $2;
       }
     ;

list_elements : list_element {
                  std::vector<PValueNode> values;
                  values.push_back(*$1);
                  $$ = new PListNode(new ListNode(values));
                }
              | list_elements TCOMMA list_element {
                  (*$1)->addElement(*$3);
                }
              ;

list_element : TSTRING {
                 $$ = new PValueNode(new StringNode(*$1));
               }
             | TINTEGER {
                 $$ = new PValueNode(new IntegerNode(*$1));
               }
             | TFLOAT {
                 $$ = new PValueNode(new FloatNode(*$1));
               }
             ;

probability_map : TLPAREN probabilities_list TRPAREN {
                    $$ = $2;
                  }
                ;

probabilities_list : probabilities {
                       $$ = $1;
                     }
                   | probabilities TSEMICOLON {
                       $$ = $1;
                     }
                   ;

probabilities : probability {
                  std::vector<PProbabilityNode> probs;
                  probs.push_back(*$1);
                  $$ = new PProbabilityMapNode(new ProbabilityMapNode(probs));
                }
              | probabilities TSEMICOLON probability {
                  (*$1)->addProbability(*$3);
                }
              ;

probability : TSTRING TCOLON list_elements {
                $$ = new PProbabilityNode(new ProbabilityNode(PStringNode(new StringNode(*$1)), *$3));
              }
            ;

conditional_probability_map : TLPAREN conditional_probabilities_list TRPAREN {
                                $$ = $2;
                              }
                            ;

conditional_probabilities_list : conditional_probabilities {
                                 $$ = $1
                               }
                               | conditional_probabilities TSEMICOLON {
                                 $$ = $1
                               }
                               ;

conditional_probabilities : conditional_probability {
                            std::vector<PConditionalProbabilityNode> conds;
                            conds.push_back(*$1);
                            $$ = new PConditionalProbabilityMapNode(new ConditionalProbabilityMapNode(conds));
                          }
                          | conditional_probabilities TSEMICOLON conditional_probability {
                              (*$1)->addConditionalProbability(*$3);
                            }
                          ;

conditional_probability : condition TCOLON probability_number {
                            $$ = new PConditionalProbabilityNode(new ConditionalProbabilityNode(*$1, *$3))
                          }
                        ;

condition : TSTRING TPIPE TSTRING {
              PStringNode c1 = PStringNode(new StringNode(*$1));
              PStringNode c2 = PStringNode(new StringNode(*$3));
              $$ = new PConditionNode(new ConditionNode(c1, c2));
            }
          ;

probability_number : TINTEGER {
                       $$ = new PFloatNode(new FloatNode(*$1));
                     }
                   | TFLOAT {
                       $$ = new PFloatNode(new FloatNode(*$1));
                     }
                   ;

sub_model : TLSBRACKETS properties TRSBRACKETS {
              $$ = $2;
            }
          ;

%%

