#include "MaximalDependenceDecomposition.hpp"

namespace tops {
  int MaximalDependenceDecompositionNode::getSymbol() {
    return _symbol;
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

  
}