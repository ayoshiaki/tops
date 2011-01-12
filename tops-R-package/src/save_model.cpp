#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"


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
SEXP save_model(SEXP exp) 
{
  ProbabilisticModelParameterValueFactory factory;
  int nargs = length(exp);

  if(nargs >= 3) 
    {
      const char * f = CHAR(STRING_ELT(CAR(CDR(exp)), 0));

      SEXP par = CAR(CDDR(exp));

      ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(par);
      if(adapter == NULL)
	{
	  std::cerr << "ERROR: save_model(SEXP exp)" << std::endl;
	  return R_NilValue;
	}

      
      adapter->getParameterValue()->setIsRoot(true);	
      tops::ProbabilisticModelCreatorClient client;
      ProbabilisticModelPtr m = client.create(adapter->getParameterValue()->getParameters());
      if(m != NULL) 
	{
	  std::ofstream output(f);
	  output << m->str() << std::endl;
	  output.close();
	} 
      else 
	{
	  std::ofstream output(f);
	  output << adapter->getParameterValue()->str();
	  output.close();
	}

      SEXP answer;
      PROTECT(answer = allocVector(INTSXP, 1));
      INTEGER(answer)[0] = 1;
      UNPROTECT(1);
      return answer;
    }
  return R_NilValue;
}
