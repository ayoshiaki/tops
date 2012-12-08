#include "ConfigurationNode.hpp"

namespace tops {
  namespace lang {

    void ConfigurationNode::addProperty(PPropertyNode property) {
      _properties.push_back(property);
    }

    std::string ConfigurationNode::str() {
      std::string str = "(ConfigurationNode";
      std::vector<PPropertyNode>::iterator it;
      for (it = _properties.begin() ; it != _properties.end(); ++it) {
        str += " " + (*it)->str();
      }
      str += ")";
      return str;
    }
  }
}
