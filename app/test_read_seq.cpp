/*
 *       test_read_seq.cpp
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

#include <boost/program_options.hpp>

#include <map>
#include <iostream>
#include <vector>
#include <string>

#include "SequenceFormat.hpp"
#include "SequenceEntry.hpp"
#include "Alphabet.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
using namespace tops;
using namespace std;
using namespace boost::program_options;

int main(int argc, char ** argv) {
  AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
  alphabet->createSymbol("A");
  alphabet->createSymbol("C");
  alphabet->createSymbol("G");
  alphabet->createSymbol("T");
  //  SequenceFormatManager::instance()->setFormat(SequenceFormatPtr(new FastaSequenceFormat()));
  while(!cin.eof())
    {
      SequenceEntryPtr  inseq = SequenceEntryPtr(new SequenceEntry(alphabet));
      cin >> *inseq;
      cout << *inseq;
    }

  return 0;
}
