#ifndef TOPS_TO_R
#define TOPS_TO_R

#include <map>
#include <boost/shared_ptr.hpp>
#include "ProbabilisticModelParameter.hpp"
#include "ProbabilisticModel.hpp"

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

SEXP parametersToSEXP (tops::ProbabilisticModelPtr model);

#endif
