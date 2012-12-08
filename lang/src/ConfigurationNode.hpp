#ifndef TOPS_LANG_CONFIGURATIONODE_HPP
#define TOPS_LANG_CONFIGURATIONODE_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ValueNode.hpp"
#include "PropertyNode.hpp"

namespace tops {
  namespace lang {
    class ConfigurationNode: public ValueNode {
    public:
      ConfigurationNode(std::vector<PPropertyNode> properties):_properties(properties) {}

      void addProperty(PPropertyNode property);

      virtual std::string str();
    private:
      std::vector<PPropertyNode> _properties;
    };
    
    typedef boost::shared_ptr<ConfigurationNode> PConfigurationNode;
  }
}

#endif
