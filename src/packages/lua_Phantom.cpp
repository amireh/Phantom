/*
** Lua binding: Phantom
** Generated automatically by tolua++-1.0.92 on Sun Oct 16 18:02:45 2011.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Phantom_open (lua_State* tolua_S);

#include "required.h"
#include "lua_exporter.hpp"
#include "Puppet.h"
#include "Unit.h"
#include "Spell.h"
#include "Entity.h"
#include "Puppet.h"
#include "Spell.h"
#include "instance.hpp"
#include "player.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Pixy__lua_exporter_SpellList_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<SpellList>* self = (Pixy::lua_exporter<SpellList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__Entity (lua_State* tolua_S)
{
 Pixy::Entity* self = (Pixy::Entity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__lua_exporter_UnitVec_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<UnitVec>* self = (Pixy::lua_exporter<UnitVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__lua_exporter_PuppetList_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<PuppetList>* self = (Pixy::lua_exporter<PuppetList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__lua_exporter_SpellVec_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<SpellVec>* self = (Pixy::lua_exporter<SpellVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__lua_exporter_PuppetVec_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<PuppetVec>* self = (Pixy::lua_exporter<PuppetVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__lua_exporter_UnitList_ (lua_State* tolua_S)
{
 Pixy::lua_exporter<UnitList>* self = (Pixy::lua_exporter<UnitList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Pixy::lua_exporter<UnitVec>");
 tolua_usertype(tolua_S,"Pixy::BaseCaster::spell_t");
 tolua_usertype(tolua_S,"Pixy::BasePuppet");
 tolua_usertype(tolua_S,"Pixy::Spell");
 tolua_usertype(tolua_S,"Pixy::lua_exporter<UnitList>");
 tolua_usertype(tolua_S,"Pixy::Event");
 tolua_usertype(tolua_S,"BaseLuaExporter");
 tolua_usertype(tolua_S,"BaseSpell");
 tolua_usertype(tolua_S,"Pixy::lua_exporter<PuppetList>");
 tolua_usertype(tolua_S,"const");
 tolua_usertype(tolua_S,"Pixy::lua_exporter<SpellVec>");
 tolua_usertype(tolua_S,"Pixy::Puppet");
 tolua_usertype(tolua_S,"Pixy::Net::instance");
 tolua_usertype(tolua_S,"Pixy::BasePuppet::deck_t");
 tolua_usertype(tolua_S,"Pixy::Entity");
 tolua_usertype(tolua_S,"Pixy::lua_exporter<SpellList>");
 tolua_usertype(tolua_S,"Pixy::BaseDeck");
 tolua_usertype(tolua_S,"Pixy::Unit");
 tolua_usertype(tolua_S,"Pixy::BaseCaster");
 tolua_usertype(tolua_S,"Pixy::BaseEntity");
 tolua_usertype(tolua_S,"Pixy::lua_exporter<PuppetVec>");
}

/* method: new of class  Pixy::lua_exporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitVecExporter_new00
static int tolua_Phantom_Pixy_UnitVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<UnitVec>* tolua_ret = (Pixy::lua_exporter<UnitVec>*)  Mtolua_new((Pixy::lua_exporter<UnitVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<UnitVec>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitVecExporter_new00_local
static int tolua_Phantom_Pixy_UnitVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<UnitVec>* tolua_ret = (Pixy::lua_exporter<UnitVec>*)  Mtolua_new((Pixy::lua_exporter<UnitVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<UnitVec>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitVecExporter_delete00
static int tolua_Phantom_Pixy_UnitVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<UnitVec>* self = (Pixy::lua_exporter<UnitVec>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitVecExporter_export00
static int tolua_Phantom_Pixy_UnitVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<UnitVec>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<UnitVec>* self = (Pixy::lua_exporter<UnitVec>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  UnitVec const* container = ((UnitVec const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::lua_exporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellVecExporter_new00
static int tolua_Phantom_Pixy_SpellVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<SpellVec>* tolua_ret = (Pixy::lua_exporter<SpellVec>*)  Mtolua_new((Pixy::lua_exporter<SpellVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<SpellVec>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellVecExporter_new00_local
static int tolua_Phantom_Pixy_SpellVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<SpellVec>* tolua_ret = (Pixy::lua_exporter<SpellVec>*)  Mtolua_new((Pixy::lua_exporter<SpellVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<SpellVec>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellVecExporter_delete00
static int tolua_Phantom_Pixy_SpellVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<SpellVec>* self = (Pixy::lua_exporter<SpellVec>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellVecExporter_export00
static int tolua_Phantom_Pixy_SpellVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<SpellVec>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<SpellVec>* self = (Pixy::lua_exporter<SpellVec>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  SpellVec const* container = ((SpellVec const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::lua_exporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetVecExporter_new00
static int tolua_Phantom_Pixy_PuppetVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<PuppetVec>* tolua_ret = (Pixy::lua_exporter<PuppetVec>*)  Mtolua_new((Pixy::lua_exporter<PuppetVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<PuppetVec>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetVecExporter_new00_local
static int tolua_Phantom_Pixy_PuppetVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<PuppetVec>* tolua_ret = (Pixy::lua_exporter<PuppetVec>*)  Mtolua_new((Pixy::lua_exporter<PuppetVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<PuppetVec>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetVecExporter_delete00
static int tolua_Phantom_Pixy_PuppetVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<PuppetVec>* self = (Pixy::lua_exporter<PuppetVec>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetVecExporter_export00
static int tolua_Phantom_Pixy_PuppetVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<PuppetVec>* self = (Pixy::lua_exporter<PuppetVec>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  PuppetVec const* container = ((PuppetVec const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::lua_exporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitListExporter_new00
static int tolua_Phantom_Pixy_UnitListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<UnitList>* tolua_ret = (Pixy::lua_exporter<UnitList>*)  Mtolua_new((Pixy::lua_exporter<UnitList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<UnitList>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitListExporter_new00_local
static int tolua_Phantom_Pixy_UnitListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<UnitList>* tolua_ret = (Pixy::lua_exporter<UnitList>*)  Mtolua_new((Pixy::lua_exporter<UnitList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<UnitList>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitListExporter_delete00
static int tolua_Phantom_Pixy_UnitListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<UnitList>* self = (Pixy::lua_exporter<UnitList>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_UnitListExporter_export00
static int tolua_Phantom_Pixy_UnitListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<UnitList>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<UnitList>* self = (Pixy::lua_exporter<UnitList>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  UnitList const* container = ((UnitList const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::lua_exporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellListExporter_new00
static int tolua_Phantom_Pixy_SpellListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<SpellList>* tolua_ret = (Pixy::lua_exporter<SpellList>*)  Mtolua_new((Pixy::lua_exporter<SpellList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<SpellList>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellListExporter_new00_local
static int tolua_Phantom_Pixy_SpellListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<SpellList>* tolua_ret = (Pixy::lua_exporter<SpellList>*)  Mtolua_new((Pixy::lua_exporter<SpellList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<SpellList>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellListExporter_delete00
static int tolua_Phantom_Pixy_SpellListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<SpellList>* self = (Pixy::lua_exporter<SpellList>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_SpellListExporter_export00
static int tolua_Phantom_Pixy_SpellListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<SpellList>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<SpellList>* self = (Pixy::lua_exporter<SpellList>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  SpellList const* container = ((SpellList const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::lua_exporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetListExporter_new00
static int tolua_Phantom_Pixy_PuppetListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<PuppetList>* tolua_ret = (Pixy::lua_exporter<PuppetList>*)  Mtolua_new((Pixy::lua_exporter<PuppetList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<PuppetList>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::lua_exporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetListExporter_new00_local
static int tolua_Phantom_Pixy_PuppetListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::lua_exporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::lua_exporter<PuppetList>* tolua_ret = (Pixy::lua_exporter<PuppetList>*)  Mtolua_new((Pixy::lua_exporter<PuppetList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::lua_exporter<PuppetList>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::lua_exporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetListExporter_delete00
static int tolua_Phantom_Pixy_PuppetListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<PuppetList>* self = (Pixy::lua_exporter<PuppetList>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: __export of class  Pixy::lua_exporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_PuppetListExporter_export00
static int tolua_Phantom_Pixy_PuppetListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::lua_exporter<PuppetList>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::lua_exporter<PuppetList>* self = (Pixy::lua_exporter<PuppetList>*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Net::instance* in_instance = ((Pixy::Net::instance*)  tolua_tousertype(tolua_S,2,0));
  PuppetList const* container = ((PuppetList const*)  tolua_tousertype(tolua_S,3,0));
  const char* data_type = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* out_table = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__export'", NULL);
#endif
  {
   self->__export(in_instance,*container,data_type,out_table);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'export'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_new00
static int tolua_Phantom_Pixy_Entity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::Entity* tolua_ret = (Pixy::Entity*)  Mtolua_new((Pixy::Entity)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_new00_local
static int tolua_Phantom_Pixy_Entity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::Entity* tolua_ret = (Pixy::Entity*)  Mtolua_new((Pixy::Entity)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Entity");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_new01
static int tolua_Phantom_Pixy_Entity_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Pixy::Entity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Pixy::Entity* src = ((const Pixy::Entity*)  tolua_tousertype(tolua_S,2,0));
  {
   Pixy::Entity* tolua_ret = (Pixy::Entity*)  Mtolua_new((Pixy::Entity)(*src));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Entity");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Phantom_Pixy_Entity_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_new01_local
static int tolua_Phantom_Pixy_Entity_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Pixy::Entity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Pixy::Entity* src = ((const Pixy::Entity*)  tolua_tousertype(tolua_S,2,0));
  {
   Pixy::Entity* tolua_ret = (Pixy::Entity*)  Mtolua_new((Pixy::Entity)(*src));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Entity");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_Phantom_Pixy_Entity_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_delete00
static int tolua_Phantom_Pixy_Entity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: live of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_live00
static int tolua_Phantom_Pixy_Entity_live00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'live'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->live();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'live'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: die of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_die00
static int tolua_Phantom_Pixy_Entity_die00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'die'", NULL);
#endif
  {
   self->die();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'die'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachSpell of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_attachSpell00
static int tolua_Phantom_Pixy_Entity_attachSpell00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::BaseCaster::spell_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
  Pixy::BaseCaster::spell_t* inSpell = ((Pixy::BaseCaster::spell_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachSpell'", NULL);
#endif
  {
   self->attachSpell(inSpell);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachSpell'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachBuff of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Entity_attachBuff00
static int tolua_Phantom_Pixy_Entity_attachBuff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::BaseCaster::spell_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
  Pixy::BaseCaster::spell_t* inSpell = ((Pixy::BaseCaster::spell_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachBuff'", NULL);
#endif
  {
   self->attachBuff(inSpell);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachBuff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __Pixy of class  Pixy::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Pixy__Entity___Pixy__BaseCaster__
static int tolua_get_Pixy__Entity___Pixy__BaseCaster__(lua_State* tolua_S)
{
  Pixy::Entity* self = (Pixy::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__Pixy'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<Pixy::BaseCaster*>(self), "Pixy::BaseCaster");
#else
   tolua_pushusertype(tolua_S,(void*)((Pixy::BaseCaster*)self), "Pixy::BaseCaster");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setExperience of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setExperience00
static int tolua_Phantom_Pixy_Puppet_setExperience00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  long inExperience = ((long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setExperience'", NULL);
#endif
  {
   self->setExperience(inExperience);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setExperience'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLevel of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setLevel00
static int tolua_Phantom_Pixy_Puppet_setLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inLevel = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLevel'", NULL);
#endif
  {
   self->setLevel(inLevel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIntelligence of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setIntelligence00
static int tolua_Phantom_Pixy_Puppet_setIntelligence00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inInt = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIntelligence'", NULL);
#endif
  {
   self->setIntelligence(inInt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIntelligence'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVitality of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setVitality00
static int tolua_Phantom_Pixy_Puppet_setVitality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inVt = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVitality'", NULL);
#endif
  {
   self->setVitality(inVt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVitality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWP of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setWP00
static int tolua_Phantom_Pixy_Puppet_setWP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inWP = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWP'", NULL);
#endif
  {
   self->setWP(inWP);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setChannels of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setChannels00
static int tolua_Phantom_Pixy_Puppet_setChannels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_1 = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setChannels'", NULL);
#endif
  {
   self->setChannels(tolua_var_1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setChannels'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWins of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setWins00
static int tolua_Phantom_Pixy_Puppet_setWins00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inWins = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWins'", NULL);
#endif
  {
   self->setWins(inWins);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWins'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLosses of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_setLosses00
static int tolua_Phantom_Pixy_Puppet_setLosses00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inLosses = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLosses'", NULL);
#endif
  {
   self->setLosses(inLosses);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLosses'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getExperience of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getExperience00
static int tolua_Phantom_Pixy_Puppet_getExperience00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getExperience'", NULL);
#endif
  {
   long tolua_ret = (long)  self->getExperience();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getExperience'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLevel of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getLevel00
static int tolua_Phantom_Pixy_Puppet_getLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntelligence of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getIntelligence00
static int tolua_Phantom_Pixy_Puppet_getIntelligence00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntelligence'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getIntelligence();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntelligence'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVitality of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getVitality00
static int tolua_Phantom_Pixy_Puppet_getVitality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVitality'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getVitality();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVitality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWP of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getWP00
static int tolua_Phantom_Pixy_Puppet_getWP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWP'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getWP();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChannels of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getChannels00
static int tolua_Phantom_Pixy_Puppet_getChannels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChannels'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getChannels();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChannels'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWins of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getWins00
static int tolua_Phantom_Pixy_Puppet_getWins00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWins'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getWins();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWins'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLosses of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getLosses00
static int tolua_Phantom_Pixy_Puppet_getLosses00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLosses'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getLosses();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLosses'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachTalent of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_attachTalent00
static int tolua_Phantom_Pixy_Puppet_attachTalent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Talent const* inTalent = ((Pixy::Talent const*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachTalent'", NULL);
#endif
  {
   self->attachTalent(inTalent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachTalent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachTalent of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_detachTalent00
static int tolua_Phantom_Pixy_Puppet_detachTalent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  std::string const* inName = ((std::string const*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachTalent'", NULL);
#endif
  {
   self->detachTalent(*inName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachTalent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTalents of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getTalents00
static int tolua_Phantom_Pixy_Puppet_getTalents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTalents'", NULL);
#endif
  {
   Pixy::BasePuppet::talents_t const& tolua_ret = (Pixy::BasePuppet::talents_t const&)  self->getTalents();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTalents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasTalent of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_hasTalent00
static int tolua_Phantom_Pixy_Puppet_hasTalent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  std::string const* inName = ((std::string const*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasTalent'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasTalent(*inName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasTalent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachDeck of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_attachDeck00
static int tolua_Phantom_Pixy_Puppet_attachDeck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::BaseDeck",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
   Pixy::BaseDeck* inDeck = ((  Pixy::BaseDeck*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachDeck'", NULL);
#endif
  {
   self->attachDeck(inDeck);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachDeck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachDeck of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_detachDeck00
static int tolua_Phantom_Pixy_Puppet_detachDeck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  std::string const* inName = ((std::string const*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachDeck'", NULL);
#endif
  {
   self->detachDeck(*inName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachDeck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _setDeck of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet__setDeck00
static int tolua_Phantom_Pixy_Puppet__setDeck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::BaseDeck",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
   Pixy::BaseDeck* inDeck = ((  Pixy::BaseDeck*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_setDeck'", NULL);
#endif
  {
   self->_setDeck(inDeck);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_setDeck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDeck of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getDeck00
static int tolua_Phantom_Pixy_Puppet_getDeck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDeck'", NULL);
#endif
  {
   Pixy::BasePuppet::deck_t* tolua_ret = (Pixy::BasePuppet::deck_t*)  self->getDeck();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::BasePuppet::deck_t");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDeck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDecks of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getDecks00
static int tolua_Phantom_Pixy_Puppet_getDecks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDecks'", NULL);
#endif
  {
   Pixy::BasePuppet::decks_t const& tolua_ret = (Pixy::BasePuppet::decks_t const&)  self->getDecks();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDecks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasDeck of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_hasDeck00
static int tolua_Phantom_Pixy_Puppet_hasDeck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  std::string const* inName = ((std::string const*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasDeck'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasDeck(*inName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasDeck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: nrDecks of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_nrDecks00
static int tolua_Phantom_Pixy_Puppet_nrDecks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Pixy::Puppet* self = (const Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'nrDecks'", NULL);
#endif
  {
   int tolua_ret = (int)  self->nrDecks();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'nrDecks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachUnit of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_attachUnit00
static int tolua_Phantom_Pixy_Puppet_attachUnit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Pixy::Unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
   Pixy::Unit* inUnit = ((  Pixy::Unit*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachUnit'", NULL);
#endif
  {
   self->attachUnit(inUnit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachUnit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachUnit of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_detachUnit00
static int tolua_Phantom_Pixy_Puppet_detachUnit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inUID = ((int)  tolua_tonumber(tolua_S,2,0));
  bool inRemove = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachUnit'", NULL);
#endif
  {
   self->detachUnit(inUID,inRemove);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachUnit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUnit of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getUnit00
static int tolua_Phantom_Pixy_Puppet_getUnit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
  int inUID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUnit'", NULL);
#endif
  {
    Pixy::Unit* tolua_ret = (  Pixy::Unit*)  self->getUnit(inUID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Unit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUnit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUnits of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Puppet_getUnits00
static int tolua_Phantom_Pixy_Puppet_getUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Puppet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUnits'", NULL);
#endif
  {
   Pixy::Puppet::units_t const& tolua_ret = (Pixy::Puppet::units_t const&)  self->getUnits();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __Pixy of class  Pixy::Puppet */
