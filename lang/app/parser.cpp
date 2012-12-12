#include "ToPSLang.hpp"

#include <iostream>

#include "ToPSLangVisitor.hpp"

using namespace tops::lang;

int main() {
  std::string input, model;
  while (std::getline(std::cin, input)) {
    model += input + "\n";
  }
  std::cout << parse(model)->str() << std::endl;

  ToPSLangVisitor visitor;
  parse(model)->accept(visitor);

  return 0;
}
