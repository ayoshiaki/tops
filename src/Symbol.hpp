/*
 *       Symbol.hpp
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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "crossplatform.hpp"

#include <boost/shared_ptr.hpp>
#include <string>


namespace tops {

  class DLLEXPORT Alphabet;
  typedef boost::shared_ptr<Alphabet> AlphabetPtr;

  //! This class represents a symbol
  class DLLEXPORT Symbol
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
