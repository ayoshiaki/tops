#include "MaximalDependenceDecomposition.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "ContextTree.hpp"

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

  std::string MaximalDependenceDecompositionNode::tree_str() {
    std::stringstream out;
    if (_left && _right) {
      out << "(";
      out << _node_name;
      out << ", ";
      out << _left->tree_str();
      out << ", ";
      out << _right->tree_str();
      out << ")";
    } else {
      out << _node_name;
    }
    return out.str();
  }

  std::string MaximalDependenceDecompositionNode::model_str() {
    std::stringstream out;
    out << _node_name << " = [" << endl;
    out << _model->str();
    out << "]" << endl;
    if (_left && _right) {
      out << _left->model_str();
      out << _right->model_str();
    }
    return out.str();
  }

  void MaximalDependenceDecomposition::setMDDTree(MaximalDependenceDecompositionNodePtr root) {
    _mdd_tree = root;
  }

  void MaximalDependenceDecomposition::setConsensusSequence(ConsensusSequence consensus_sequence) {
    _consensus_sequence = consensus_sequence;
  }

  double MaximalDependenceDecomposition::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
    vector<int> indexes;
    return _evaluateAux(s, _mdd_tree, indexes);
  }

  double MaximalDependenceDecomposition::_evaluateAux(const Sequence & s, MaximalDependenceDecompositionNodePtr node, vector<int> &indexes) const {
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

  Sequence & MaximalDependenceDecomposition::choose(Sequence & s, int size) const {
    s = Sequence(size, -1);
    _chooseAux(s, _mdd_tree);
    return s;
  }

  void MaximalDependenceDecomposition::_chooseAux(Sequence & s, MaximalDependenceDecompositionNodePtr node) const {
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
  }

  InhomogeneousMarkovChainPtr MaximalDependenceDecomposition::trainInhomogeneousMarkovChain(SequenceEntryList & sequences) {
    vector<ContextTreePtr> position_specific_context_trees;
    for (int j = 0; j < sequences[0]->getSequence().size(); j++) {
      SequenceEntryList imc_sequences;

      SequenceEntryPtr se = SequenceEntryPtr(new SequenceEntry());
      Sequence s;
      for (int i = 0; i < sequences.size(); i++) {
        s.push_back(sequences[i]->getSequence()[j]);
      }
      se->setSequence(s);
      se->setName("s");
      imc_sequences.push_back(se);

      std::map<std::string, double> w;
      ContextTreePtr tree = ContextTreePtr(new ContextTree(_alphabet));
      tree->initializeCounter(imc_sequences, 0, w);
      tree->normalize();
      position_specific_context_trees.push_back(tree);
    }
    InhomogeneousMarkovChainPtr model = InhomogeneousMarkovChainPtr(new InhomogeneousMarkovChain());
    model->setPositionSpecificDistribution(position_specific_context_trees);
    model->setAlphabet(_alphabet);
    return model;
  }

  int MaximalDependenceDecomposition::getMaximalDependenceIndex(InhomogeneousMarkovChainPtr model, Sequence selected) {
    Sequence s(_consensus_sequence.size(), -1);
    double maximal = -HUGE;
    double maximal_i = -1;
    for (int i = 0; i < _consensus_sequence.size(); i++) {
      double sum;
      for (int j = 0; j < _consensus_sequence.size(); j++) {
        if (i != j) {
          double x;
          double chi = -HUGE;
          for (int k = 0; k < _alphabet->size(); k++) {
            s[i] = k;
            double e = _consensus_model->inhomogeneous()->evaluatePosition(s, i, i);
            s[j] = k;
            double o = model->evaluatePosition(s, j, j);
            x = (o - e)+(o - e)-e;
            chi = log_sum(chi, x);
          }
          // cout << chi << "\t";
          sum = log_sum(sum, chi);
        } else {
          // cout << "-" << "\t";
        }
      }
      // cout << sum <<endl;
      if (maximal < sum) {
        bool ok = false;
        for (int k = 0; k < selected.size(); k++) {
          if (selected[k] == i) {
            ok = true;
            break;
          }
        }
        if (!ok) {
          maximal = sum;
          maximal_i = i;
        }
      }
    }
    return maximal_i;
  }

  void MaximalDependenceDecomposition::setConsensusModel(ProbabilisticModelPtr model) {
    _consensus_model = model;
  }

  void MaximalDependenceDecomposition::subset(int index, SequenceEntryList & sequences, SequenceEntryList & consensus, SequenceEntryList & nonconsensus) {
    for (int i = 0; i < sequences.size(); i++) {
      if (_consensus_sequence[index].is(sequences[i]->getSequence()[index])) {
        consensus.push_back(sequences[i]);
      } else {
        nonconsensus.push_back(sequences[i]);
      }
    }
  }

  MaximalDependenceDecompositionNodePtr MaximalDependenceDecomposition::newNode(std::string node_name, SequenceEntryList & sequences, int divmin, Sequence selected) {
    InhomogeneousMarkovChainPtr model = trainInhomogeneousMarkovChain(sequences);
    int consensus_index = getMaximalDependenceIndex(model, selected);
    selected.push_back(consensus_index);

    MaximalDependenceDecompositionNodePtr mdd_node;
    
    SequenceEntryList consensus_sequences;
    SequenceEntryList nonconsensus_sequences;
    subset(consensus_index, sequences, consensus_sequences, nonconsensus_sequences);

    // cout << "**********************************" << endl;
    // cout << "consensus_index = " << consensus_index << endl;
    // cout << "consensus_siquences = " << consensus_sequences.size() << endl;
    // cout << "nonconsensus_siquences = " << nonconsensus_sequences.size() << endl;

    if ((consensus_sequences.size() > divmin) && (nonconsensus_sequences.size() > divmin)) {
      mdd_node = MaximalDependenceDecompositionNodePtr(new MaximalDependenceDecompositionNode(node_name, model, consensus_index));
      std::stringstream p;
      p << "mdd_node_p" << consensus_index;
      MaximalDependenceDecompositionNodePtr left = newNode(p.str(), consensus_sequences, divmin, selected);
      std::stringstream n;
      n << "mdd_node_n" << consensus_index;
      MaximalDependenceDecompositionNodePtr right = newNode(n.str(), nonconsensus_sequences, divmin, selected);
      mdd_node->setChildern(left, right);
    } else {
      mdd_node = MaximalDependenceDecompositionNodePtr(new MaximalDependenceDecompositionNode(node_name, model, -1));
    }
    

    return mdd_node;
  }

  void MaximalDependenceDecomposition::train(SequenceEntryList & sequences, int divmin) {
    Sequence selected;
    setMDDTree(newNode("root", sequences, divmin, selected));
  }

  std::string MaximalDependenceDecomposition::str () const {
    std::stringstream out;

    out << "model_name = \"MaximalDependenceDecomposition\"" << endl;

    out << _alphabet->str();

    out << "consensus = (";
    for (int i = 0; i < _consensus_sequence.size(); i++) {
      out << _consensus_sequence[i].sym_str(_alphabet);
      if (i != (_consensus_sequence.size() - 1))
        out << ", ";
    }
    out << ")" << endl;

    out << "consensus_model = [" << endl;
    out << _consensus_model->str();
    out<< "]" << endl;

    out << _mdd_tree->model_str();
    out << "tree = " << _mdd_tree->tree_str() << endl;
    
    return out.str();
  }
}














