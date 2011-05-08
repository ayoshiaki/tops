/*
 *       SequenceFactory.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#ifndef SEQUENCE_FACTORY_H
#define SEQUENCE_FACTORY_H

#include "crossplatform.hpp"

#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"

namespace tops{
  class DLLEXPORT Alphabet;
  typedef boost::shared_ptr<Alphabet> AlphabetPtr;
  struct my_tolower
  {
    char operator()(char c) const
    {
      return tolower(c);
    }
  };

  //! Provides factory methods for creating objects of type Sequence
  class DLLEXPORT SequenceFactory
  {
  private:
    AlphabetPtr _alphabet;
  public:
    SequenceFactory(){}
    SequenceFactory(AlphabetPtr alpha)
    {
      _alphabet =  alpha;
    };
    virtual ~SequenceFactory(){}
    virtual Sequence createSequence(const std::string & seq);
    virtual Sequence createSequenceRemovedSpaces(const std::string & seq);
    virtual Sequence createSequence(const std::vector<std::string> & seq);

      virtual Sequence createSequence(const std::string & seq, std::vector <int> &invalidPositions);
      virtual Sequence createSequenceRemovedSpaces(const std::string & seq, std::vector <int>& invalidPositions);
      virtual Sequence createSequence(const std::vector<std::string> & seq, std::vector <int> & invalidPositions);


  };
}
#endif
