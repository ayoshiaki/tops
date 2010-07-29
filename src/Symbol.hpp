#ifndef SYMBOL_H
#define SYMBOL_H

#include <boost/shared_ptr.hpp>
#include <string>


namespace tops {

  class Alphabet;
  typedef boost::shared_ptr<Alphabet> AlphabetPtr;

  //! This class represents a symbol
  class Symbol
  {
  public:
    ~Symbol(){}
    Symbol(){ _name = "_"; _id=-1;}
    Symbol (int id, std::string name, Alphabet * alphabet)  
    { _id = id; _name = name; _alphabet = alphabet;}  

    //! Returns the symbol identifier 
    int id ();

    //! Returns the symbol name
    std::string name();

  
    //! Returns the alphabet of the symbol
    Alphabet *  alphabet()
    {
      return _alphabet;
    }
  private:
    int _id; // symbol id
    std::string _name; // symbol name
    Alphabet * _alphabet; // alphabet
  };

  typedef boost::shared_ptr<Symbol> SymbolPtr;


}
#endif