#ifndef TOLUA_DISABLE_tolua_get_Pixy__Puppet___Pixy__BasePuppet__
static int tolua_get_Pixy__Puppet___Pixy__BasePuppet__(lua_State* tolua_S)
{
  Pixy::Puppet* self = (Pixy::Puppet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__Pixy'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<Pixy::BasePuppet*>(self), "Pixy::BasePuppet");
#else
   tolua_pushusertype(tolua_S,(void*)((Pixy::BasePuppet*)self), "Pixy::BasePuppet");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: instance::lua_log of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_Log00
static int tolua_Phantom_Pixy_Net_instance_Log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  std::string inMsg = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'instance::lua_log'", NULL);
#endif
  {
   self->instance::lua_log(inMsg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: broadcast of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_broadcast00
static int tolua_Phantom_Pixy_Net_instance_broadcast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Pixy::Event",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  const Pixy::Event* tolua_var_2 = ((const Pixy::Event*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'broadcast'", NULL);
#endif
  {
   self->broadcast(*tolua_var_2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'broadcast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_send00
static int tolua_Phantom_Pixy_Net_instance_send00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Pixy::Event",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  int inPuppetUID = ((int)  tolua_tonumber(tolua_S,2,0));
  const Pixy::Event* tolua_var_3 = ((const Pixy::Event*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send'", NULL);
#endif
  {
   self->send(inPuppetUID,*tolua_var_3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _create_unit of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_create_unit00
static int tolua_Phantom_Pixy_Net_instance_create_unit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"Pixy::Puppet",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  std::string model = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  Pixy::Puppet* owner = ((Pixy::Puppet*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_create_unit'", NULL);
#endif
  {
   Pixy::Unit* tolua_ret = (Pixy::Unit*)  self->_create_unit(model,*owner);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Unit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_unit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _destroy_unit of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_destroy_unit00
static int tolua_Phantom_Pixy_Net_instance_destroy_unit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  int inUID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_destroy_unit'", NULL);
#endif
  {
   self->_destroy_unit(inUID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy_unit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _destroy_puppet of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_destroy_puppet00
static int tolua_Phantom_Pixy_Net_instance_destroy_puppet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  int inUID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_destroy_puppet'", NULL);
#endif
  {
   self->_destroy_puppet(inUID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy_puppet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _draw_spells of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_draw_spells00
static int tolua_Phantom_Pixy_Net_instance_draw_spells00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_4 = ((int)  tolua_tonumber(tolua_S,2,0));
  int tolua_var_5 = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_draw_spells'", NULL);
#endif
  {
   self->_draw_spells(tolua_var_4,tolua_var_5);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_spells'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_enemy of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_Phantom_Pixy_Net_instance_get_enemy00
static int tolua_Phantom_Pixy_Net_instance_get_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  int inUID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_enemy'", NULL);
#endif
  {
   Pixy::Puppet* tolua_ret = (Pixy::Puppet*)  self->get_enemy(inUID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::Puppet");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_enemy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Phantom_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Pixy",0);
  tolua_beginmodule(tolua_S,"Pixy");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UnitVecExporter","Pixy::lua_exporter<UnitVec>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_UnitVec_);
   #else
   tolua_cclass(tolua_S,"UnitVecExporter","Pixy::lua_exporter<UnitVec>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UnitVecExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_UnitVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_UnitVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_UnitVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_UnitVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_UnitVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SpellVecExporter","Pixy::lua_exporter<SpellVec>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_SpellVec_);
   #else
   tolua_cclass(tolua_S,"SpellVecExporter","Pixy::lua_exporter<SpellVec>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SpellVecExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_SpellVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_SpellVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_SpellVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_SpellVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_SpellVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PuppetVecExporter","Pixy::lua_exporter<PuppetVec>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_PuppetVec_);
   #else
   tolua_cclass(tolua_S,"PuppetVecExporter","Pixy::lua_exporter<PuppetVec>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PuppetVecExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_PuppetVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_PuppetVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_PuppetVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_PuppetVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_PuppetVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UnitListExporter","Pixy::lua_exporter<UnitList>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_UnitList_);
   #else
   tolua_cclass(tolua_S,"UnitListExporter","Pixy::lua_exporter<UnitList>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UnitListExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_UnitListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_UnitListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_UnitListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_UnitListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_UnitListExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SpellListExporter","Pixy::lua_exporter<SpellList>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_SpellList_);
   #else
   tolua_cclass(tolua_S,"SpellListExporter","Pixy::lua_exporter<SpellList>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SpellListExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_SpellListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_SpellListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_SpellListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_SpellListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_SpellListExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PuppetListExporter","Pixy::lua_exporter<PuppetList>","BaseLuaExporter",tolua_collect_Pixy__lua_exporter_PuppetList_);
   #else
   tolua_cclass(tolua_S,"PuppetListExporter","Pixy::lua_exporter<PuppetList>","BaseLuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PuppetListExporter");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_PuppetListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_PuppetListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_PuppetListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_PuppetListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_Phantom_Pixy_PuppetListExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Entity","Pixy::Entity","Pixy::BaseEntity",tolua_collect_Pixy__Entity);
   #else
   tolua_cclass(tolua_S,"Entity","Pixy::Entity","Pixy::BaseEntity",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Entity");
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_Entity_new00);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_Entity_new00_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_Entity_new00_local);
    tolua_function(tolua_S,"new",tolua_Phantom_Pixy_Entity_new01);
    tolua_function(tolua_S,"new_local",tolua_Phantom_Pixy_Entity_new01_local);
    tolua_function(tolua_S,".call",tolua_Phantom_Pixy_Entity_new01_local);
    tolua_function(tolua_S,"delete",tolua_Phantom_Pixy_Entity_delete00);
    tolua_function(tolua_S,"live",tolua_Phantom_Pixy_Entity_live00);
    tolua_function(tolua_S,"die",tolua_Phantom_Pixy_Entity_die00);
    tolua_function(tolua_S,"attachSpell",tolua_Phantom_Pixy_Entity_attachSpell00);
    tolua_function(tolua_S,"attachBuff",tolua_Phantom_Pixy_Entity_attachBuff00);
    tolua_variable(tolua_S,"__Pixy__BaseCaster__",tolua_get_Pixy__Entity___Pixy__BaseCaster__,NULL);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Unit","Pixy::Unit","",NULL);
   tolua_beginmodule(tolua_S,"Unit");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Puppet","Pixy::Puppet","Pixy::Entity",NULL);
   tolua_beginmodule(tolua_S,"Puppet");
    tolua_function(tolua_S,"setExperience",tolua_Phantom_Pixy_Puppet_setExperience00);
    tolua_function(tolua_S,"setLevel",tolua_Phantom_Pixy_Puppet_setLevel00);
    tolua_function(tolua_S,"setIntelligence",tolua_Phantom_Pixy_Puppet_setIntelligence00);
    tolua_function(tolua_S,"setVitality",tolua_Phantom_Pixy_Puppet_setVitality00);
    tolua_function(tolua_S,"setWP",tolua_Phantom_Pixy_Puppet_setWP00);
    tolua_function(tolua_S,"setChannels",tolua_Phantom_Pixy_Puppet_setChannels00);
    tolua_function(tolua_S,"setWins",tolua_Phantom_Pixy_Puppet_setWins00);
    tolua_function(tolua_S,"setLosses",tolua_Phantom_Pixy_Puppet_setLosses00);
    tolua_function(tolua_S,"getExperience",tolua_Phantom_Pixy_Puppet_getExperience00);
    tolua_function(tolua_S,"getLevel",tolua_Phantom_Pixy_Puppet_getLevel00);
    tolua_function(tolua_S,"getIntelligence",tolua_Phantom_Pixy_Puppet_getIntelligence00);
    tolua_function(tolua_S,"getVitality",tolua_Phantom_Pixy_Puppet_getVitality00);
    tolua_function(tolua_S,"getWP",tolua_Phantom_Pixy_Puppet_getWP00);
    tolua_function(tolua_S,"getChannels",tolua_Phantom_Pixy_Puppet_getChannels00);
    tolua_function(tolua_S,"getWins",tolua_Phantom_Pixy_Puppet_getWins00);
    tolua_function(tolua_S,"getLosses",tolua_Phantom_Pixy_Puppet_getLosses00);
    tolua_function(tolua_S,"attachTalent",tolua_Phantom_Pixy_Puppet_attachTalent00);
    tolua_function(tolua_S,"detachTalent",tolua_Phantom_Pixy_Puppet_detachTalent00);
    tolua_function(tolua_S,"getTalents",tolua_Phantom_Pixy_Puppet_getTalents00);
    tolua_function(tolua_S,"hasTalent",tolua_Phantom_Pixy_Puppet_hasTalent00);
    tolua_function(tolua_S,"attachDeck",tolua_Phantom_Pixy_Puppet_attachDeck00);
    tolua_function(tolua_S,"detachDeck",tolua_Phantom_Pixy_Puppet_detachDeck00);
    tolua_function(tolua_S,"_setDeck",tolua_Phantom_Pixy_Puppet__setDeck00);
    tolua_function(tolua_S,"getDeck",tolua_Phantom_Pixy_Puppet_getDeck00);
    tolua_function(tolua_S,"getDecks",tolua_Phantom_Pixy_Puppet_getDecks00);
    tolua_function(tolua_S,"hasDeck",tolua_Phantom_Pixy_Puppet_hasDeck00);
    tolua_function(tolua_S,"nrDecks",tolua_Phantom_Pixy_Puppet_nrDecks00);
    tolua_function(tolua_S,"attachUnit",tolua_Phantom_Pixy_Puppet_attachUnit00);
    tolua_function(tolua_S,"detachUnit",tolua_Phantom_Pixy_Puppet_detachUnit00);
    tolua_function(tolua_S,"getUnit",tolua_Phantom_Pixy_Puppet_getUnit00);
    tolua_function(tolua_S,"getUnits",tolua_Phantom_Pixy_Puppet_getUnits00);
    tolua_variable(tolua_S,"__Pixy__BasePuppet__",tolua_get_Pixy__Puppet___Pixy__BasePuppet__,NULL);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Spell","Pixy::Spell","BaseSpell",NULL);
   tolua_beginmodule(tolua_S,"Spell");
   tolua_endmodule(tolua_S);
   tolua_module(tolua_S,"Net",0);
   tolua_beginmodule(tolua_S,"Net");
    tolua_cclass(tolua_S,"instance","Pixy::Net::instance","",NULL);
    tolua_beginmodule(tolua_S,"instance");
     tolua_function(tolua_S,"Log",tolua_Phantom_Pixy_Net_instance_Log00);
     tolua_function(tolua_S,"broadcast",tolua_Phantom_Pixy_Net_instance_broadcast00);
     tolua_function(tolua_S,"send",tolua_Phantom_Pixy_Net_instance_send00);
     tolua_function(tolua_S,"create_unit",tolua_Phantom_Pixy_Net_instance_create_unit00);
     tolua_function(tolua_S,"destroy_unit",tolua_Phantom_Pixy_Net_instance_destroy_unit00);
     tolua_function(tolua_S,"destroy_puppet",tolua_Phantom_Pixy_Net_instance_destroy_puppet00);
     tolua_function(tolua_S,"draw_spells",tolua_Phantom_Pixy_Net_instance_draw_spells00);
     tolua_function(tolua_S,"get_enemy",tolua_Phantom_Pixy_Net_instance_get_enemy00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Phantom (lua_State* tolua_S) {
 return tolua_Phantom_open(tolua_S);
};
#endif

