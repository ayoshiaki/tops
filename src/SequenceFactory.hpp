#ifndef SEQUENCE_FACTORY_H
#define SEQUENCE_FACTORY_H
#include <boost/shared_ptr.hpp>
#include "Sequence.hpp"

namespace tops{
  class Alphabet;
  typedef boost::shared_ptr<Alphabet> AlphabetPtr;
  struct my_tolower
  {
    char operator()(char c) const
    {
      return tolower(c);
    }
  };
  
  //! Provides factory methods for creating objects of type Sequence
  class SequenceFactory
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
  };
}
#endif
