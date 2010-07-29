#ifndef Sequence_ENTRY_HPP
#define Sequence_ENTRY_HPP
#include <istream>
#include <ostream>
#include <string>
#include <boost/shared_ptr.hpp>

#include "SequenceFactory.hpp"
#include "SequenceFormat.hpp"
#include "Sequence.hpp"

namespace tops {

  class SequenceEntry {
  private:
    friend std::ostream & operator << (std::ostream & stream, SequenceEntry & out);
    friend std::istream & operator >> (std::istream & stream, SequenceEntry & in);
    std::string _name;
    std::string _description;
    Sequence _sequence;
    AlphabetPtr _alphabet;
    std::string _sep;
    SequenceFormatPtr _format;
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
