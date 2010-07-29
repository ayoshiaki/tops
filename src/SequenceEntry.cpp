#include "SequenceEntry.hpp"
#include <istream>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "util.hpp"
namespace tops {
  //! Save the sequence 
  std::ostream & operator << (std::ostream & stream, SequenceEntry & out) {
    return out._format->saveSequence(stream, out);
  }
  
  //! Load the sequence 
  std::istream & operator >> (std::istream & stream, SequenceEntry &in){
    return in._format->readSequence(stream, in);
  }
}
