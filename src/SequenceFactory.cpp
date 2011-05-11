/*
 *       SequenceFactory.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
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

#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "SequenceFactory.hpp"


#include <iostream>
#include <fstream>
#include <sstream>

namespace tops {


    Sequence SequenceFactory::createSequence(const std::string & seq){
        std::vector<int> inv;
        return createSequence(seq, inv);
    }
    Sequence SequenceFactory::createSequenceRemovedSpaces(const std::string & seq){
        std::vector<int> inv;
        return createSequenceRemovedSpaces(seq, inv);
    }
    Sequence SequenceFactory::createSequence(const std::vector<std::string> & seq){
        std::vector<int> inv;
        return createSequence(seq, inv);
    }
    Sequence  SequenceFactory::createSequenceRemovedSpaces(const std::string & seq, std::vector<int> & invalidPositions)
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
                  std::cerr << "WARNING: Invalid symbol at position " <<  start <<": '" << substr << "'" << std::endl;
                  invalidPositions.push_back(result.size()-1);
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

  Sequence  SequenceFactory::createSequence(const std::string & seq, std::vector<int> & invalidPositions)
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
                  invalidPositions.push_back(result.size()-1);
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

  Sequence SequenceFactory::createSequence(const std::vector<std::string> & seq, std::vector<int> & invalidPositions)
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
          else{
            result.push_back (_alphabet->getSymbol(seq[i])->id());
            invalidPositions.push_back(result.size()-1);
          }
        }
      }
    return result;
  }
}
