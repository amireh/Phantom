/*
** Lua binding: EServer
** Generated automatically by tolua++-1.0.92 on Sun Jul  3 19:47:08 2011.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EServer_open (lua_State* tolua_S);

#include "required.h"
#include "instance.hpp"
#include "player.hpp"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Pixy::Unit");
 tolua_usertype(tolua_S,"Pixy::Event");
 tolua_usertype(tolua_S,"Pixy::Puppet");
 tolua_usertype(tolua_S,"Pixy::Net::instance");
}

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
   Pixy::Unit& tolua_ret = (Pixy::Unit&)  self->_create_unit(model,*owner);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Pixy::Unit");
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

/* method: _destroy_unit of class  Pixy::Net::instance */
#ifndef TOLUA_DISABLE_tolua_EServer_Pixy_Net_instance_destroy_unit01
static int tolua_EServer_Pixy_Net_instance_destroy_unit01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Pixy::Net::instance",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Pixy::Unit",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Pixy::Net::instance* self = (Pixy::Net::instance*)  tolua_tousertype(tolua_S,1,0);
  Pixy::Unit* tolua_var_3 = ((Pixy::Unit*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_destroy_unit'", NULL);
#endif
  {
   self->_destroy_unit(*tolua_var_3);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EServer_Pixy_Net_instance_destroy_unit00(tolua_S);
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
   tolua_module(tolua_S,"Net",0);
   tolua_beginmodule(tolua_S,"Net");
    tolua_cclass(tolua_S,"instance","Pixy::Net::instance","",NULL);
    tolua_beginmodule(tolua_S,"instance");
     tolua_function(tolua_S,"Log",tolua_EServer_Pixy_Net_instance_Log00);
     tolua_function(tolua_S,"broadcast",tolua_EServer_Pixy_Net_instance_broadcast00);
     tolua_function(tolua_S,"send",tolua_EServer_Pixy_Net_instance_send00);
     tolua_function(tolua_S,"create_unit",tolua_EServer_Pixy_Net_instance_create_unit00);
     tolua_function(tolua_S,"destroy_unit",tolua_EServer_Pixy_Net_instance_destroy_unit00);
     tolua_function(tolua_S,"destroy_unit",tolua_EServer_Pixy_Net_instance_destroy_unit01);
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

