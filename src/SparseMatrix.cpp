#include "SparseMatrix.hpp"
#include <iostream>

using namespace std;

namespace tops{

  float SparseMatrix::postProbs_thresh = 0.001;
  float SparseMatrix::cut_thresh = 0.01;

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

  void SparseMatrix::addNoCutoff(int i, int j, float value){
    if(i < 0 || i >= _nrows || 0 > j || j >= _ncolumns){
      cout << "Bad index when adding in sparse matrix!" << endl;
      cout << "nrows = " << _nrows << " ncols = " << _ncolumns << " i = " << i << " j = " << j << endl;
      exit(-1);
    }
    if(value == 0)
      return;
    M[i][j] = value;
  }

  void SparseMatrix::add(int i, int j, float value){
    if(value < postProbs_thresh)
      return;
    if(i < 0 || i >= _nrows || 0 > j || j >= _ncolumns){
      cout << "Bad index when adding in sparse matrix!" << endl;
      cout << "nrows = " << _nrows << " ncols = " << _ncolumns << " i = " << i << " j = " << j << endl;
      exit(-1);
    }
    if(value == 0)
      return;
    M[i][j] = value;
  }

  void SparseMatrix::remove(int i, int j){
    if(i < 0 || i >= _nrows || 0 > j || j >= _ncolumns){
      cout << "Bad index when removing from sparse matrix!" << endl;
      cout << "nrows = " << _nrows << " ncols = " << _ncolumns << " i = " << i << " j = " << j << endl;
      exit(-1);
    }
    M[i].erase(j);
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

  void SparseMatrix::sum(int i, int j, float value){
    map<int,float>::iterator it = M[i].find(j);
    if(it != M[i].end())
      it->second += value;
    else
      addNoCutoff(i,j,value);
  }

  float SparseMatrix::get(int i, int j){
    if(i < 0 || i >= _nrows || 0 > j || j >= _ncolumns){
      cout << "Bad index when adding in sparse matrix!" << endl;
      exit(-1);
    }
    map<int,float>::iterator it = M[i].find(j);
    if(it != M[i].end())
      return it->second;
    else
      return 0.0;
  }
  
  vector< map<int,float> > SparseMatrix::matrix(){
    return M;
  }

  void SparseMatrix::matrix(vector< map<int,float> > N){
    M = N;
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::clear(){
    for(int i = 0; i < (int)M.size(); i++)
      M[i].clear();
    M.clear();
  }

  void SparseMatrix::clean(){
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it = M[i].begin();
      while(it != M[i].end()){
	map<int,float>::iterator itaux = it;
	it++;
	if(itaux->second < postProbs_thresh){
	  M[i].erase(itaux);
	}
      }
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::clean(SparseMatrixPtr R){
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it = M[i].begin();
      while(it != M[i].end()){
	map<int,float>::iterator itaux = it;
	it++;
	if(itaux->second < postProbs_thresh || R->get(i,itaux->first) == 0){
	  M[i].erase(itaux);
	}
      }
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::prodclean(float n){
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it = M[i].begin();
      while(it != M[i].end()){
	map<int,float>::iterator itaux = it;
	it++;
	itaux->second *= n;
	if(itaux->second < postProbs_thresh){
	  M[i].erase(itaux);
	}
      }
    }
    nextr = 0;
    nextc = M[0].begin();
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

  void SparseMatrix::sum_prod(SparseMatrixPtr N, SparseMatrixPtr R, float n){
    if(_nrows != N->nrows() || _ncolumns != R->ncols() || N->ncols() != R->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (sum_prod)" << endl;
      exit(-1);
    }
   
    vector< map<int,float> > M1, M2;
    map<int,float>::const_iterator itn;
    map<int,float>::const_iterator itr;
    
    M1 = N->matrix();
    M2 = R->matrix();
    
    for(int i = 0; i < N->nrows(); i++){
      for(itn = M1[i].begin(); itn != M1[i].end(); itn++){
	for(itr = M2[itn->first].begin(); itr != M2[itn->first].end(); itr++){
	  sum(i, itr->first, itn->second * itr->second * n);
	}
      }
    }
    //clean();
  }

  void SparseMatrix::prod(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n){
    if(ncols() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod matrices)" << endl;
      exit(-1);
    }
   
    OUT = SparseMatrixPtr(new SparseMatrix(nrows(), N->ncols()));
    
    vector< map<int,float> > M1, M2;
    map<int,float>::const_iterator itn;
    map<int,float>::const_iterator itr;
    
    M1 = matrix();
    M2 = N->matrix();
    
    for(int i = 0; i < nrows(); i++){
      for(itn = M1[i].begin(); itn != M1[i].end(); itn++){
	for(itr = M2[itn->first].begin(); itr != M2[itn->first].end(); itr++){
	  OUT->sum(i, itr->first, itn->second * itr->second * n);
	}
      }
    }
  }

  void SparseMatrix::sum_trans_prod(SparseMatrixPtr N, SparseMatrixPtr R, float n){
    if(N->nrows() != R->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (sum_trans_prod)" << endl;
      exit(-1);
    }
    SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix(N->ncols(), N->nrows()));
    vector< map<int,float> > A = N->matrix(); 
    map<int,float>::iterator itm;
    for(int i = 0; i < N->nrows(); i++){
      for(itm = A[i].begin(); itm != A[i].end(); itm++){
	AUX->addNoCutoff(itm->first,i,itm->second);
      }
    }
    sum_prod(AUX,R,n);
  }

  void SparseMatrix::trans_prod(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n){
    if(nrows() != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (trans_prod)" << endl;
      exit(-1);
    }
    SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix(ncols(), nrows()));
    vector< map<int,float> > A = matrix(); 
    map<int,float>::iterator itm;
    for(int i = 0; i < nrows(); i++){
      for(itm = A[i].begin(); itm != A[i].end(); itm++){
	AUX->addNoCutoff(itm->first,i,itm->second);
      }
    }
    AUX->prod(N, OUT, n);
  }

  void SparseMatrix::sum_prod_trans(SparseMatrixPtr N, SparseMatrixPtr R, float n){
    if(N->ncols() != R->ncols()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (sum_prod_trans)" << endl;
      exit(-1);
    }
    SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix(R->ncols(), R->nrows()));
    vector< map<int,float> > A = R->matrix(); 
    map<int,float>::iterator it;
    for(int i = 0; i < R->nrows(); i++){
      for(it = A[i].begin(); it != A[i].end(); it++){
	AUX->addNoCutoff(it->first,i,it->second);
      }
    }
    sum_prod(N,AUX,n);
  }	

