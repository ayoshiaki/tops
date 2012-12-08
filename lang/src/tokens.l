%option noyywrap

%{
#include "lang.hpp"
#include <string>
#include <iostream>

#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

void parse_lang(std::string str_input)
{
    YY_BUFFER_STATE buffer = yy_scan_string(str_input.c_str());
    yyparse();
    yy_delete_buffer(buffer);
}

%}

%%

[ \t]                             { nextChar(yyleng); }
[\n]                              { nextRow(); }
[a-zA-Z_][a-zA-Z0-9_]*            { nextChar(yyleng); SAVE_TOKEN; return TIDENTIFIER; }
L?\"(\\.|[^\\"])*\"               { nextChar(yyleng); SAVE_TOKEN; return TSTRING; }
"#"[^\r\n]*
[0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?  { nextChar(yyleng); SAVE_TOKEN; return TFLOAT; }
[0-9]+                            { nextChar(yyleng); SAVE_TOKEN; return TINTEGER; }
"="                               { nextChar(yyleng); return TOKEN(TEQUAL); }
"|"                               { nextChar(yyleng); return TOKEN(TPIPE); }
":"                               { nextChar(yyleng); return TOKEN(TCOLON); }
";"                               { nextChar(yyleng); return TOKEN(TSEMICOLON); }
","                               { nextChar(yyleng); return TOKEN(TCOMMA); }
"("                               { nextChar(yyleng); return TOKEN(TLPAREN); }
")"                               { nextChar(yyleng); return TOKEN(TRPAREN); }
"["                               { nextChar(yyleng); return TOKEN(TLSBRACKETS); }
"]"                               { nextChar(yyleng); return TOKEN(TRSBRACKETS); }
.                                 { printError((std::string("Unknown token: '") + std::string(yytext, yyleng) + "'").c_str()); }

%%
