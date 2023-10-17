#ifndef MULTIPLE_ALIGNMENT_HPP
#define MULTIPLE_ALIGNMENT_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <stack>
#include <boost/timer/timer.hpp>
#include "ProbabilisticModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"

using namespace std;

namespace tops{
  
  typedef pair<int,int> seqPos;

  typedef struct{
    seqPos pos1;
    seqPos pos2;
    float prob;} postProb;
  
  typedef struct{
    int index;
    bool visited;
    bool isDead;
    vector<seqPos> colSeqPos;} column;    

  class DLLEXPORT MultipleAlignment{

  private:
    int _numberOfColumns;
    vector<column> _graph;
    map< seqPos,int > _seqPosMap; //isPresent[i][j] = -1 if residue j if sequence is is not present and isPresent[i][j] = m if it is present in node m
    list<postProb> _postProbs;
    SequenceList _seqs;
    vector<string> _names;
    int _gap_id;
    vector<SequenceEntry> _alignment;
    map<string,map<string,SparseMatrixPtr > > _ppAlign;
    map<string,map<string,SparseMatrixPtr > > _ppGap1;
    map<string,map<string,SparseMatrixPtr > > _ppGap2;
    map<string, SparseMatrixPtr > _ppPred;
    map<string,map<string,float> > _eas;


  public:
    MultipleAlignment(){}
    void computeOneAlignment(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, int consScheme);
    void computeAllAlignments(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, string alFileName, string outDir);
    void computePredictionsAndAlignment(ProbabilisticModelPtr almodel, vector<ProbabilisticModelPtr> &predmodels, SequenceList seqs, vector<string> names, int numit, bool alternateConsistencies, string outDir);
    //void initializeFromFile(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, int consScheme, string inputFileName);
    //void trainAndComputePPs(string initialModelFile, SequenceList seqs, vector<string> names, int maxTainIter, string outFile);
    float postProbAlign(ProbabilisticModelPtr model, map<string, map< string, SparseMatrixPtr > > &ppAlign, map<string, map< string, SparseMatrixPtr > > &ppGap1, map<string, map< string, SparseMatrixPtr > > &ppGap2, map<string, map<string, float> > &eas);
    void picxaaAlConsistency(map< string, map< string, SparseMatrixPtr > > &ppAlign,  map<string, map<string, float> > eas, int numit);
    void classicAlConsistency(map< string, map< string, SparseMatrixPtr > > &ppAlign, int numit);
    void predalignAlConsistencyWithEas(map< string, map< string, SparseMatrixPtr > > &ppAlign, map< string, map< string, SparseMatrixPtr > > &ppGap1,map< string, map< string, SparseMatrixPtr > > &ppGap2, int numit, map<string, map<string, float> > eas);
    void predalignAlConsistencyNoEas(map< string, map< string, SparseMatrixPtr > > &ppAlign, map< string, map< string, SparseMatrixPtr > > &ppGap1,map< string, map< string, SparseMatrixPtr > > &ppGap2, int numit);
    void makeAlignmentConsistentWithPrediction(map<string, SparseMatrixPtr > &ppPred, map< string, map< string, SparseMatrixPtr > > &ppAlign, int numit);
    void makePredictionConsistentWithAlignment(map<string, SparseMatrixPtr > &ppPred, map< string, map< string, SparseMatrixPtr > > &ppAlign, int numit);
    void postProbPred(vector<ProbabilisticModelPtr> &predmodels, map<string, SparseMatrixPtr > &ppPred);
    void initializePostProbsList(map< string, map< string, SparseMatrixPtr > > &ppAlign);
    void addList(postProb p);
    void printPPList();
    void generateGraph();
    void addToGraph(postProb p);
    void addToGappedGraph(postProb p);
    bool dfsf(int ub, column n, vector<column> &df);
    void dfsb(int lb, column n, vector<column> &db);
    void merge(column *ub, column *lb);
    void reorder(vector<column> &df, vector<column> &db);
    void findGraphMcd();
    void unvisit(vector<column> &v);
    void generateAlignment(ProbabilisticModelPtr almodel);
    void clearAll();
  };
  typedef boost::shared_ptr<MultipleAlignment> MultipleAlignmentPtr;
}
#endif      