  void SparseMatrix::prod_trans(SparseMatrixPtr N, SparseMatrixPtr &OUT, float n){
    if(ncols() != N->ncols()){
      cout << "Cannot compute the product. Wrong number of rows or columns! (prod_trans)" << endl;
      exit(-1);
    }
    SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix(N->ncols(), N->nrows()));
    vector< map<int,float> > A = N->matrix(); 
    map<int,float>::iterator it;
    for(int i = 0; i < N->nrows(); i++){
      for(it = A[i].begin(); it != A[i].end(); it++){
	AUX->addNoCutoff(it->first,i,it->second);
      }
    }
    prod(AUX, OUT, n);
  }	

  void SparseMatrix::addGaps(SparseMatrixPtr ppGap1, SparseMatrixPtr ppGap2){
    vector<float> aux (_ncolumns, 0.0);
    map<int,float>::iterator it;
    resize(_nrows+1, _ncolumns+1);
    vector< map<int,float> > A = ppGap1->matrix();
    for(int i = 0; i < ppGap1->nrows(); i++){
      for(it = A[i].begin(); it != A[i].end(); it++){
	  aux[it->first] += it->second;
      }
    }
    for(int i = 0; i < (int)aux.size(); i++)
      add(_nrows-1,i,aux[i]);
    
    A = ppGap2->matrix();
    for(int i = 0; i < ppGap2->nrows(); i++){
      float sum = 0.0;
      for(it = A[i].begin(); it != A[i].end(); it++){
	  sum += it->second;
      }
      add(i,_ncolumns-1, sum);
    }
    nextr = 0;
    nextc = M[0].begin();
  }

  void SparseMatrix::sum(SparseMatrixPtr N){
    if(_ncolumns != N->ncols() || _nrows != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns!" << endl;
      exit(-1);
    }
    map<int,float>::iterator itm;
    map<int,float>::iterator itn;
    vector< map<int,float> > A = N->matrix();
    for(int i = 0; i < N->nrows(); i++){
      for(itn = A[i].begin(); itn != A[i].end(); itn++){
	sum(i,itn->first,itn->second);
      }
    }
  }

  void SparseMatrix::sum_times(SparseMatrixPtr N, float n){
    if(_ncolumns != N->ncols() || _nrows != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns!" << endl;
      exit(-1);
    }
    vector< map <int,float> > NM = N->matrix();
    map<int,float>::iterator itm;
    map<int,float>::iterator itn;
    for(int i = 0; i < N->nrows(); i++){
      for(itn = NM[i].begin(); itn != NM[i].end(); itn++){
	sum(i,itn->first,n*itn->second);
      }
    }
    clean();
  }

  void SparseMatrix::cutoff_sum_times(SparseMatrixPtr N, float n){
    if(_ncolumns != N->ncols() || _nrows != N->nrows()){
      cout << "Cannot compute the product. Wrong number of rows or columns!" << endl;
      exit(-1);
    }
    vector< map <int,float> > NM = N->matrix();
    vector< map <int,float> > MM;
    MM.resize(_nrows);
    map<int,float>::iterator itm;
    map<int,float>::iterator itn;
    for(int i = 0; i < N->nrows(); i++){
      for(itn = NM[i].begin(); itn != NM[i].end(); itn++){
	if(itn->second > cut_thresh)
	  MM[i][itn->first] = n*M[i][itn->first] + (1-n)*itn->second;
      }
    }
    matrix(MM);
  }

  void SparseMatrix::prod(float n, SparseMatrixPtr &OUT){
    OUT = SparseMatrixPtr(new SparseMatrix(nrows(), ncols()));
    if(n == 0)
      return;
    map<int,float>::iterator it;
    for(int i = 0; i < nrows(); i++){
      for(it = M[i].begin(); it != M[i].end(); it++){
	OUT->addNoCutoff(i,it->first,it->second*n);
      }
    }
  }

  void SparseMatrix::printFullMatrix(){
    for(int i = 0; i < _nrows; i++){
      for(int j = 0; j < _ncolumns; j++)
	cout << get(i,j) << " ";
      cout << endl;
    }
  }
  
  void SparseMatrix::printMatrix(){
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it;
      for(it = M[i].begin(); it != M[i].end(); it++)
	cout << i << "-" << it->first << "-" << it->second << " ";
    }
  }

  void SparseMatrix::printMatrix(ostringstream &temp){
    int sum = 0;
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it;
      for(it = M[i].begin(); it != M[i].end(); it++){
	sum++;
      }
    }
    temp << sum << endl;
    for(int i = 0; i < _nrows; i++){
      map<int,float>::iterator it;
      for(it = M[i].begin(); it != M[i].end(); it++){
	temp << i << " " << it->first << " " << it->second << endl;
      }
    }
  }

  void SparseMatrix::readFromFile(ifstream fin){
    while(fin.good()){
      int line, col;
      float prob;
      fin >> line >> col >> prob;
      add(line, col, prob);
    }
  }
}
    
