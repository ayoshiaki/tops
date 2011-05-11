/*
 *       StoreLoadedModel.hpp
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

#ifndef STORE_LOADED_MODEL_HPP
#define STORE_LOADED_MODEL_HPP

#include "crossplatform.hpp"

#include <boost/shared_ptr.hpp>
#include "ProbabilisticModel.hpp"
namespace tops {

  class DLLEXPORT StoreLoadedModel;

  class DLLEXPORT StoreLoadedModel {
  public:

      virtual ~StoreLoadedModel(){};


      virtual ProbabilisticModelPtr add(std::string  name, ProbabilisticModelPtr model) ;

      virtual  ProbabilisticModelPtr get(std::string  name) ;

      static StoreLoadedModel * instance() ;


  protected:
    StoreLoadedModel() {
    };
  private:
      ProbabilisticModelPtr null;
      static StoreLoadedModel * _inst;
      std::map <std::string, ProbabilisticModelPtr> _models;
  };


}


#endif
