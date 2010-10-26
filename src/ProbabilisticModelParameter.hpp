#ifndef CONFIGURATION_PARAMETER_VALUE_HPP
#define CONFIGURATION_PARAMETER_VALUE_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <iostream>

#include "SequenceEntry.hpp"
#include "Sequence.hpp"

namespace tops {

  typedef std::vector <double> DoubleVector;
  typedef std::vector <int> IntVector;
  typedef boost::numeric::ublas::matrix<double> Matrix;
  typedef std::vector <std::string> StringVector;
  typedef std::map <std::string, std::string> StringMap;
  class ProbabilisticModelParameterValue;
  typedef boost::shared_ptr <ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  //! This class registers a set of parameters
  class ProbabilisticModelParameters  {
    std::map <std::string, ProbabilisticModelParameterValuePtr> _parameters;
  public:
    ProbabilisticModelParameters(){}
    void add(const char * name, ProbabilisticModelParameterValuePtr value);
    
    //! Adds a new model parameter
    void add(std::string name, ProbabilisticModelParameterValuePtr value);
    
    //! set a model parameter
    void set(std::string name, ProbabilisticModelParameterValuePtr value);
    
    //! Get a mandatory parameter
    ProbabilisticModelParameterValuePtr getMandatoryParameterValue(const std::string & name) const;

    //! Get a optional parameter
    ProbabilisticModelParameterValuePtr getOptionalParameterValue(const std::string & name) const;
    
    std::map <std::string, ProbabilisticModelParameterValuePtr>  parameters() const;
    
  };
  

  
  //! Represents a parameter value
  class ProbabilisticModelParameterValue {
  private:
    StringVector s;
    DoubleVector d;
    IntVector i;
    std::vector <DoubleVector> m;    
    std::map<std::string,double> p;
    std::map<std::string,std::string> str_map;
    std::string _str;
    ProbabilisticModelParameters  _parameters;
  public:
    virtual void setIsRoot(bool root);
    ProbabilisticModelParameterValue(){}
    virtual    ~ProbabilisticModelParameterValue() {}
    virtual std::string parameter_type () const;
    virtual std::map<std::string,double> & getDoubleMap();
    virtual StringVector & getStringVector();
    virtual DoubleVector & getDoubleVector();
    virtual IntVector & getIntVector();
    virtual ProbabilisticModelParameters & getParameters();
    virtual std::string getString() const;
    virtual std::map<std::string,std::string> &  getStringMap();
    virtual int getInt() const;
    virtual double getDouble()  const;
    virtual std::string str() const;
  };

  class ProbabilisticModelParameterListValue : public ProbabilisticModelParameterValue { 
  public:
    ProbabilisticModelParameterListValue(){_root = false;}
    ProbabilisticModelParameterListValue ( ProbabilisticModelParameters p) : _parameters(p) {
      _root =false;
    };
    virtual void initialize(ProbabilisticModelParameters p);
    virtual ProbabilisticModelParameters & getParameters() ;
    virtual std::string parameter_type () const;
    virtual void setIsRoot(bool root);
    virtual std::string getString() const;
    virtual std::string str () const;
  private:
    ProbabilisticModelParameters  _parameters;
    bool _root;
  };
  typedef boost::shared_ptr<ProbabilisticModelParameterListValue> ProbabilisticModelParameterListValuePtr;
    
  //! Double parameter value
  class DoubleParameterValue: public ProbabilisticModelParameterValue {
  private:
    double _d;
  public:
    DoubleParameterValue(){}
    virtual void initialize(double d);
    DoubleParameterValue(double d) {
      _d = d;
    }
    virtual std::string parameter_type () const;
    virtual ~DoubleParameterValue(){}
    virtual double getDouble() const;
    virtual int getInt() const;
    virtual std::string str() const;
  };

  //! Integer parameter value
  class IntParameterValue: public ProbabilisticModelParameterValue {
  private:
    int _v;
  public:
    IntParameterValue() {};
    virtual std::string parameter_type () const;
    IntParameterValue(int v){ _v = v;}
    virtual void initialize(int v);
    virtual ~IntParameterValue() {}
    virtual int getInt() const;
    virtual double getDouble() const;
    virtual std::string str() const ;
  };

