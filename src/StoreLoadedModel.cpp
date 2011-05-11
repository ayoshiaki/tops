/*
 *       StoreLoadedModel.cpp
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

#include "StoreLoadedModel.hpp"

namespace tops {
       ProbabilisticModelPtr StoreLoadedModel::add(std::string  name, ProbabilisticModelPtr model) {
          _models[name] = model;
          return model;
      }
       ProbabilisticModelPtr StoreLoadedModel::get(std::string  name) {
          if(_models.find(name) == _models.end())
              return null;
          else
              return _models.find(name)->second;
      }
    StoreLoadedModel * StoreLoadedModel::_inst ;
    StoreLoadedModel * StoreLoadedModel::instance() {
        if(!_inst) {
            _inst = new StoreLoadedModel();
        }
        return _inst;
    }

}
