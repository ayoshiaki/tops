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
SEXP evaluate(SEXP exp) 
{
  exp = CDR(exp);
  int nargs = length(exp);

  if(nargs >= 1) {
    ProbabilisticModelParameterValueFactory factory;
    SEXP par = CAR(exp);
    SEXP seq = CAR(CDR(exp));



    ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(par);
    if(adapter == NULL)
      {
	std::cerr << "ERROR: evaluate(SEXP exp)" << std::endl;
	return R_NilValue;
      }
    
    
    adapter->getParameterValue()->setIsRoot(true);	
    tops::ProbabilisticModelCreatorClient client;
    ProbabilisticModelPtr m = client.create(adapter->getParameterValue()->getParameters());
    if(m != NULL) 
      {
	ProbabilisticModelParameterValueAdapterPtr seqAdapter = factory.adapter(seq);
	tops::SequenceFactory seqfactory(m->alphabet());
	tops::Sequence s = seqfactory.createSequence(seqAdapter->getParameterValue()->getStringVector());
	double result = m->evaluate(s, 0, s.size()-1);
	DoubleParameterValueAdapterPtr dblAdapter = DoubleParameterValueAdapterPtr(new DoubleParameterValueAdapter());
	dblAdapter->initialize(tops::DoubleParameterValuePtr(new DoubleParameterValue(result)));
	SEXP answer = dblAdapter->getSEXP();
	return answer;
      }
  }
  return R_NilValue;
}
