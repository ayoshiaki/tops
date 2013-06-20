#include "Consensus.hpp"

namespace tops {
  bool Consensus::is(int symbol) {
    for (std::vector<int>::iterator it = _symbols.begin() ; it != _symbols.end(); ++it) {
      if (*it == symbol)
        return true;
    }
    return false;
  }

  vector<double> Consensus::getFrequency() {
    return _frequency;
  }

  void ConsensusDependence::init() {

  }

  double ConsensusDependence::chi(int ci, int xi) {
    vector<double> freq_e = _consensus_sequence[ci].getFrequency();
    vector<double>  freq_o(freq_e.size(), 0);
    std::vector<Sequence>::iterator it;
    for (it = _sequences.begin(); it != _sequences.end(); it++) { 
      freq_o[(*it)[xi]] += 1;
    }

    double chi = 0;
    for (int i = 0; i < freq_e.size(); i++) {
      chi += pow(freq_e[i] - (freq_o[i]/_sequences.size()), 2)/freq_e[i];
    }

    return chi;
  }

  Matrix ConsensusDependence::dependences() {
    Matrix dep(_consensus_sequence.size(), _consensus_sequence.size());
    for (int i = 0; i < _consensus_sequence.size(); i++) {
      for (int j = 0; j < _consensus_sequence.size(); j++) {
        if (i != j) {
          dep(i, j) = chi(i, j);
        }
      }
    }
    return dep;
  }
}