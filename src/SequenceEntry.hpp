/*
 *       SequenceEntry.hpp
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

#ifndef Sequence_ENTRY_HPP
#define Sequence_ENTRY_HPP

#include "crossplatform.hpp"

#include <istream>
#include <ostream>
#include <string>
#include <boost/shared_ptr.hpp>

#include "SequenceFactory.hpp"
#include "SequenceFormat.hpp"
#include "Sequence.hpp"

namespace tops {
  //! Represent a sequence entry
  class DLLEXPORT SequenceEntry {
  private:
    DLLEXPORT friend std::ostream & operator << (std::ostream & stream, SequenceEntry & out);
    DLLEXPORT friend std::istream & operator >> (std::istream & stream, SequenceEntry & in);
    std::string _name;
    std::string _description;
    Sequence _sequence;
    AlphabetPtr _alphabet;
    std::string _sep;
    SequenceFormatPtr _format;
      std::vector <int>_invalidPositions;
  public:
    SequenceEntry(){
      _format = (SequenceFormatManager::instance())->getFormat();
    };
    SequenceEntry ( AlphabetPtr alphabet) : _alphabet(alphabet), _sep(" ")
    {
      _format = (SequenceFormatManager::instance())->getFormat();
    }
    AlphabetPtr getAlphabet() {
      return _alphabet;
    }
      void addInvalidPosition(int i) {
          _invalidPositions.push_back(i);
      }
      void setInvalidPositions (std::vector <int> v) {
          _invalidPositions = v;
      }
      std::vector <int> & invalidPositions(){
          return _invalidPositions;
      }
    void setSeparator(std::string sep) {
      _sep = sep;
    }
    std::string getSeparator() {
      return _sep;
    }
    Sequence &  getSequence()  {
      return _sequence;
    }
    void setSequence(Sequence  s) {
      _sequence = s;
    }

    std::string  getName() {
      return _name;
    }

    void setName(std::string name) {
      _name = name;
    }

    void setDescription(std::string description) {
      _description = description;
    }

    std::string  getDescription() {
      return _description;
    }

    void setSequenceFormat(SequenceFormatPtr format) {
      _format = format;
    }
  };
  typedef boost::shared_ptr <SequenceEntry> SequenceEntryPtr;
  typedef std::vector <Sequence> SequenceList;
  typedef std::vector <SequenceEntryPtr> SequenceEntryList;


}
#endif
