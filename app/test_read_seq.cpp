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
  SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));
  while(!cin.eof())
    {
      SequenceEntryPtr  inseq = SequenceEntryPtr(new SequenceEntry(alphabet));
      cin >> *inseq;
      cout << *inseq;
    }
  
  return 0;
}
