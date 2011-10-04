/*
** Lua binding: EServer
** Generated automatically by tolua++-1.0.92 on Mon Oct  3 14:57:03 2011.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EServer_open (lua_State* tolua_S);

#include "required.h"
#include "EPixyLua.h"
#include "Puppet.h"
#include "Unit.h"
#include "Spell.h"
#include "instance.hpp"
#include "player.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Pixy__ELuaExporter_UnitVec_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<UnitVec>* self = (Pixy::ELuaExporter<UnitVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__ELuaExporter_SpellVec_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<SpellVec>* self = (Pixy::ELuaExporter<SpellVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__ELuaExporter_PuppetVec_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<PuppetVec>* self = (Pixy::ELuaExporter<PuppetVec>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__ELuaExporter_PuppetList_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<PuppetList>* self = (Pixy::ELuaExporter<PuppetList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__ELuaExporter_SpellList_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<SpellList>* self = (Pixy::ELuaExporter<SpellList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Pixy__ELuaExporter_UnitList_ (lua_State* tolua_S)
{
 Pixy::ELuaExporter<UnitList>* self = (Pixy::ELuaExporter<UnitList>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"const");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<UnitList>");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<SpellList>");
 tolua_usertype(tolua_S,"Pixy::Puppet");
 tolua_usertype(tolua_S,"Pixy::Net::instance");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<UnitVec>");
 tolua_usertype(tolua_S,"Pixy::Event");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<SpellVec>");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<PuppetVec>");
 tolua_usertype(tolua_S,"Pixy::Unit");
 tolua_usertype(tolua_S,"Pixy::ELuaExporter<PuppetList>");
 tolua_usertype(tolua_S,"LuaExporter");
}

/* method: new of class  Pixy::ELuaExporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitVecExporter_new00
static int tolua_EServer_Pixy_UnitVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<UnitVec>* tolua_ret = (Pixy::ELuaExporter<UnitVec>*)  Mtolua_new((Pixy::ELuaExporter<UnitVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<UnitVec>");
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

/* method: new_local of class  Pixy::ELuaExporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitVecExporter_new00_local
static int tolua_EServer_Pixy_UnitVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<UnitVec>* tolua_ret = (Pixy::ELuaExporter<UnitVec>*)  Mtolua_new((Pixy::ELuaExporter<UnitVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<UnitVec>");
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

/* method: delete of class  Pixy::ELuaExporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitVecExporter_delete00
static int tolua_EServer_Pixy_UnitVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<UnitVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<UnitVec>* self = (Pixy::ELuaExporter<UnitVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<UnitVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitVecExporter_export00
static int tolua_EServer_Pixy_UnitVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<UnitVec>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<UnitVec>* self = (Pixy::ELuaExporter<UnitVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Pixy::ELuaExporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellVecExporter_new00
static int tolua_EServer_Pixy_SpellVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<SpellVec>* tolua_ret = (Pixy::ELuaExporter<SpellVec>*)  Mtolua_new((Pixy::ELuaExporter<SpellVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<SpellVec>");
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

/* method: new_local of class  Pixy::ELuaExporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellVecExporter_new00_local
static int tolua_EServer_Pixy_SpellVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<SpellVec>* tolua_ret = (Pixy::ELuaExporter<SpellVec>*)  Mtolua_new((Pixy::ELuaExporter<SpellVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<SpellVec>");
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

/* method: delete of class  Pixy::ELuaExporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellVecExporter_delete00
static int tolua_EServer_Pixy_SpellVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<SpellVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<SpellVec>* self = (Pixy::ELuaExporter<SpellVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<SpellVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellVecExporter_export00
static int tolua_EServer_Pixy_SpellVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<SpellVec>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<SpellVec>* self = (Pixy::ELuaExporter<SpellVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Pixy::ELuaExporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetVecExporter_new00
static int tolua_EServer_Pixy_PuppetVecExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<PuppetVec>* tolua_ret = (Pixy::ELuaExporter<PuppetVec>*)  Mtolua_new((Pixy::ELuaExporter<PuppetVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<PuppetVec>");
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

/* method: new_local of class  Pixy::ELuaExporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetVecExporter_new00_local
static int tolua_EServer_Pixy_PuppetVecExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<PuppetVec>* tolua_ret = (Pixy::ELuaExporter<PuppetVec>*)  Mtolua_new((Pixy::ELuaExporter<PuppetVec>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<PuppetVec>");
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

/* method: delete of class  Pixy::ELuaExporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetVecExporter_delete00
static int tolua_EServer_Pixy_PuppetVecExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<PuppetVec>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<PuppetVec>* self = (Pixy::ELuaExporter<PuppetVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<PuppetVec> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetVecExporter_export00
static int tolua_EServer_Pixy_PuppetVecExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<PuppetVec>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<PuppetVec>* self = (Pixy::ELuaExporter<PuppetVec>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Pixy::ELuaExporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitListExporter_new00
static int tolua_EServer_Pixy_UnitListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<UnitList>* tolua_ret = (Pixy::ELuaExporter<UnitList>*)  Mtolua_new((Pixy::ELuaExporter<UnitList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<UnitList>");
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

/* method: new_local of class  Pixy::ELuaExporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitListExporter_new00_local
static int tolua_EServer_Pixy_UnitListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<UnitList>* tolua_ret = (Pixy::ELuaExporter<UnitList>*)  Mtolua_new((Pixy::ELuaExporter<UnitList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<UnitList>");
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

/* method: delete of class  Pixy::ELuaExporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitListExporter_delete00
static int tolua_EServer_Pixy_UnitListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<UnitList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<UnitList>* self = (Pixy::ELuaExporter<UnitList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<UnitList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_UnitListExporter_export00
static int tolua_EServer_Pixy_UnitListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<UnitList>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<UnitList>* self = (Pixy::ELuaExporter<UnitList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Pixy::ELuaExporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellListExporter_new00
static int tolua_EServer_Pixy_SpellListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<SpellList>* tolua_ret = (Pixy::ELuaExporter<SpellList>*)  Mtolua_new((Pixy::ELuaExporter<SpellList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<SpellList>");
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

/* method: new_local of class  Pixy::ELuaExporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellListExporter_new00_local
static int tolua_EServer_Pixy_SpellListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<SpellList>* tolua_ret = (Pixy::ELuaExporter<SpellList>*)  Mtolua_new((Pixy::ELuaExporter<SpellList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<SpellList>");
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

/* method: delete of class  Pixy::ELuaExporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellListExporter_delete00
static int tolua_EServer_Pixy_SpellListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<SpellList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<SpellList>* self = (Pixy::ELuaExporter<SpellList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<SpellList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_SpellListExporter_export00
static int tolua_EServer_Pixy_SpellListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<SpellList>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<SpellList>* self = (Pixy::ELuaExporter<SpellList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Pixy::ELuaExporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetListExporter_new00
static int tolua_EServer_Pixy_PuppetListExporter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<PuppetList>* tolua_ret = (Pixy::ELuaExporter<PuppetList>*)  Mtolua_new((Pixy::ELuaExporter<PuppetList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<PuppetList>");
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

/* method: new_local of class  Pixy::ELuaExporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetListExporter_new00_local
static int tolua_EServer_Pixy_PuppetListExporter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Pixy::ELuaExporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Pixy::ELuaExporter<PuppetList>* tolua_ret = (Pixy::ELuaExporter<PuppetList>*)  Mtolua_new((Pixy::ELuaExporter<PuppetList>)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Pixy::ELuaExporter<PuppetList>");
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

/* method: delete of class  Pixy::ELuaExporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetListExporter_delete00
static int tolua_EServer_Pixy_PuppetListExporter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<PuppetList>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Pixy::ELuaExporter<PuppetList>* self = (Pixy::ELuaExporter<PuppetList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: __export of class  Pixy::ELuaExporter<PuppetList> */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_PuppetListExporter_export00
static int tolua_EServer_Pixy_PuppetListExporter_export00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::ELuaExporter<PuppetList>",0,&tolua_err) ||
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
  Pixy::ELuaExporter<PuppetList>* self = (Pixy::ELuaExporter<PuppetList>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: instance::lua_log of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_Log00
static int tolua_EServer_Pixy_Net_instance_Log00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_broadcast00
static int tolua_EServer_Pixy_Net_instance_broadcast00(lua_State* tolua_S)
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
  const Pixy::Event* tolua_var_1 = ((const Pixy::Event*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'broadcast'", NULL);
#endif
  {
   self->broadcast(*tolua_var_1);
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_send00
static int tolua_EServer_Pixy_Net_instance_send00(lua_State* tolua_S)
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
  const Pixy::Event* tolua_var_2 = ((const Pixy::Event*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send'", NULL);
#endif
  {
   self->send(inPuppetUID,*tolua_var_2);
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_create_unit00
static int tolua_EServer_Pixy_Net_instance_create_unit00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_destroy_unit00
static int tolua_EServer_Pixy_Net_instance_destroy_unit00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_destroy_puppet00
static int tolua_EServer_Pixy_Net_instance_destroy_puppet00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_draw_spells00
static int tolua_EServer_Pixy_Net_instance_draw_spells00(lua_State* tolua_S)
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
  int tolua_var_3 = ((int)  tolua_tonumber(tolua_S,2,0));
  int tolua_var_4 = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_draw_spells'", NULL);
#endif
  {
   self->_draw_spells(tolua_var_3,tolua_var_4);
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

/* Open function */
TOLUA_API int tolua_EServer_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Pixy",0);
  tolua_beginmodule(tolua_S,"Pixy");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UnitVecExporter","Pixy::ELuaExporter<UnitVec>","LuaExporter",tolua_collect_Pixy__ELuaExporter_UnitVec_);
   #else
   tolua_cclass(tolua_S,"UnitVecExporter","Pixy::ELuaExporter<UnitVec>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UnitVecExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_UnitVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_UnitVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_UnitVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_UnitVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_UnitVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SpellVecExporter","Pixy::ELuaExporter<SpellVec>","LuaExporter",tolua_collect_Pixy__ELuaExporter_SpellVec_);
   #else
   tolua_cclass(tolua_S,"SpellVecExporter","Pixy::ELuaExporter<SpellVec>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SpellVecExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_SpellVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_SpellVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_SpellVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_SpellVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_SpellVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PuppetVecExporter","Pixy::ELuaExporter<PuppetVec>","LuaExporter",tolua_collect_Pixy__ELuaExporter_PuppetVec_);
   #else
   tolua_cclass(tolua_S,"PuppetVecExporter","Pixy::ELuaExporter<PuppetVec>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PuppetVecExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_PuppetVecExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_PuppetVecExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_PuppetVecExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_PuppetVecExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_PuppetVecExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UnitListExporter","Pixy::ELuaExporter<UnitList>","LuaExporter",tolua_collect_Pixy__ELuaExporter_UnitList_);
   #else
   tolua_cclass(tolua_S,"UnitListExporter","Pixy::ELuaExporter<UnitList>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UnitListExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_UnitListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_UnitListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_UnitListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_UnitListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_UnitListExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SpellListExporter","Pixy::ELuaExporter<SpellList>","LuaExporter",tolua_collect_Pixy__ELuaExporter_SpellList_);
   #else
   tolua_cclass(tolua_S,"SpellListExporter","Pixy::ELuaExporter<SpellList>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SpellListExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_SpellListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_SpellListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_SpellListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_SpellListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_SpellListExporter_export00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PuppetListExporter","Pixy::ELuaExporter<PuppetList>","LuaExporter",tolua_collect_Pixy__ELuaExporter_PuppetList_);
   #else
   tolua_cclass(tolua_S,"PuppetListExporter","Pixy::ELuaExporter<PuppetList>","LuaExporter",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PuppetListExporter");
    tolua_function(tolua_S,"new",tolua_EServer_Pixy_PuppetListExporter_new00);
    tolua_function(tolua_S,"new_local",tolua_EServer_Pixy_PuppetListExporter_new00_local);
    tolua_function(tolua_S,".call",tolua_EServer_Pixy_PuppetListExporter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EServer_Pixy_PuppetListExporter_delete00);
    tolua_function(tolua_S,"export",tolua_EServer_Pixy_PuppetListExporter_export00);
   tolua_endmodule(tolua_S);
   tolua_module(tolua_S,"Net",0);
   tolua_beginmodule(tolua_S,"Net");
    tolua_cclass(tolua_S,"instance","Pixy::Net::instance","",NULL);
    tolua_beginmodule(tolua_S,"instance");
     tolua_function(tolua_S,"Log",tolua_EServer_Pixy_Net_instance_Log00);
     tolua_function(tolua_S,"broadcast",tolua_EServer_Pixy_Net_instance_broadcast00);
     tolua_function(tolua_S,"send",tolua_EServer_Pixy_Net_instance_send00);
     tolua_function(tolua_S,"create_unit",tolua_EServer_Pixy_Net_instance_create_unit00);
     tolua_function(tolua_S,"destroy_unit",tolua_EServer_Pixy_Net_instance_destroy_unit00);
     tolua_function(tolua_S,"destroy_puppet",tolua_EServer_Pixy_Net_instance_destroy_puppet00);
     tolua_function(tolua_S,"draw_spells",tolua_EServer_Pixy_Net_instance_draw_spells00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EServer (lua_State* tolua_S) {
 return tolua_EServer_open(tolua_S);
};
#endif

