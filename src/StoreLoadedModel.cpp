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
