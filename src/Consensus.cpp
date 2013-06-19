#include "Consensus.hpp"

namespace tops {
  bool Consensus::is(int symbol) {
    for (std::vector<int>::iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      if (*it == symbol)
        return true;
    }
    return false;
  }

  double Consensus::getFrequency() {
    return _frequency;
  }
}