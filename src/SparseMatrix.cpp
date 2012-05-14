#include "SparseMatrix.hpp"
#include <iostream>

using namespace std;

namespace tops{

  float SparseMatrix::postProbs_thresh = 0.001;
  float SparseMatrix::log_postProbs_thresh = log(0.001);

  void SparseMatrix::resize(int nrows, int ncols){
    M.resize(nrows);
    _nrows = nrows;
    _ncolumns = ncols;
  }

  int SparseMatrix::nrows(){
    return _nrows;
  }

  int SparseMatrix::ncols(){
    return _ncolumns;
  }

  void SparseMatrix::buildPredMatrix(int nrows, int ncols, Matrix &postProbs){
    _nrows = nrows+1;
    _ncolumns = ncols+1;
    M.resize(_nrows);
    for(int i = 0; i < nrows; i++){
      for(int j = 0; j < ncols; j++){
	if(postProbs(i,j) >= log_postProbs_thresh)
	  M[i+1][j+1] = exp(postProbs(i,j));
      }
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  bool SparseMatrix::next(int *i, int *j, float *prob){
    while(M[nextr].empty()){
      nextr++;
      nextc = M[nextr].begin();
      if(nextr == _nrows)
	return false;
    }
    *i = nextr;
    *j = nextc->first;
    *prob = nextc->second;
    nextc++;

    if(nextc == M[nextr].end() && nextr == _nrows-1){
      nextr = 0;
      nextc = M[0].begin();
      return false;
    }
    if(nextc == M[nextr].end() && nextr != _nrows-1){
      nextr++;
      nextc = M[nextr].begin();
    }
    return true;
  }

  vector< map<int,float> > &SparseMatrix::matrix(){
    return M;
  }

  map<int,float>::iterator SparseMatrix::lineBegin(int i){
    return M[i].begin();
  }
  map<int,float>::iterator SparseMatrix::lineEnd(int i){
    return M[i].end();
  }

  void SparseMatrix::getfMatrixTimesX(fMatrix &fM, float x){
    fM.resize(_nrows,_ncolumns);
    for(int i = 0; i < _nrows; i++){
      for(int j = 0; j < _ncolumns; j++){
	fM(i,j) = 0;
      }
    }
    for(int i = 0; i < _nrows; i++){
      for(map<int,float>::iterator it = M[i].begin(); it != M[i].end(); it++){
	fM(i,it->first) = x * it->second;
      }
    }
  }

  void SparseMatrix::removeLastLine(){
    M.pop_back();
    _nrows--;
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::removeLastColumn(){
    for(int i = 0; i < _nrows; i++){
      M[i].erase(_ncolumns-1);
    }
    _ncolumns--;
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::leftXright(SparseMatrixPtr &N, fMatrix &OUT, float n){
    if(ncols() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod matrices)" << endl;
      exit(-1);
    }
   
    map<int,float>::iterator it1;
    map<int,float>::iterator it2;
    
    for(int i = 0; i < nrows(); i++){
      for(it1 = M[i].begin(); it1 != M[i].end(); it1++){
	for(it2 = N->lineBegin(it1->first); it2 != N->lineEnd(it1->first); it2++){
	  OUT(i, it2->first) += it1->second * it2->second * n;
	}
      }
    }
  }

  void SparseMatrix::leftTransXright(SparseMatrixPtr &N, fMatrix &OUT, float n){
    if(nrows() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod matrices)" << endl;
      exit(-1);
    }
   
    map<int,float>::iterator it1;
    map<int,float>::iterator it2;
    
    for(int i = 0; i < nrows(); i++){
      for(it1 = M[i].begin(); it1 != M[i].end(); it1++){
	for(it2 = N->lineBegin(i); it2 != N->lineEnd(i); it2++){
	  OUT(it1->first, it2->first) += it1->second * it2->second * n;
	}
      }
    }
  }

  void SparseMatrix::leftXright(SparseMatrixPtr &N, fMatrix &OUT){
    if(ncols() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod matrices)" << endl;
      exit(-1);
    }
   
    map<int,float>::iterator it1;
    map<int,float>::iterator it2;
    
    for(int i = 0; i < nrows(); i++){
      for(it1 = M[i].begin(); it1 != M[i].end(); it1++){
	for(it2 = N->lineBegin(it1->first); it2 != N->lineEnd(it1->first); it2++){
	  OUT(i, it2->first) += it1->second * it2->second;
	}
      }
    }
  }

  void SparseMatrix::leftTransXright(SparseMatrixPtr &N, fMatrix &OUT){
    if(nrows() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod matrices)" << endl;
      exit(-1);
    }
   
    map<int,float>::iterator it1;
    map<int,float>::iterator it2;
    
    for(int i = 0; i < nrows(); i++){
      for(it1 = M[i].begin(); it1 != M[i].end(); it1++){
	for(it2 = N->lineBegin(i); it2 != N->lineEnd(i); it2++){
	  OUT(it1->first, it2->first) += it1->second * it2->second;
	}
      }
    }
  }

  void SparseMatrix::transposeOf(SparseMatrixPtr &A){
    M.resize(A->ncols());
    _ncolumns = A->nrows();
    _nrows = A->ncols();
    
    map<int,float>::iterator it;
    for(int i = 0; i < A->nrows(); i++){
      for(it = A->lineBegin(i); it != A->lineEnd(i); it++){
	M[it->first][i] = it->second;
      }
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::addGaps(SparseMatrixPtr ppGap1, SparseMatrixPtr ppGap2){
    vector<float> aux (_ncolumns, 0.0);
    map<int,float>::iterator it;
    resize(_nrows+1, _ncolumns+1);
    for(int i = 0; i < ppGap1->nrows(); i++){
      for(it = ppGap1->lineBegin(i); it != ppGap1->lineEnd(i); it++){
	  aux[it->first] += it->second;
      }
    }
    for(int i = 0; i < (int)aux.size(); i++)
      M[_nrows-1][i] = aux[i];
    
    for(int i = 0; i < ppGap2->nrows(); i++){
      float sum = 0.0;
      for(it = ppGap2->lineBegin(i); it != ppGap2->lineEnd(i); it++){
	  sum += it->second;
      }
      M[i][_ncolumns-1] = sum;
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::printMatrix(){
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it;
      for(it = M[i].begin(); it != M[i].end(); it++)
	cout << i << "-" << it->first << "-" << it->second << " ";
    }
  }
}
    
