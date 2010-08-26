#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "SequenceFactory.hpp"


#include <iostream>
#include <fstream>
#include <sstream>

namespace tops {
  Sequence  SequenceFactory::createSequenceRemovedSpaces(const std::string & seq)
  {
    Sequence result;
    int i;
    int start = 0;
    //  transform(seq.begin(), seq.end(), seq.begin(), my_tolower());
    if(_alphabet->size() == 0)
      {
	std::istringstream instream; 
	instream.str(seq);
	while(instream.good()) {
	  double v;
	  instream >> v;
	  result.push_back (v);
	}
	return result;
      }
    if(seq.size() > 0) 
      for(i = 0 ; i < (int)seq.size(); i++)
      {
	    std::string substr = seq.substr(start, i-start+1);
	    if((substr.size() > 0) && _alphabet->has(substr))
	      {
		result.push_back (_alphabet->getSymbol(substr)->id());
	      } 
	    else 
	      {
		if(substr.size() >0) {
		  result.push_back (0);
		  //		  std::cerr << "WARNING: Invalid symbol at position " <<  start <<": '" << substr << "'" << std::endl;
		}
	      }
	    start = i+1;
      }
      std::string substr = seq.substr(start, seq.size()-start);
      if((substr.size() > 0) && _alphabet->has(substr))
	{
	  result.push_back (_alphabet->getSymbol(substr)->id());
	} 
      
    return result;
  }

  Sequence  SequenceFactory::createSequence(const std::string & seq)
  {
    Sequence result;
    int i;
    int start = 0;
    //  transform(seq.begin(), seq.end(), seq.begin(), my_tolower());
    if(_alphabet->size() == 0)
      {
	std::istringstream instream; 
	instream.str(seq);
	while(instream.good()) {
	  double v;
	  instream >> v;
	  result.push_back (v);
	}
	return result;
      }
    if(seq.size() > 0) 
      for(i = 0 ; i < (int)seq.size(); i++)
      {
	if(seq[i] == ' ')
	  {
	    std::string substr = seq.substr(start, i-start);
	    if((substr.size() > 0) && _alphabet->has(substr))
	      {
		result.push_back (_alphabet->getSymbol(substr)->id());
	      } 
	    else 
	      {
		if(substr.size() >0) {
		  result.push_back (0);
		  std::cerr << "WARNING: Invalid symbol at position " <<  start <<": '" << substr << "'" << std::endl;
		}
	      }
	    start = i+1;
	  }
      }
    std::string substr = seq.substr(start, seq.size()-start);
    if((substr.size() > 0) && _alphabet->has(substr))
      {
	result.push_back (_alphabet->getSymbol(substr)->id());
      } 
    
    return result;
  }
  
  Sequence SequenceFactory::createSequence(const std::vector<std::string> & seq)
  {
    Sequence result;
    
    if(_alphabet->size() == 0)
      {
	std::istringstream instream; 
	for(int i = 0 ; i < (int)seq.size(); i++)
	  {
	    instream.clear();
	    instream.str(seq[i]);
	    double v;
	    instream >> v;
            result.push_back (v);
	  }
      }
    else 
      {
	for(int i = 0 ; i < (int)seq.size(); i++){
	  if(_alphabet->getSymbol(seq[i])->id()<0){
            result.push_back (0);
	    std::cerr << "WARNING: Invalid symbol at position " <<  i  <<": '" << seq[i] << "'" << std::endl;
	  }
	  else
	    result.push_back (_alphabet->getSymbol(seq[i])->id());
	}
      }
    return result;
  }
}
