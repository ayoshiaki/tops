#include "ToPSLang.hpp"

#include <iostream>

using namespace tops::lang;

int main() {
  std::string input, model;
  while (std::getline(std::cin, input)) {
    model += input + "\n";
  }
  std::cout << parse(model)->str() << std::endl;
  return 0;
}
