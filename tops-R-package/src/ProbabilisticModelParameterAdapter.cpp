#include "ProbabilisticModelParameterAdapter.hpp"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>
#include "SequenceEntry.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "util.hpp"

SEXP getListElement(SEXP list, const char * str) 
{
  SEXP elmt = R_NilValue, names = getAttrib(list, R_NamesSymbol);
  int i ;
  for( i = 0; i < length(list); i++)
    if(strcmp(CHAR(STRING_ELT(names, i)), str) == 0)
      {
	elmt = VECTOR_ELT(list, i);
	break;
      }
  return elmt;
}


void DoubleParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::DoubleParameterValue::initialize(v->getDouble());
}

void DoubleParameterValueAdapter::initialize(SEXP s) 
{
  PROTECT(s);
  tops::DoubleParameterValue::initialize(REAL(s)[0]);
  UNPROTECT(1);
}

SEXP DoubleParameterValueAdapter::getSEXP()  {
  SEXP answer;
  PROTECT(answer = allocVector(REALSXP, 1));
  REAL(answer)[0] = getDouble();
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr DoubleParameterValueAdapter::getParameterValue () {
  DoubleParameterValueAdapterPtr p = shared_from_this();
  return p;
}


void IntParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::IntParameterValue::initialize(v->getInt());
}

void IntParameterValueAdapter::initialize(SEXP s) 
{
  PROTECT(s);
  tops::IntParameterValue::initialize(INTEGER(s)[0]);
  UNPROTECT(1);

}


SEXP IntParameterValueAdapter::getSEXP() {
  SEXP answer;
  PROTECT(answer = allocVector(INTSXP, 1));
  INTEGER(answer)[0] = getInt();
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr IntParameterValueAdapter::getParameterValue () {
  IntParameterValueAdapterPtr p = shared_from_this();
  return p;
}

void StringParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::StringParameterValue::initialize(v->getString());
}


void StringParameterValueAdapter::initialize(SEXP s) 
{
  PROTECT(s);
  std::string str (CHAR(STRING_ELT(s,0)));
  tops::StringParameterValue::initialize(str);
  UNPROTECT(1);
}


SEXP StringParameterValueAdapter::getSEXP() {
  SEXP answer;
  PROTECT(answer = allocVector(STRSXP, 1));
  SET_STRING_ELT(answer, 0, mkChar(getString().c_str()));
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr StringParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}

void IntVectorParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::IntVectorParameterValue::initialize(v->getIntVector());
}

void IntVectorParameterValueAdapter::initialize(SEXP s) 
{
  std::vector <int> v ;
  PROTECT(s);
  int *vec = INTEGER(s);
  for(int i = 0; i < length(s); i++)
    v.push_back(vec[i]);
  tops::IntVectorParameterValue::initialize(v);
  UNPROTECT(1);
}

SEXP IntVectorParameterValueAdapter::getSEXP() {
  std::vector<int> v= getIntVector();
  SEXP answer;
  PROTECT(answer = allocVector(INTSXP, v.size()));
  int * vec = INTEGER(answer);
  for(int i = 0; i < v.size(); i++)
    vec[i] = v[i];
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr IntVectorParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}


void DoubleMapParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::DoubleMapParameterValue::initialize(v->getDoubleMap());
}


void DoubleMapParameterValueAdapter::initialize(SEXP s) 
{
  SEXP dims = getAttrib(s, R_DimNamesSymbol);
  SEXP rownames = GetRowNames(dims);
  SEXP colnames = GetColNames(dims);
  std::map<std::string, double> d;
  double * data = REAL(s);
  bool withconditional = false;
  for(int i = 0; i < length(rownames); i++)
    {
      std::string rname(CHAR(STRING_ELT(rownames, i)));
      if(rname.size() > 0)
	{
	  withconditional = true;
	  break;
	}
    }
  for(int i = 0; i < length(rownames); i++)
    {
      const char * rname =  CHAR(STRING_ELT(rownames, i));
      for(int j = 0; j < length(colnames); j++)
	{
	  const char *cname = CHAR(STRING_ELT(colnames, j));
	  std::stringstream out;
	  if(withconditional)
	    out  << cname << "|" << rname ;
	  else 
	    out << cname ;
	  double r = data[i + j *length(rownames)];
	  if(!tops::close(r, 0.0, 1e-1))
	    d[out.str()] = r;
	}
    }
  tops::DoubleMapParameterValue::initialize(d);
}

