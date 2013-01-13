# ToPS Language

Users can define probabilistic models by a manual description, and this document describes the ToPS Language

## Language Definition

### Tokens

The language defines the followings tokens:

IDENTIFIER         : [a-zA-Z_][a-zA-Z0-9_]*
STRING             : L?\"(\\.|[^\\"])*\"
COMMENTS           : "#"[^\r\n]*
FLOAT_POINT_NUMBER : [0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?
INTEGER_NUMBER     : [0-9]+

### BNF

model                          : properties
                               ;
                               
properties                     : property
                               | properties property
                               ;
                               
property                       : IDENTIFIER '=' value
                               ;
                               
value                          : STRING
                               | INTEGER_NUMBER
                               | FLOAT_POINT_NUMBER
                               | list
                               | probability_map
                               | conditional_probability_map
                               | sub_model
                               | IDENTIFIER
                               ;
                               
list                           : '(' list_elements ')'
                               ;
                               
list_elements                  : list_element
                               | list_elements ',' list_element
                               ;
                               
list_element                   : STRING
                               | INTEGER_NUMBER
                               | FLOAT_POINT_NUMBER
                               ;
                               
probability_map                : '(' probabilities_list ')'
                               ;
                               
probabilities_list             : probabilities
                               | probabilities ';'
                               ;
                               
probabilities                  : probability
                               | probabilities ';' probability
                               ;
                               
probability                    : TSTRING ':' list_element
                               ;

conditional_probability_map    : '(' conditional_probabilities_list ')'
                               ;

conditional_probabilities_list : conditional_probabilities
                               | conditional_probabilities ';'
                               ;

conditional_probabilities      : conditional_probability
                               | conditional_probabilities ';' conditional_probability
                               ;

conditional_probability        : condition ':' probability_number
                               ;

condition                      : STRING '|' STRING
                               ;

probability_number             : INTEGER_NUMBER
                               | FLOAT_POINT_NUMBER
                               ;

sub_model                      : '[' properties ']'
                               ;

## Examples

You can see some examples in the "example" folder.

