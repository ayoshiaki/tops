/*
 *       Alphabet.cpp
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
