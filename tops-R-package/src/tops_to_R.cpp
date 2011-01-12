#include "ProbabilisticModel.hpp"
#include "ToPSWrapperClient.hpp"
#include "tops_to_R.hpp"

SEXP parametersToSEXP (tops::ProbabilisticModelPtr model) {
  SEXP r;
  ToPSWrapperClient * client = new ToPSWrapperClient() ;
  r = client->convert (model);
  delete client;
  return r;
}
