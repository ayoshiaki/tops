#ifndef PROBABILISTIC_MODEL_PARAMETER_WRAPPER_HPP
#define PROBABILISTIC_MODEL_PARAMETER_WRAPPER_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

#include "ProbabilisticModel.hpp"
#include "SequenceEntry.hpp"
#include "Sequence.hpp"

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
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "ProbabilisticModelParameter.hpp"


class DLLEXPORT ProbabilisticModelParameterValueAdapter{
public:
  ProbabilisticModelParameterValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v){}
  virtual void initialize(SEXP s) {};
  virtual SEXP getSEXP() {
    return R_NilValue;
  };
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue () {
    tops::ProbabilisticModelParameterValuePtr p;
    return p;
  };

};
typedef boost::shared_ptr<ProbabilisticModelParameterValueAdapter> ProbabilisticModelParameterValueAdapterPtr;

class DLLEXPORT ProbabilisticModelParameterListValueAdapter: public tops::ProbabilisticModelParameterListValue 
						 , public ProbabilisticModelParameterValueAdapter
						 , public boost::enable_shared_from_this<ProbabilisticModelParameterListValueAdapter> 
{
public:
  ProbabilisticModelParameterListValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<ProbabilisticModelParameterListValueAdapter> ProbabilisticModelParameterListValueAdapterPtr;


class DLLEXPORT DoubleParameterValueAdapter: public tops::DoubleParameterValue , public ProbabilisticModelParameterValueAdapter						 , public boost::enable_shared_from_this<DoubleParameterValueAdapter>  {
public:
  DoubleParameterValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<DoubleParameterValueAdapter> DoubleParameterValueAdapterPtr;

class DLLEXPORT IntParameterValueAdapter: public tops::IntParameterValue , public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<IntParameterValueAdapter> {
public:
  IntParameterValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<IntParameterValueAdapter> IntParameterValueAdapterPtr;

class DLLEXPORT StringParameterValueAdapter: public tops::StringParameterValue , public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<StringParameterValueAdapter> {
public:
  StringParameterValueAdapter() {};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<StringParameterValueAdapter> StringParameterValueAdapterPtr;

class DLLEXPORT IntVectorParameterValueAdapter: public tops::IntVectorParameterValue, public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<IntVectorParameterValueAdapter> {
public:
  IntVectorParameterValueAdapter() {};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<IntVectorParameterValueAdapter> IntVectorParameterValueAdapterPtr;

class DLLEXPORT DoubleMapParameterValueAdapter: public tops::DoubleMapParameterValue , public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<DoubleMapParameterValueAdapter> {
public:
  DoubleMapParameterValueAdapter() {};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<DoubleMapParameterValueAdapter> DoubleMapParameterValueAdapterPtr;

class DLLEXPORT DoubleVectorParameterValueAdapter: public tops::DoubleVectorParameterValue , public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<DoubleVectorParameterValueAdapter> {
public:
  DoubleVectorParameterValueAdapter() {};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<DoubleVectorParameterValueAdapter> DoubleVectorParameterValueAdapterPtr;

class DLLEXPORT StringMapParameterValueAdapter: public tops::StringMapParameterValue, public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<StringMapParameterValueAdapter> {
public:
  StringMapParameterValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();

};
typedef boost::shared_ptr<StringMapParameterValueAdapter> StringMapParameterValueAdapterPtr;

class DLLEXPORT StringVectorParameterValueAdapter: public tops::StringVectorParameterValue, public ProbabilisticModelParameterValueAdapter 						 , public boost::enable_shared_from_this<StringVectorParameterValueAdapter> {
public:
  StringVectorParameterValueAdapter(){};
  virtual void initialize(tops::ProbabilisticModelParameterValuePtr v);
  virtual void initialize(SEXP s);
  virtual SEXP getSEXP() ;
  virtual tops::ProbabilisticModelParameterValuePtr getParameterValue ();
};
typedef boost::shared_ptr<StringVectorParameterValueAdapter> StringVectorParameterValueAdapterPtr;

class DLLEXPORT ProbabilisticModelParameterValueFactory {
public:
  ProbabilisticModelParameterValueFactory();
  virtual ProbabilisticModelParameterValueAdapterPtr adapter(tops::ProbabilisticModelParameterValuePtr parameter) ;
  virtual ProbabilisticModelParameterValueAdapterPtr adapter(SEXP s) ;
private:
  std::map <std::string, ProbabilisticModelParameterValueAdapterPtr> adapters;
};


#endif

