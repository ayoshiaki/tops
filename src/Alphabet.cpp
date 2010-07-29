#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "ProbabilisticModelParameter.hpp"
#include <boost/algorithm/string.hpp>

namespace tops {
  bool Alphabet::has (const std::string & s)
  {
    std::map<std::string, SymbolPtr>::iterator it;
    it = _stringToSymbol.find(s);
    return it != _stringToSymbol.end();
  }
  
  SymbolPtr Alphabet::getSymbol(const std::string & s)
    {
      std::map<std::string, SymbolPtr>::iterator it;
      it = _stringToSymbol.find(s);
      if(it == _stringToSymbol.end())
	return SymbolPtr(new Symbol());
      else return it->second;
    }

  SymbolPtr Alphabet::getSymbol(int k)
  {
    if((k >= 0) && (k < (int)_pool.size()))
      return _pool[k];
    else
      return SymbolPtr(new Symbol());
  }
  
  unsigned int Alphabet::size ()
  {
    return _pool.size();
  }
  
  std::string Alphabet::str() const {
    std::stringstream out;
      std::vector<SymbolPtr, boost::pool_allocator<SymbolPtr> >::const_iterator it;
      if(_pool.size() > 0)
	{
	  out << "alphabet = (";
      
	  out << "\"" << _pool[0]->name() << "\"";
	  for(int k = 1; k < (int)_pool.size(); k++)
	    {
	      out << ", \"" << (_pool[k])->name() << "\"";
	    }
	  out << ")";
	}

      out << std::endl;
      return out.str();
    }
    

  SymbolPtr Alphabet::createSymbol(char * name){
      std::string s(name);
      return createSymbol(s);
    }

  ProbabilisticModelParameterValuePtr Alphabet::getParameterValue(){
    ProbabilisticModelParameterValuePtr v;
    if(_pool.size() > 0)
      {
	StringVector strlist;
	for(int k = 0; k < (int)_pool.size(); k++)
	  {
	    strlist.push_back(_pool[k]->name());
	  }
	v = ProbabilisticModelParameterValuePtr (new StringVectorParameterValue(strlist));
      }
    return v;
  }
  SymbolPtr Alphabet::createSymbol (const std::string & name)
  {
    int i;
    for(i = 0; i < (int)_pool.size(); i++)
      if(_pool[i]->name() == name)
	return getSymbol(i);
    SymbolPtr s = SymbolPtr( new Symbol(i, name, this));
    _stringToSymbol[name] = s;
    _pool.push_back(s);
    return getSymbol(i);
  }


  void Alphabet::initializeFromVector(const std::vector<std::string> & str)
  {
    for(int i = 0; i < (int)str.size(); i++)
      createSymbol(str[i]);
  }
}
