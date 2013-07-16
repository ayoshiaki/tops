#include "Consensus.hpp"

namespace tops {
  bool Consensus::is(int symbol) const {
    for (std::vector<int>::const_iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      if (*it == symbol)
        return true;
    }
    return false;
  }

  std::string Consensus::str() {
    std::stringstream out;
    for (std::vector<int>::iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      out << (*it);
    }
    return out.str();
  }
}