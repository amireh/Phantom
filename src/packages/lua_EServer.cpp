/*
** Lua binding: EServer
** Generated automatically by tolua++-1.0.92 on Fri Jul  1 18:32:17 2011.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EServer_open (lua_State* tolua_S);

#include "required.h"
#include "player.hpp"
#include "instance.hpp"
#include "player.hpp"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Pixy::Net::player");
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
    tolua_cclass(tolua_S,"player","Pixy::Net::player","",NULL);
    tolua_beginmodule(tolua_S,"player");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"instance","Pixy::Net::instance","",NULL);
    tolua_beginmodule(tolua_S,"instance");
     tolua_function(tolua_S,"Log",tolua_EServer_Pixy_Net_instance_Log00);
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

