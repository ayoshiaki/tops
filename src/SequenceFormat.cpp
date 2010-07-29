#include "SequenceFormat.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFactory.hpp"
#include "util.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

namespace tops {

  std::ostream & SequenceFormat::saveSequence (std::ostream & stream, SequenceEntry & out){
    stream << out.getName() << " " << out.getDescription() << ":\t" ;
    if((out.getAlphabet())->size() > 0)
      for(int i = 0; i < (int)out.getSequence().size(); i+=1){
	  stream << (out.getAlphabet())->getSymbol((out.getSequence())[i])->name() << out.getSeparator();
      }
    else
      for(int i = 0; i < (int)out.getSequence().size(); i+=1){
	stream << (out.getSequence())[i] << out.getSeparator();
      }
    stream << std::endl;
    return stream;
    
  }
  std::istream & SequenceFormat::readSequence (std::istream & stream, SequenceEntry & in) {
    std::string line;
    std::string header;
    std::string sequence;
    SequenceFactory factory(in.getAlphabet());
    in.getSequence() = factory.createSequence(sequence);
    
    if(stream.bad()){
      return stream;
    }
    while(!stream.eof()){
      getline(stream,line,'\n');

      // Ignores blank lines
      unsigned int i;
      for(i = 0; i < line.length();  i++)
        if(!isspace(line[i]) && line[i] != '\n'){
          break;
        }
      if(line[i] == '\0'){
        continue;
      }
      trim_spaces(line);

      std::vector<std::string> seq_entry;
      boost::regex separator(":\t");
      split_regex(line, seq_entry, separator);
      if(seq_entry.size() != 2)
	{
	  std::cerr << "Invalid sequence entry !" << std::endl;
	  exit(-1);
	}

      boost::regex sep(" ");
      std::vector <std::string> name_and_description;
      split_regex(seq_entry[0], name_and_description, sep);
      in.setName( name_and_description[0]);
      std::string description;
      for(int i = 1; i < (int)name_and_description.size(); i++)
	description += name_and_description[i];
      in.setDescription ( description );
      boost::regex symbol_separator(in.getSeparator());
      trim_spaces(seq_entry[1]);
      in.setSequence(factory.createSequence(seq_entry[1]));
      return stream;
    }
    return stream;
  }
  std::ostream & FastaSequenceFormat::saveSequence (std::ostream & stream, SequenceEntry & out)
  {
    stream << ">" << out.getName() << " " << out.getDescription() << std::endl;
    if((out.getAlphabet())->size() > 0)
      for(int i = 0; i < (int)out.getSequence().size(); i+=1){
	stream << (out.getAlphabet())->getSymbol((out.getSequence())[i])->name() ;
      }
    else
      for(int i = 0; i < (int)out.getSequence().size(); i+=1)
	stream << (out.getSequence())[i] << out.getSeparator();
    stream << std::endl;
    return stream;
  }
  std::istream & FastaSequenceFormat::readSequence (std::istream & stream, SequenceEntry & in) 
  {
    std::string line;
    std::string sequence ;
    bool firstSeq = true;
    if(_nextFastaHeader.length() > 0) {
      _currentFastaHeader = _nextFastaHeader;
      firstSeq = false;
    }
    while (!stream.eof()) {
      std::getline(stream, line, '\n');
      unsigned int i;
      for (i = 0; i < line.length(); i++)
	if(!isspace(line[i]) && (line[i] != '\n'))
	  break;
      if(i == line.length()) 
	continue;
      
      if(line[0] == '>') {
	if(firstSeq == true){
	  _currentFastaHeader = line;
	  firstSeq =false;
	  continue;
	}
	else {
	  _nextFastaHeader = line;
	  break;
	}
      }
      sequence += line;
    }
    int j = 0;
    for (int i = 0; i < (int)sequence.length(); i++)
      {
	if(isspace(sequence[i]) || (sequence[i] == '\n') )
	   continue;
	sequence[j] = sequence[i];
	j++;
      }
    sequence = sequence.substr(0,j);

    SequenceFactory factory(in.getAlphabet());
    trim_spaces(sequence);
    in.setSequence(factory.createSequenceRemovedSpaces(sequence));

    boost::regex sep(" ");
    std::vector <std::string> name_and_description;
    split_regex(_currentFastaHeader, name_and_description, sep);

    in.setName( name_and_description[0].substr(1, name_and_description[0].length()-1));
    std::string description;
    for(int i = 1; i < (int)name_and_description.size(); i++)
      description += name_and_description[i];
    in.setDescription ( description );
    
    return stream;
  }
  
  SequenceFormatManagerPtr SequenceFormatManager::_inst ;
  SequenceFormatManagerPtr SequenceFormatManager::instance() {
    if(!_inst) {
      _inst = SequenceFormatManagerPtr(new SequenceFormatManager());
    } 
    return _inst;
  }
}
