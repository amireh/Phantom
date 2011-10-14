#ifndef H_LuaExporter_H
#define H_LuaExporter_H

#include "BaseLuaExporter.h"
#include "instance.hpp"

typedef std::vector<Pixy::Unit*> UnitVec;
typedef std::vector<Pixy::Spell*> SpellVec;
typedef std::vector<Pixy::Puppet*> PuppetVec;
typedef std::list<Pixy::Unit*> UnitList;
typedef std::list<Pixy::Spell*> SpellList;
typedef std::list<Pixy::Puppet*> PuppetList;

namespace Pixy {

  using std::list;

  template <typename T>
  class lua_exporter : public BaseLuaExporter<T>
  {
    TOLUA_TEMPLATE_BIND(T)

    public:
      lua_exporter() { }
      virtual ~lua_exporter() {}


      inline virtual void
      __export(Net::instance* in_instance, T const& container, const char* data_type, const char* out_table)
      {
        BaseLuaExporter<T>::__export(in_instance->_get_lua(), container, data_type, out_table);
      }
  };
}

#endif
