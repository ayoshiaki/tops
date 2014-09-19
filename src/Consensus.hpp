/*
 *       ConfigurationReader.hpp
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

#ifndef CONSENSUS_HPP
#define CONSENSUS_HPP
#include <boost/shared_ptr.hpp>

#include "crossplatform.hpp"
#include "util.hpp"
#include "Sequence.hpp"

#include <iostream>
#include <string>
#include <vector>


using namespace std;


namespace tops {

  class Consensus {
  public:
    Consensus(Sequence symbols):_symbols(symbols) {}
    bool is(int symbol) const;
    std::string str() const;
    std::string sym_str(AlphabetPtr alphabet) const;
    Sequence symbols() {
      return _symbols;
    }
  private:
    Sequence _symbols;
  };

  typedef std::vector<Consensus> ConsensusSequence;

}

#endif
