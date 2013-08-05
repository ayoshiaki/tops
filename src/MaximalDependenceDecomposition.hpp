/*
 *       MaximalDependenceDecomposition.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef MAXIMAL_DEPENDENCE_DECOMPOSITION_HPP
#define MAXIMAL_DEPENDENCE_DECOMPOSITION_HPP
#include <boost/shared_ptr.hpp>

#include "crossplatform.hpp"
#include "util.hpp"
#include "Sequence.hpp"
#include "Consensus.hpp"
#include "ProbabilisticModel.hpp"
#include "InhomogeneousMarkovChain.hpp"

#include <iostream>
#include <string>
#include <vector>


using namespace std;


namespace tops {

  class MaximalDependenceDecompositionNode;
  typedef boost::shared_ptr<MaximalDependenceDecompositionNode> MaximalDependenceDecompositionNodePtr;

  class MaximalDependenceDecompositionNode {
  public:
    MaximalDependenceDecompositionNode(std::string node_name, ProbabilisticModelPtr model, int index):_model(model), _index(index), _node_name(node_name) {};

    int getIndex();
    ProbabilisticModelPtr getModel();

    void setChildern(MaximalDependenceDecompositionNodePtr left, MaximalDependenceDecompositionNodePtr right);
    MaximalDependenceDecompositionNodePtr getLeft();
    MaximalDependenceDecompositionNodePtr getRight();

    std::string tree_str();
    std::string model_str();
  private:
    vector<int> _otherIndexes;
    ProbabilisticModelPtr _model;
    int _index;
    std::string _node_name;
    MaximalDependenceDecompositionNodePtr _left;
    MaximalDependenceDecompositionNodePtr _right;
  };

  class MaximalDependenceDecomposition : public ProbabilisticModel {
  public:
    MaximalDependenceDecomposition() {};
    void setAlphabet(AlphabetPtr alphabet) {
      _alphabet = alphabet;
    }
    virtual AlphabetPtr alphabet() const
    {
      return _alphabet;
    }
    void setMDDTree(MaximalDependenceDecompositionNodePtr root);
    void setConsensusSequence(ConsensusSequence consensus_sequence);
    void setConsensusModel(ProbabilisticModelPtr model);

    InhomogeneousMarkovChainPtr trainInhomogeneousMarkovChain(SequenceEntryList & sequences);
    int getMaximalDependenceIndex(InhomogeneousMarkovChainPtr model, Sequence selected);
    void subset(int index, SequenceEntryList & sequences, SequenceEntryList & consensus, SequenceEntryList & nonconsensus);
    MaximalDependenceDecompositionNodePtr newNode(std::string node_name, SequenceEntryList & sequences, int divmin, Sequence selected);
    void train(SequenceEntryList & sequences, int divmin);

    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual Sequence & choose(Sequence & h, int size) const;

    virtual std::string model_name() const {
      return "MaximumDependenceDecomposition";
    }

    virtual std::string str () const ;

    virtual void initialize(const ProbabilisticModelParameters & parameters);
    MaximalDependenceDecompositionNodePtr initializeTree(const ProbabilisticModelParameters & parameters, std::vector<std::string>& tree);
  private:

    double _evaluateAux(const Sequence & s, MaximalDependenceDecompositionNodePtr node, vector<int> &indexes) const;
    void _chooseAux(Sequence & s, MaximalDependenceDecompositionNodePtr node) const;


    MaximalDependenceDecompositionNodePtr _mdd_tree;
    ConsensusSequence _consensus_sequence;
    ProbabilisticModelPtr _consensus_model;
    AlphabetPtr _alphabet;
  };

  typedef boost::shared_ptr<MaximalDependenceDecomposition> MaximalDependenceDecompositionPtr;
}

#endif
