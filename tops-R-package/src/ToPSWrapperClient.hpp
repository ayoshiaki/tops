#ifndef TOPS_ADAPTER_CLIENT
#define TOPS_ADAPTER_CLIENT

#include <boost/shared_ptr.hpp>


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

class DLLEXPORT ToPSWrapperClient {
public:
  SEXP convert (tops::ProbabilisticModelPtr model) ;
};




#endif