SEXP DoubleMapParameterValueAdapter::getSEXP() {
  SEXP doublemap;
  std::map<std::string, double> v = getDoubleMap();
  std::map<std::string,tops::DoubleVector> doublemap2;
  std::map <std::string,double>::const_iterator it;
  std::map<std::string,tops::DoubleVector>::const_iterator it2;
  tops::AlphabetPtr alphabet = tops::AlphabetPtr(new tops::Alphabet());
  for(it = v.begin();
      it !=v.end();
      it++)
    {
      std::vector <std::string> splited;
      boost::regex separator("\\|");
      tops::split_regex(it->first, splited, separator);
      if(splited.size() == 1) {
	splited.push_back("");
      }
      std::string context (splited[1]);
      std::string symbol ( splited[0]);
      alphabet->createSymbol(symbol);
    }

  for(it = v.begin();
      it != v.end();
      it++)
    {

      std::vector <std::string> splited;
      boost::regex separator("\\|");
      tops::split_regex(it->first, splited, separator);

      if(splited.size() == 1) {
	splited.push_back("");
      }
      std::string context (splited[1]);
      std::string symbol ( splited[0]);

      if(doublemap2.find(context) == doublemap2.end())
	{
	  int id = alphabet->getSymbol(symbol)->id();
	  tops::DoubleVector probs;
	  probs.resize(alphabet->size());
	  doublemap2[context]=probs;
	  if(id < (int)doublemap2[context].size())
	    (doublemap2[context])[id] = it->second;
	}
      else
	{
	  int id = alphabet->getSymbol(symbol)->id();
	  if(id < (int)doublemap2[context].size())
	    (doublemap2[context])[id] = it->second;
	}

    }
  

  PROTECT(doublemap = allocMatrix(REALSXP,  alphabet->size(), doublemap2.size()));
  double * r;
  r = REAL(doublemap);  
  int j = 0;
  SEXP colnames, rownames;
  PROTECT(colnames = allocVector(STRSXP, alphabet->size())) ;  
  for(int i = 0; i < alphabet->size(); i++)
    {
      SET_STRING_ELT(colnames, i, mkChar((alphabet->getSymbol(i)->name()).c_str()));
    }
  UNPROTECT(1);


  PROTECT(rownames = allocVector(STRSXP, doublemap2.size())) ;  
  j = 0;
  for(it2 = doublemap2.begin(); it2 != doublemap2.end(); it2++) {
    std::string context = it2->first;
    SET_STRING_ELT(rownames,j++,mkChar(context.c_str()));
  }
  UNPROTECT(1);
  
  for(int i = 0; i < alphabet->size(); i++){
    j = 0;
    for(it2 = doublemap2.begin(); it2 != doublemap2.end();  it2++) 
      {
	std::vector<double> p = it2->second;
	r[j + doublemap2.size()*i] = p[i];
	j++;
      }
  }

  SEXP dim;
  PROTECT(dim = allocVector(INTSXP, 2));
  INTEGER(dim)[0] =  doublemap2.size(); INTEGER(dim)[1] = alphabet->size();
  setAttrib(doublemap, R_DimSymbol, dim);
  UNPROTECT(1);


  SEXP dimnames;
  PROTECT(dimnames = allocVector(VECSXP, 2));
  SET_VECTOR_ELT(dimnames, 0, rownames);
  SET_VECTOR_ELT(dimnames, 1, colnames);
  setAttrib(doublemap, R_DimNamesSymbol, dimnames);  
  UNPROTECT(2);

  return doublemap;
}

tops::ProbabilisticModelParameterValuePtr DoubleMapParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}

void DoubleVectorParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::DoubleVectorParameterValue::initialize(v->getDoubleVector());
}

