/*
 *       Alphabet.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef ALPHABET_H
#define ALPHABET_H

#include "crossplatform.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>




namespace tops {
  class DLLEXPORT Symbol;
  typedef boost::shared_ptr <Symbol> SymbolPtr;
  class DLLEXPORT ProbabilisticModelParameterValue;
  typedef boost::shared_ptr <ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  //! A class representing Alphabet.
  class DLLEXPORT Alphabet
  {
  public:
    //!  Creates a new symbol
    /*!
      \param name is the symbol name
    */
    SymbolPtr createSymbol(const std::string & name);
    SymbolPtr createSymbol(char * name);
    //! Returns the maximum length of the symbol
    unsigned int maxSymbolSize();

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
    unsigned int _max_symbol_size;
  };

  typedef boost::shared_ptr<Alphabet> AlphabetPtr;
}
#endif
