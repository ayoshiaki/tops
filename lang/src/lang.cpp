#include "lang.hpp"

#include "ToPSLang.hpp"

#include <iostream>
#include <string>

static int _nRow = 1;
static int _nCol = 1;

extern PConfigurationNode configurationNode;

std::string convertToken(std::string str, std::string token_name, std::string token) {
  size_t i = str.find(token_name);
  if (i != std::string::npos) {
    str = str.substr(0, i) + "'" + token + "'" + str.substr(i+token_name.size());
  }
  return str;
}

std::string convertTokens(const char *c) {
  std::string str(c);

  str = convertToken(str, "TIDENTIFIER", "identifier");
  str = convertToken(str, "TSTRING", "string");
  str = convertToken(str, "TINTEGER", "integer");
  str = convertToken(str, "TFLOAT", "float");
  str = convertToken(str, "TEQUAL", "=");
  str = convertToken(str, "TPIPE", "|");
  str = convertToken(str, "TCOLON", ":");
  str = convertToken(str, "TSEMICOLON", ";");
  str = convertToken(str, "TCOMMA", ",");
  str = convertToken(str, "TLPAREN", "(");
  str = convertToken(str, "TRPAREN", ")");
  str = convertToken(str, "TLSBRACKETS", "[");
  str = convertToken(str, "TRSBRACKETS", "]");
  str = convertToken(str, "$end", "\\n");

  return str;
}

extern
int nRow() {
  return _nRow;
}

extern
void nextRow() {
  _nRow++;
  _nCol = 1;
}

extern
void nextChar(int n) {
  _nCol += n;
}

extern
int nCol() {
  return _nCol;
}

extern
void printError(const char *errorstring, ...) {
  std::cerr << "error:" << nRow() << "," << nCol() << ": " << convertTokens(errorstring) << std::endl;
  exit(0);
}

extern void parse_lang(std::string str_input);

namespace tops {
  namespace lang {
    PConfigurationNode parse(std::string model) {
      parse_lang(model);
      return configurationNode;
    }
    
    ProbabilisticModelParametersPtr parse_model(std::string model) {
      
    }
  }
}

