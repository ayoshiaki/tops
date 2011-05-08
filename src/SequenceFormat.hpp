/*
 *       SequenceFormat.hpp
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

#ifndef SEQUENCE_FORMAT_HPP
#define SEQUENCE_FORMAT_HPP

#include "crossplatform.hpp"

#include <istream>
#include <ostream>
#include <boost/shared_ptr.hpp>


namespace tops {
  class DLLEXPORT SequenceEntry;
  //! Represents a  format for the sequence
  class DLLEXPORT SequenceFormat {
  public:
    SequenceFormat(){};
    virtual ~SequenceFormat(){}
    virtual std::ostream & saveSequence (std::ostream & stream, SequenceEntry & out);
    virtual std::istream & readSequence (std::istream & stream, SequenceEntry & in) ;
  };
  typedef boost::shared_ptr<SequenceFormat> SequenceFormatPtr;

  //! Fasta Format
  class DLLEXPORT FastaSequenceFormat : public SequenceFormat {
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
  
  class DLLEXPORT SequenceFormatManager;
  typedef boost::shared_ptr <SequenceFormatManager> SequenceFormatManagerPtr;
  //! Sequence Format Manager
  class DLLEXPORT SequenceFormatManager {
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
