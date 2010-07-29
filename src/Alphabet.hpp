#ifndef ALPHABET_H
#define ALPHABET_H

#include <boost/shared_ptr.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>




namespace tops {
  class Symbol;
  typedef boost::shared_ptr <Symbol> SymbolPtr;
  class ProbabilisticModelParameterValue;
  typedef boost::shared_ptr <ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  //! A class representing Alphabet.
  class Alphabet
  {
  public:
    //!  Creates a new symbol
    /*!
      \param name is the symbol name
    */
    SymbolPtr createSymbol(const std::string & name);
    SymbolPtr createSymbol(char * name);

    //! Returns the alphabet size
    unsigned int size ();

    //! Returns the symbol given an id
    SymbolPtr getSymbol(int k);

    //! Get the symbol given the symbol name
    SymbolPtr getSymbol(const std::string & s);

    //! Returns true if the symbol name s is in the Alphabet
    /*!
      \param s is the name of the symbol
      \return true if the name of the symbol is in the Alphabet
    */
    bool has (const std::string & s);

    //! Initialize the alphabet using a list of symbols.
    void initializeFromVector(const std::vector <std::string> & str);

    //! String representation of this alphabet
    std::string str() const;    
    ProbabilisticModelParameterValuePtr getParameterValue();
  private:
    std::vector<SymbolPtr, boost::pool_allocator<SymbolPtr> > _pool;
    std::map <std::string, SymbolPtr> _stringToSymbol;
  };
  
  typedef boost::shared_ptr<Alphabet> AlphabetPtr;
}
#endif
