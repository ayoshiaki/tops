#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include "crossplatform.hpp"
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <sstream>

using namespace std;

namespace tops{
  
  class DLLEXPORT SparseMatrix{
  
  private:
    static float postProbs_thresh;
    static float cut_thresh;
    vector< map<int,float> > M;
    int _ncolumns;
    int _nrows;
    int nextr;
    map<int,float>::iterator nextc;
  
  public:
    typedef boost::shared_ptr<SparseMatrix> SparseMatrixPtr;
    SparseMatrix(){}
    SparseMatrix(int nrows, int ncols){
      M.resize(nrows);
      _nrows = nrows;
      _ncolumns = ncols;
      nextr = 0;
      nextc = M[0].begin();
    }
    static void setppthresh(float n){
      postProbs_thresh = n;
    }
    static void setcutthresh(float n){
      cut_thresh = n;
    }
    void resize(int nrows, int ncols);
    int nrows();
    int ncols();
    void add(int i, int j, float value);
    void addNoCutoff(int i, int j, float value);
    void remove(int i, int j);
    void sum(int i, int j, float value);
    void sum_times(SparseMatrixPtr N, float n);
    void cutoff_sum_times(SparseMatrixPtr N, float n);
    float get(int i, int j);
    bool next(int *i, int *j, float *prob);
    vector< map<int,float> > matrix();
    void matrix(vector< map<int,float> > N);
    //void prod(SparseMatrixPtr N, SparseMatrixPtr R);
    void sum_prod(SparseMatrixPtr N, SparseMatrixPtr R, float n);
    void sum_prod_trans(SparseMatrixPtr N, SparseMatrixPtr R, float n);
    void sum_trans_prod(SparseMatrixPtr N, SparseMatrixPtr R, float n);
    void prod(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n);
    void prod_trans(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n);
    void trans_prod(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n);
    void prod(float n, SparseMatrixPtr &OUT);
    /*void trans_prod(SparseMatrixPtr N, SparseMatrixPtr R);
    void prod_trans(SparseMatrixPtr N, SparseMatrixPtr R);*/
    void sum(SparseMatrixPtr N);
    void clear();
    void clean();
    void prodclean(float n);
    void clean(SparseMatrixPtr R);
    void printMatrix();
    void printMatrix(ostringstream &temp);
    void printFullMatrix();
    void readFromFile(ifstream fin);
    void removeLastLine();
    void removeLastColumn();
    void addGaps(SparseMatrixPtr ppGap1, SparseMatrixPtr ppGap2);
  };
  typedef boost::shared_ptr<SparseMatrix> SparseMatrixPtr;
};
#endif
    
      
