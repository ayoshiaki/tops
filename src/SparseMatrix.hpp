#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include "crossplatform.hpp"
#include "util.hpp"
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
    static float log_postProbs_thresh;
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

    SparseMatrix(int nrows, int ncols, fMatrix postProbs){
      M.resize(nrows);
      _nrows = nrows;
      _ncolumns = ncols;
      for(int i = 0; i < nrows; i++){
	for(int j = 0; j < ncols; j++){
	  if(postProbs(i,j) >= postProbs_thresh){
	    M[i][j] = postProbs(i,j);
	  }
	}
      }
      nextr = 0;
      nextc = M[0].begin();
    }

    SparseMatrix(fMatrix postProbs, SparseMatrixPtr reference, float n){
      M.resize(reference->nrows());
      _nrows = reference->nrows();
      _ncolumns = reference->ncols();
      map<int,float>::iterator it;
      for(int i = 0; i < _nrows; i++){
	for(it = reference->lineBegin(i); it != reference->lineEnd(i); it++){
	  if((postProbs(i,it->first)/n) >= postProbs_thresh){
	    M[i][it->first] = postProbs(i,it->first)/n;
	  }
	}
      }
      nextr = 0;
      nextc = M[0].begin();
    }

    SparseMatrix(SparseMatrixPtr reference){
      M.resize(reference->nrows());
      _nrows = reference->nrows();
      _ncolumns = reference->ncols();
      map<int,float>::iterator it;
      for(int i = 0; i < _nrows; i++){
	for(it = reference->lineBegin(i); it != reference->lineEnd(i); it++){
	  M[i][it->first] = it->second;
	}
      }
      nextr = 0;
      nextc = M[0].begin();
    }    

    static void setppthresh(float n){
      postProbs_thresh = n;
      log_postProbs_thresh = log(n);
    }

    void buildPredMatrix(int nrows, int ncols, Matrix &postProbs);
    void getfMatrixTimesX(fMatrix &fM, float x);
    void leftXright(SparseMatrixPtr &N, fMatrix &OUT, float n);
    void leftTransXright(SparseMatrixPtr &N, fMatrix &OUT, float n);
    void leftXright(SparseMatrixPtr &N, fMatrix &OUT);
    void leftTransXright(SparseMatrixPtr &N, fMatrix &OUT);
    void transposeOf(SparseMatrixPtr &A);
    void resize(int nrows, int ncols);
    int nrows();
    int ncols();
    bool next(int *i, int *j, float *prob);
    vector< map<int,float> > &matrix();
    map<int,float>::iterator lineBegin(int i);
    map<int,float>::iterator lineEnd(int i);
    void printMatrix();
    void removeLastLine();
    void removeLastColumn();
    void addGaps(SparseMatrixPtr ppGap1, SparseMatrixPtr ppGap2);
  };
  typedef boost::shared_ptr<SparseMatrix> SparseMatrixPtr;
};
#endif
    
      
