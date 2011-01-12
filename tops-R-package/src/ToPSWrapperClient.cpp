#include "Alphabet.hpp"
#include "ProbabilisticModel.hpp"
#include "ToPSWrapperClient.hpp"
#include "ProbabilisticModelParameterAdapter.hpp"
#include <map>
SEXP ToPSWrapperClient::convert (tops::ProbabilisticModelPtr model) 
{
  tops::ProbabilisticModelParameters pars = model->parameters();
  std::map<std::string, tops::ProbabilisticModelParameterValuePtr> all_par = pars.parameters();
  std::map<std::string, tops::ProbabilisticModelParameterValuePtr>::const_iterator it;

  SEXP list;
  PROTECT(list = allocVector(VECSXP, all_par.size()));
  SEXP names;
  PROTECT(names = allocVector(STRSXP, all_par.size()));
  ProbabilisticModelParameterValueFactory factory;
  int i = 0;
  for(it = all_par.begin(); it != all_par.end(); it++)
    {
      ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(it->second);
      SEXP converted = adapter->getSEXP();
      SET_STRING_ELT(names, i, mkChar((it->first).c_str()));
      SET_VECTOR_ELT(list, i++, converted);
    }
  setAttrib(list, R_NamesSymbol, names);
  UNPROTECT(2);
  return list;
}