void DoubleVectorParameterValueAdapter::initialize(SEXP s)
{
  std::vector <double> v ;
  PROTECT(s);
  double *vec = REAL(s);
  for(int i = 0; i < length(s); i++)
    v.push_back(vec[i]);
  tops::DoubleVectorParameterValue::initialize(v);
  UNPROTECT(1);
}

SEXP DoubleVectorParameterValueAdapter::getSEXP() {
  std::vector<double> v= getDoubleVector();
  SEXP answer;
  PROTECT(answer = allocVector(REALSXP, v.size()));
  double * vec = REAL(answer);
  for(int i = 0; i < v.size(); i++)
    vec[i] = v[i];
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr DoubleVectorParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}

void StringMapParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::StringMapParameterValue::initialize(v->getStringMap());
}

void StringMapParameterValueAdapter::initialize(SEXP s)
{
  std::cerr << "NOT IMPLEMENTED: StringMapParameterValueAdapter::initialize(SEXP s)" << std::endl;
}

tops::ProbabilisticModelParameterValuePtr StringMapParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}

SEXP StringMapParameterValueAdapter::getSEXP() {
  SEXP answer, names;
  std::map<std::string, std::string> str = getStringMap();
  std::map<std::string, std::string>::const_iterator it;
  PROTECT(answer = allocVector(STRSXP, str.size()));
  PROTECT(names= allocVector(STRSXP, str.size()));
  int i = 0;
  for(it = str.begin(); it != str.end(); it++)
    {
      SET_STRING_ELT(answer, i, mkChar((it->second).c_str()));
      SET_STRING_ELT(names, i++, mkChar((it->first).c_str()));
    }
  setAttrib(answer, R_NamesSymbol, names);
  UNPROTECT(2);
  return answer;

}

void StringVectorParameterValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::StringVectorParameterValue::initialize(v->getStringVector());
}

void StringVectorParameterValueAdapter::initialize(SEXP s)
{
  std::vector <std::string> v ;
  PROTECT(s);
  for(int i = 0; i < length(s); i++)
    v.push_back(CHAR(STRING_ELT(s, i)));
  tops::StringVectorParameterValue::initialize(v);
  UNPROTECT(1);
}


SEXP StringVectorParameterValueAdapter::getSEXP() {
  SEXP answer;
  std::vector<std::string> str = getStringVector();
  PROTECT(answer = allocVector(STRSXP, str.size()));
  for(  int i = 0; i < str.size(); i++)
    {
      SET_STRING_ELT(answer, i, mkChar((str[i]).c_str()));
    }
  UNPROTECT(1);
  return answer;
}

tops::ProbabilisticModelParameterValuePtr StringVectorParameterValueAdapter::getParameterValue () {
  return shared_from_this();
}

void ProbabilisticModelParameterListValueAdapter::initialize(tops::ProbabilisticModelParameterValuePtr v)
{
  tops::ProbabilisticModelParameterListValue::initialize(v->getParameters());
}

void ProbabilisticModelParameterListValueAdapter::initialize(SEXP s)
{
  tops::ProbabilisticModelParameters pars;
  ProbabilisticModelParameterValueFactory factory;
  SEXP names = getAttrib(s, R_NamesSymbol);
  for(int i = 0; i < length(s) ; i ++ )
    {
      ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(VECTOR_ELT(s, i));
      std::string parname(CHAR(STRING_ELT(names, i)));
      pars.add(parname, adapter->getParameterValue()) ;
    }
  tops::ProbabilisticModelParameterListValue::initialize(pars);
}

SEXP ProbabilisticModelParameterListValueAdapter::getSEXP() {
  
  std::map<std::string, tops::ProbabilisticModelParameterValuePtr> pars = getParameters().parameters();
  std::map<std::string, tops::ProbabilisticModelParameterValuePtr>::const_iterator iter;
  ProbabilisticModelParameterValueFactory factory;
  SEXP list,names;
  PROTECT(list = allocVector(VECSXP, pars.size()));
  PROTECT(names = allocVector(STRSXP, pars.size()));
  int i  = 0;
  for(iter = pars.begin(); iter != pars.end(); iter++)
    {
      if(iter->second != NULL) 
	{
	  ProbabilisticModelParameterValueAdapterPtr adapter = factory.adapter(iter->second);
	  if(adapter== NULL) 
	    {
	      std::cerr << "ERROR: Adapter not found " << iter->second->parameter_type() << std::endl;
	      exit(-1);
	    }
	  SET_STRING_ELT(names, i, mkChar((iter->first).c_str()));
	  SET_VECTOR_ELT(list, i++, adapter->getSEXP());
	}
    }
  setAttrib(list, R_NamesSymbol, names);
  UNPROTECT(2);
  return list;
}

