#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"
#include "ProbabilisticModelParameterAdapter.hpp"
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
SEXP viterbi(SEXP exp) 
{
  exp = CDR(exp);
  int nargs = length(exp);

  if(nargs >= 2) {
    ProbabilisticModelParameterValueFactory factory;
    SEXP par = CAR(exp);
    SEXP seq = CAR(CDR(exp));

    ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(par);
    if(adapter == NULL)
      {
	std::cerr << "ERROR: viterbi(SEXP exp)" << std::endl;
	return R_NilValue;
      }
    
    adapter->getParameterValue()->setIsRoot(true);	
    tops::ProbabilisticModelCreatorClient client;
    ProbabilisticModelPtr m = client.create(adapter->getParameterValue()->getParameters());
    std::vector<std::string> seqstr;
    tops::SequenceFactory seqfactory(m->alphabet());
    ProbabilisticModelParameterValueAdapterPtr seqAdapter = factory.adapter(seq);
    tops::Sequence s = seqfactory.createSequence(seqAdapter->getParameterValue()->getStringVector());
    tops::Sequence p;
    if(m == NULL) 
      {
	return R_NilValue;
      }
    tops::Matrix v;
    double score = m->decodable()->viterbi(s, p, v);

    if (m->alphabet()->size() > 0) {

      SEXP result, sequence, path, names;

      DoubleParameterValueAdapterPtr dblAdapter = DoubleParameterValueAdapterPtr(new DoubleParameterValueAdapter());
      dblAdapter->initialize(tops::DoubleParameterValuePtr(new DoubleParameterValue(score)));
      SEXP score2 = dblAdapter->getSEXP();
      
      PROTECT(result = allocVector(VECSXP, 3));
      PROTECT(names = allocVector(STRSXP, 3));
      PROTECT(sequence = allocVector(STRSXP, s.size()));
      for(int i = 0; i < s.size(); i++)
	{
	  SET_STRING_ELT(sequence, i, mkChar((m->alphabet()->getSymbol(s[i])->name()). c_str()));
	}
      SET_STRING_ELT(names, 0, mkChar("sequence"));
      SET_VECTOR_ELT(result, 0, sequence);

      UNPROTECT(1);
      
      PROTECT(path = allocVector(STRSXP, p.size()));
      for(int i = 0; i < p.size(); i++)
	{
	  SET_STRING_ELT(path, i, mkChar((m->getStateNames()->getSymbol(p[i])->name()). c_str()));
	}
      SET_VECTOR_ELT(result, 1, path);
      SET_VECTOR_ELT(result, 2, score2);
      SET_STRING_ELT(names, 1, mkChar("path"));
      SET_STRING_ELT(names, 2, mkChar("score"));
      UNPROTECT(1);
      setAttrib(result, R_NamesSymbol, names);
      UNPROTECT(2);
      return  result;
    }
  }
  return R_NilValue;
}
