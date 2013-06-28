#include "MaximalDependenceDecomposition.hpp"
#include "InhomogeneousMarkovChain.hpp"

namespace tops {
  int MaximalDependenceDecompositionNode::getIndex() {
    return _index;
  }

  ProbabilisticModelPtr MaximalDependenceDecompositionNode::getModel() {
    return _model;
  }

  void MaximalDependenceDecompositionNode::setChildern(MaximalDependenceDecompositionNodePtr left, MaximalDependenceDecompositionNodePtr right) {
    _left = left;
    _right = right;
  }

  MaximalDependenceDecompositionNodePtr MaximalDependenceDecompositionNode::getLeft() {
    return _left;
  }

  MaximalDependenceDecompositionNodePtr MaximalDependenceDecompositionNode::getRight() {
    return _right;
  }

  void MaximalDependenceDecomposition::setMDDTree(MaximalDependenceDecompositionNodePtr root) {
    _mdd_tree = root;
  }

  void MaximalDependenceDecomposition::setConsensusSequence(ConsensusSequence consensus_sequence) {
    _consensus_sequence = consensus_sequence;
  }

  double MaximalDependenceDecomposition::evaluate(const Sequence & s, unsigned int begin, unsigned int end) {
    vector<int> indexes;
    return _evaluateAux(s, _mdd_tree, indexes);
  }

  double MaximalDependenceDecomposition::_evaluateAux(const Sequence & s, MaximalDependenceDecompositionNodePtr node, vector<int> &indexes) {
    double p = 0;
    if (node->getLeft()) {
      p = node->getModel()->inhomogeneous()->evaluatePosition(s, node->getIndex(), node->getIndex());
      indexes.push_back(node->getIndex());
      // cout << node->getIndex() << endl;
      // cout << "tem filho" << endl;
      if (_consensus_sequence[node->getIndex()].is(s[node->getIndex()])) {
        // cout << "eh consensus" << endl;
        p += _evaluateAux(s, node->getLeft(), indexes);
      } else {
        // cout << "nao eh consensus" << endl;
        p += _evaluateAux(s, node->getRight(), indexes);
      }
    } else { // leaf
      // cout << "nao tem filho" << endl;
      for (int i = 0; i < s.size(); i++) {
        if (std::find(indexes.begin(), indexes.end(), i) == indexes.end()) {
          p += node->getModel()->inhomogeneous()->evaluatePosition(s, i, i);
        }
      }
    }
    return p;
  }

  Sequence & MaximalDependenceDecomposition::choose(Sequence & s, int size) {
    s = Sequence(size, -1);
    _chooseAux(s, _mdd_tree);
    return s;
  }

  int MaximalDependenceDecomposition::_chooseAux(Sequence & s, MaximalDependenceDecompositionNodePtr node) {
    if (node->getLeft()) {
      s[node->getIndex()] = node->getModel()->inhomogeneous()->choosePosition(s, node->getIndex(), node->getIndex());
      if (_consensus_sequence[node->getIndex()].is(s[node->getIndex()])) {
        _chooseAux(s, node->getLeft());
      } else {
        _chooseAux(s, node->getRight());
      }
    } else { // leaf
      for (int i = 0; i < s.size(); i++) {
        if (s[i] == -1) {
          s[i] = node->getModel()->inhomogeneous()->choosePosition(s, i, i);
        }
      }
    }
    return 0;
  }
}
