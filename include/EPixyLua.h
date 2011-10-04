#ifndef H_EPixyLua_H
#define H_EPixyLua_H

#include "PixyLua.h"
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
  class ELuaExporter : public LuaExporter<T>
  {
    TOLUA_TEMPLATE_BIND(T)

    public:
      ELuaExporter() { }
      virtual ~ELuaExporter() {}


      inline virtual void
      __export(Net::instance* in_instance, T const& container, const char* data_type, const char* out_table)
      {
        LuaExporter<T>::__export(in_instance->_get_lua(), container, data_type, out_table);
      }
  };
}

#endif
