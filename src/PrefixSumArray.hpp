/*
 *       PrefixSumArray.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
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

#ifndef PREFIX_SUM_ARRAY_HPP
#define PREFIX_SUM_ARRAY_HPP

#include "crossplatform.hpp"

#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"

namespace tops {

//! This class provides an interface for working with the prefix  sum arrays.
class DLLEXPORT PrefixSumArray
{
public:
  virtual ~PrefixSumArray(){};

  //! Initialize the prefix sum array
  virtual void initialize(const Sequence & s) = 0;

  //! Initialize the prefix sum array
  virtual void initialize(const Sequence & s, int phase) {
    initialize(s);
  }

  //! compute the prefix sum array
  virtual double compute(int begin, int end ) const = 0;

  //! compute the prefix sum array
  virtual double compute (int begin, int end, int start_phase) const {
    return compute(begin, end);
  }
};

typedef boost::shared_ptr <PrefixSumArray> PrefixSumArrayPtr;
}

#endif
