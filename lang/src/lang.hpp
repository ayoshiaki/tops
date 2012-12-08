#ifndef TOPS_LANG_LANG_HPP
#define TOPS_LANG_LANG_HPP

#include "ConfigurationNode.hpp"
#include "PropertyNode.hpp"
#include "StringNode.hpp"
#include "IntegerNode.hpp"
#include "FloatNode.hpp"
#include "ListNode.hpp"
#include "ConditionalProbabilityMapNode.hpp"
#include "ProbabilityMapNode.hpp"
#include "ListNode.hpp"

using namespace tops::lang;

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char*);

extern void printError(const char *s, ...);

extern int nRow();
extern void nextRow();
extern void nextChar(int n);
extern int nCol();

#endif
