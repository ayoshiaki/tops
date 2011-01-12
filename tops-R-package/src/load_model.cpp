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
using namespace tops;
SEXP load_model(SEXP exp) 
{
  int nargs = length(exp);
  const char * f;
  if (nargs >= 2) {
    exp = CDR(exp);
    f = CHAR(STRING_ELT(CAR(exp), 0));
    std::string filename(f);
      
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr model = creator.create(filename);
    if(model != NULL) 
      {
	SEXP r = parametersToSEXP(model);
	return r;
      }
  }
  return R_NilValue;
}