  //! String parameter value
  class StringParameterValue: public ProbabilisticModelParameterValue {
  private:
    std::string _v;
  public:
    StringParameterValue() {}
    StringParameterValue(std::string  v) {
      _v = v;
    }
    virtual std::string parameter_type () const;
    virtual void initialize(std::string v);
    virtual ~StringParameterValue(){}
    virtual std::string getString() const;
    virtual std::string str() const ;
  };

  //! integer vector parameter value
  class IntVectorParameterValue: public ProbabilisticModelParameterValue {
  private:
    IntVector _v;
  public:
    IntVectorParameterValue() {}
    virtual std::string parameter_type () const;
    virtual void initialize(IntVector v);
    IntVectorParameterValue(IntVector v) ;
    virtual ~IntVectorParameterValue(){}
    virtual IntVector & getIntVector()  ;
    virtual std::string str() const;
  };
  //! probability table
  class DoubleMapParameterValue: public ProbabilisticModelParameterValue {
  private:
    std::map <std::string,double> _v;
  public:
    DoubleMapParameterValue() {
    }
    DoubleMapParameterValue(const std::map<std::string, double> & values) {
      _v = values;
    }
    virtual void initialize(const std::map<std::string, double > & values);
    virtual std::string parameter_type () const;
    DoubleMapParameterValue(std::vector<std::string> keys, std::vector<double> value) {
      for(int i = 0; i < (int)value.size(); i++)
	_v[keys[i]] = value[i];
    }
    virtual ~DoubleMapParameterValue(){}
    virtual std::map<std::string, double> & getDoubleMap() ;
    virtual std::string str() const;

  };
  //! double vector parameter value
  class DoubleVectorParameterValue: public ProbabilisticModelParameterValue {
  private:
    DoubleVector _v;
  public:
    DoubleVectorParameterValue() {
    }
    DoubleVectorParameterValue(DoubleVector v) {
      _v = v;
    }
    virtual void initialize(DoubleVector v);
    ~DoubleVectorParameterValue(){}
    virtual DoubleVector & getDoubleVector() ;
    virtual std::string parameter_type () const ;
    virtual std::string str() const ;

  };

  //! string vector parameter value
  class StringMapParameterValue: public ProbabilisticModelParameterValue {
  private:
    std::map<std::string,std::string> _str_map;
  public:

    StringMapParameterValue(std::vector<std::string> keys,std::vector<std::string> values) 
    {   
      for(int i = 0; (i  < (int)keys.size()) && (i < (int)values.size()); i++)
	_str_map[keys[i]] = values[i];
    }
    StringMapParameterValue(std::map<std::string,std::string> m) 
    {
      _str_map = m;
    }
    virtual void initialize(std::map<std::string,std::string> m) ;
    StringMapParameterValue() {
    }
    ~StringMapParameterValue(){}
    virtual StringMap & getStringMap();
    virtual std::string parameter_type () const;
    virtual std::string str() const;
  };

  //! string vector parameter value
  class StringVectorParameterValue: public ProbabilisticModelParameterValue {
  private:
    StringVector _v;
  public:
    StringVectorParameterValue(){}
    StringVectorParameterValue(StringVector v) {
      _v = v;
    }
    ~StringVectorParameterValue(){}
    virtual StringVector &  getStringVector() ;
    virtual void initialize(StringVector v);
    virtual std::string parameter_type () const ;
    virtual std::string str() const;
  };
  

  typedef boost::shared_ptr <IntParameterValue> IntParameterValuePtr;
  typedef boost::shared_ptr <DoubleParameterValue> DoubleParameterValuePtr;
  typedef boost::shared_ptr <StringParameterValue> StringParameterValuePtr;
  typedef boost::shared_ptr <StringMapParameterValue> StringMapParameterValuePtr;
  
  typedef boost::shared_ptr <IntVectorParameterValue> IntVectorParameterValuePtr;
  typedef boost::shared_ptr <DoubleVectorParameterValue> DoubleVectorParameterValuePtr;
  typedef boost::shared_ptr <StringVectorParameterValue> StringVectorParameterValuePtr;
  typedef boost::shared_ptr <DoubleMapParameterValue> DoubleMapParameterValuePtr;
}



#endif

