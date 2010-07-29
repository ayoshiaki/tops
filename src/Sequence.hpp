#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <boost/pool/pool_alloc.hpp>
#include <vector>

namespace tops {
  typedef std::vector<int> Sequence;
  typedef std::vector<Sequence> SequenceList;
}

#endif
