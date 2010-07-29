#ifndef SEQUENCE_FORMAT_HPP
#define SEQUENCE_FORMAT_HPP

#include <istream>
#include <ostream>
#include <boost/shared_ptr.hpp>


namespace tops {
  class SequenceEntry;
  class SequenceFormat {
  public:
    SequenceFormat(){};
    virtual ~SequenceFormat(){}
    virtual std::ostream & saveSequence (std::ostream & stream, SequenceEntry & out);
    virtual std::istream & readSequence (std::istream & stream, SequenceEntry & in) ;
  };
  typedef boost::shared_ptr<SequenceFormat> SequenceFormatPtr;


  class FastaSequenceFormat : public SequenceFormat {
  private:
    std::string _nextFastaHeader;
    std::string _currentFastaHeader;
    std::string _currentSequence;
  public:
    FastaSequenceFormat(){};
    virtual ~FastaSequenceFormat(){}
    virtual std::ostream & saveSequence (std::ostream & stream, SequenceEntry & out);
    virtual std::istream & readSequence (std::istream & stream, SequenceEntry & in) ;
  };
  typedef boost::shared_ptr <FastaSequenceFormat> FastaSequenceFormatPtr;

  class SequenceFormatManager;
  typedef boost::shared_ptr <SequenceFormatManager> SequenceFormatManagerPtr;
  
  class SequenceFormatManager {
  public:
    static SequenceFormatManagerPtr instance();
    virtual ~SequenceFormatManager(){};
    virtual void setFormat(SequenceFormatPtr format) {
      _format = format;
    }
    virtual SequenceFormatPtr getFormat(){
      return _format;
    }
  protected:
    SequenceFormatManager() {
      _format = SequenceFormatPtr(new SequenceFormat());
    };
  private:
    static SequenceFormatManagerPtr _inst;
    SequenceFormatPtr _format;
  };

};

#endif
