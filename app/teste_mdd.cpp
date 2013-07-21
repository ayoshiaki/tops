#include <iostream>
#include <vector>
#include <fstream>

#include "Consensus.hpp"
#include "ChiSquare.hpp"
#include "MaximalDependenceDecomposition.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ContextTree.hpp"

using namespace tops;
using namespace std;

int main (int argc, char ** argv)
{
  cout << "TesteMDD" << endl;

  /***********************************************************/
  // Alphabet
  AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
  alphabet->createSymbol("A");
  alphabet->createSymbol("C");
  alphabet->createSymbol("G");
  alphabet->createSymbol("T");

  /***********************************************************/
  // Consensus Sequence
  ConsensusSequence consensus_sequence;

  vector<int> s1;
  s1.push_back(0); s1.push_back(1); // a/c
  Consensus c1(s1);
  consensus_sequence.push_back(c1);

  int x1[] = {0, 2, 2, 3}; // AGGT
  for (int i = 0; i < 4; i++) {
    vector<int> s;
    s.push_back(x1[i]); 
    Consensus c(s);
    consensus_sequence.push_back(c);
  }

  vector<int> s2;
  s2.push_back(0); s2.push_back(2); // a/g
  Consensus c2(s2);
  consensus_sequence.push_back(c2);

  int x2[] = {0, 2, 3}; // agt
  for (int i = 0; i < 3; i++) {
    vector<int> s;
    s.push_back(x2[i]); 
    Consensus c(s);
    consensus_sequence.push_back(c);
  }

  for (std::vector<Consensus>::iterator it = consensus_sequence.begin() ; it != consensus_sequence.end(); ++it) {
    cout << it->str() << '\t';
  }
  cout << endl;

  /***********************************************************/
  // WMMs
  ProbabilisticModelCreatorClient creator;
  ProbabilisticModelPtr root = creator.create("_test2/root.txt");
  ProbabilisticModelPtr g5 = creator.create("_test2/g5.txt");
  ProbabilisticModelPtr h5 = creator.create("_test2/h5.txt");
  ProbabilisticModelPtr g5gm1 = creator.create("_test2/g5gm1.txt");
  ProbabilisticModelPtr g5hm1 = creator.create("_test2/g5hm1.txt");
  ProbabilisticModelPtr g5gm1am2 = creator.create("_test2/g5gm1am2.txt");
  ProbabilisticModelPtr g5gm1bm2 = creator.create("_test2/g5gm1bm2.txt");
  ProbabilisticModelPtr g5gm1am2u6 = creator.create("_test2/g5gm1am2u6.txt");
  ProbabilisticModelPtr g5gm1am2v6 = creator.create("_test2/g5gm1am2v6.txt");

  /***********************************************************/
  // MDD tree
  MaximalDependenceDecompositionNodePtr mdd_root = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("root", root, 7));
  MaximalDependenceDecompositionNodePtr mdd_g5 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5", g5, 2));
  MaximalDependenceDecompositionNodePtr mdd_h5 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("h5", h5, -1));
  MaximalDependenceDecompositionNodePtr mdd_g5gm1 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5gm1", g5gm1, 1));
  MaximalDependenceDecompositionNodePtr mdd_g5hm1 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5hm1", g5hm1, -1));
  MaximalDependenceDecompositionNodePtr mdd_g5gm1am2 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5gm1am2", g5gm1am2, 8));
  MaximalDependenceDecompositionNodePtr mdd_g5gm1bm2 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5gm1bm2", g5gm1bm2, -1));
  MaximalDependenceDecompositionNodePtr mdd_g5gm1am2u6 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5gm1am2u6", g5gm1am2u6, -1));
  MaximalDependenceDecompositionNodePtr mdd_g5gm1am2v6 = MaximalDependenceDecompositionNodePtr(
    new MaximalDependenceDecompositionNode("g5gm1am2v6", g5gm1am2v6, -1));

  mdd_root->setChildern(mdd_g5, mdd_h5);
  mdd_g5->setChildern(mdd_g5gm1, mdd_g5hm1);
  mdd_g5gm1->setChildern(mdd_g5gm1am2, mdd_g5gm1bm2);
  mdd_g5gm1am2->setChildern(mdd_g5gm1am2u6, mdd_g5gm1am2v6);

  /***********************************************************/
  // MDD definition

  MaximalDependenceDecompositionPtr mdd = MaximalDependenceDecompositionPtr(new MaximalDependenceDecomposition());
  mdd->setAlphabet(alphabet);
  mdd->setMDDTree(mdd_root);
  mdd->setConsensusSequence(consensus_sequence);
  ProbabilisticModelPtr consensus_model = creator.create("_test2/consensus_model.txt");
  mdd->setConsensusModel(consensus_model);

  /***********************************************************/
  // Evaluate

  Sequence s;
  s.push_back(1);s.push_back(0);s.push_back(2);s.push_back(2);s.push_back(3);s.push_back(2);s.push_back(0);s.push_back(0);s.push_back(3);
  cout << mdd->evaluate(s, 0, 9) << endl;

  /***********************************************************/
  // Choose

  srand(time(NULL));
  Sequence new_sequence;
  mdd->choose(new_sequence, 9);

  for (int i = 0; i < 9; i++) {
    cout << new_sequence[i] << "\t";
  }
  cout << endl;

  /***********************************************************/
  // Criando SequenceEntryList
  SequenceEntryList sequences;
  for (int i = 0; i < 10; i++) {
    SequenceEntryPtr se = SequenceEntryPtr(new SequenceEntry(alphabet));
    Sequence random_sequence;
    mdd->choose(random_sequence, 9);
    se->setSequence(random_sequence);
    se->setName("s");
    sequences.push_back(se);
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      cout << sequences[i]->getSequence()[j];
    }
    cout << endl;
  }

  /***********************************************************/
  // Treinando MDD
  MaximalDependenceDecompositionPtr trained_mdd = MaximalDependenceDecompositionPtr(new MaximalDependenceDecomposition());
  trained_mdd->setAlphabet(alphabet);
  trained_mdd->setConsensusSequence(consensus_sequence);

  // ProbabilisticModelPtr consensus_model = creator.create("_test2/consensus_model.txt");
  trained_mdd->setConsensusModel(consensus_model);

  trained_mdd->train(sequences, 2);

  cout << "-------------------------" << endl;
  Sequence new_sequence_from_trained_mdd;
  trained_mdd->choose(new_sequence_from_trained_mdd, 9);

  for (int i = 0; i < 9; i++) {
    cout << new_sequence_from_trained_mdd[i] << "\t";
  }
  cout << endl;

  // std::map<std::string, double> w;
  // ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
  // tree->initializeCounter(sequences, 0, w);
  // tree->normalize();

  // cout << tree->getContext(0)->getDistribution()->str() << endl;

  // cout << mdd->str() << endl;

  cout << "--------------------------" << endl;
  ProbabilisticModelPtr mdd_from_config = creator.create("_test2/mdd.txt");
  cout << mdd_from_config->str() << endl;

  return 0;
}





