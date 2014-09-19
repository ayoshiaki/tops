#include "Consensus.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"

namespace tops {
  bool Consensus::is(int symbol) const {
    for (std::vector<int>::const_iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      if (*it == symbol)
        return true;
    }
    return false;
  }

  std::string Consensus::str() const {
    std::stringstream out;
    for (std::vector<int>::const_iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      out << (*it);
    }
    return out.str();
  }

  std::string Consensus::sym_str(AlphabetPtr alphabet) const {
    std::stringstream out;
    out << "\"";
    for (std::vector<int>::const_iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      out << alphabet->getSymbol(*it)->name();
      if ((it+1) != _symbols.end())
        out << " ";
    }
    out << "\"";
    return out.str();
  }
}