tops::ProbabilisticModelParameterValuePtr ProbabilisticModelParameterListValueAdapter::getParameterValue () {
  return shared_from_this();
}


ProbabilisticModelParameterValueAdapterPtr ProbabilisticModelParameterValueFactory::adapter(tops::ProbabilisticModelParameterValuePtr parameter) 
{
  std::map <std::string, ProbabilisticModelParameterValueAdapterPtr> ::const_iterator it;
  std::string name = (parameter->parameter_type());
  it = adapters.find(name);
  if(it == adapters.end())
    {
      std::cerr <<  "ERROR: Could not find adapter for " << name <<  std::endl;
      exit(-1);
    }
  it->second->initialize(parameter);
  return it->second;
};

ProbabilisticModelParameterValueAdapterPtr ProbabilisticModelParameterValueFactory::adapter(SEXP s) 
{
  ProbabilisticModelParameterValueAdapterPtr a;
  if(isString(s))
    {
      if(length(s) == 1) 
	{
	  a = StringParameterValueAdapterPtr(new StringParameterValueAdapter());
	}
      else 
	{ 
	  a = StringVectorParameterValueAdapterPtr(new StringVectorParameterValueAdapter());

	}
    }
  else if(isReal(s)) {
    if(length(s) == 1) {
      a = DoubleParameterValueAdapterPtr(new DoubleParameterValueAdapter());

    }
    else if(isMatrix(s)) {
      a = DoubleMapParameterValueAdapterPtr(new DoubleMapParameterValueAdapter());

    }
    else {
      a = DoubleVectorParameterValueAdapterPtr(new DoubleVectorParameterValueAdapter());

    }

  }
  else if (isInteger(s))
    if(length(s) > 1) {
      a = IntVectorParameterValueAdapterPtr(new IntVectorParameterValueAdapter());

    }
    else {
      a = IntParameterValueAdapterPtr(new IntParameterValueAdapter());

    }
  else if (isNewList(s)) {
    a = ProbabilisticModelParameterListValueAdapterPtr(new ProbabilisticModelParameterListValueAdapter());

  }
  else {
    std::cerr << "Could not find the type of the S-expression " << std::endl;
    exit(-1);
  }

  a->initialize(s);
  
  return a;
};

ProbabilisticModelParameterValueFactory::ProbabilisticModelParameterValueFactory() 
{
  adapters["IntParameterValue"] = IntParameterValueAdapterPtr (new IntParameterValueAdapter());
  adapters["DoubleParameterValue"] =  DoubleParameterValueAdapterPtr (new DoubleParameterValueAdapter());
  adapters["StringParameterValue"] = StringParameterValueAdapterPtr(new  StringParameterValueAdapter());
  adapters["StringMapParameterValue"] = StringMapParameterValueAdapterPtr(new  StringMapParameterValueAdapter());
  adapters["IntVectorParameterValue"] = IntVectorParameterValueAdapterPtr(new  IntVectorParameterValueAdapter());
  adapters["DoubleVectorParameterValue"] = DoubleVectorParameterValueAdapterPtr(new  DoubleVectorParameterValueAdapter());
  adapters["StringVectorParameterValue"] = StringVectorParameterValueAdapterPtr(new  StringVectorParameterValueAdapter());
  adapters["DoubleMapParameterValue"] = DoubleMapParameterValueAdapterPtr(new  DoubleMapParameterValueAdapter());
  adapters["ProbabilisticModelParameterListValue"] = ProbabilisticModelParameterListValueAdapterPtr(new  ProbabilisticModelParameterListValueAdapter());
}


