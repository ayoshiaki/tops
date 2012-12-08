#ifndef TOPS_LANG_TOPSLANG_HPP
#define TOPS_LANG_TOPSLANG_HPP

#include "ConfigurationNode.hpp"

namespace tops {
  namespace lang {
    PConfigurationNode parse(std::string model);
    //ProbabilisticModelParametersPtr parse_model(std::string model);
  }
}

#endif
