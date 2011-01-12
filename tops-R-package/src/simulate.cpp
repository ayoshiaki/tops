#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"

extern "C" {
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <R_ext/Rdynload.h>
#include <Rembedded.h>
#include <Rinterface.h>
#include <R_ext/RS.h>
#include <R_ext/Error.h>
#include <R_ext/PrtUtil.h>
}
#include "tops_to_R.hpp"
#include "ProbabilisticModelParameterAdapter.hpp"
#include <fstream>
#include <iostream>

using namespace tops;
SEXP simulate(SEXP exp) 
{
  exp = CDR(exp);
  int nargs = length(exp);

  if(nargs >= 2) {
    ProbabilisticModelParameterValueFactory factory;
    SEXP par = CAR(exp);
    int length = (int)REAL(CAR(CDR(exp)))[0];

    ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(par);
    if(adapter == NULL)
      {
	std::cerr << "ERROR: save_model(SEXP exp)" << std::endl;
	return R_NilValue;
      }
    
    
    adapter->getParameterValue()->setIsRoot(true);	
    tops::ProbabilisticModelCreatorClient client;
    ProbabilisticModelPtr m = client.create(adapter->getParameterValue()->getParameters());
    tops::Sequence s;
    tops::Sequence p;
    if(m == NULL) 
      {
	return R_NilValue;
      }
    m->choose(s,p, length);
    if (m->alphabet()->size() > 0) {

      SEXP result, sequence, path, names;

      
      if(p.size() > 0){
	PROTECT(result = allocVector(VECSXP, 2));
	PROTECT(names = allocVector(STRSXP, 2));
      }
      else {
	PROTECT(names = allocVector(STRSXP, 1));
	PROTECT(result = allocVector(VECSXP, 1));
      }
      
      PROTECT(sequence = allocVector(STRSXP, s.size()));
      for(int i = 0; i < s.size(); i++)
	{
	  SET_STRING_ELT(sequence, i, mkChar((m->alphabet()->getSymbol(s[i])->name()). c_str()));
	}
      SET_STRING_ELT(names, 0, mkChar("sequence"));
      SET_VECTOR_ELT(result, 0, sequence);
      UNPROTECT(1);
      
      if(p.size() > 0)
	{
	  PROTECT(path = allocVector(STRSXP, p.size()));
	  for(int i = 0; i < p.size(); i++)
	    {
	      SET_STRING_ELT(path, i, mkChar((m->getStateNames()->getSymbol(p[i])->name()). c_str()));
	    }
	  SET_VECTOR_ELT(result, 1, path);
	  SET_STRING_ELT(names, 1, mkChar("path"));
	  UNPROTECT(1);
	}
      setAttrib(result, R_NamesSymbol, names);
      UNPROTECT(2);
      return  result;
    } else {
      SEXP result, sequence, names;
      
      PROTECT(names = allocVector(STRSXP, 1));
      PROTECT(result = allocVector(VECSXP, 1));

      
      PROTECT(sequence = allocVector(REALSXP, s.size()));
      for(int i = 0; i < s.size(); i++)
	{
	  REAL(sequence)[i] = (double)s[i];
	}
      SET_STRING_ELT(names, 0, mkChar("sequence"));
      SET_VECTOR_ELT(result, 0, sequence);
      UNPROTECT(1);

      setAttrib(result, R_NamesSymbol, names);
      UNPROTECT(2);
      return  result;

    }

  }
  return R_NilValue;
}
