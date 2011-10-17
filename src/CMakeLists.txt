# add sources
SET(Project_SRCS
  #Entity.cpp
  #Puppet.cpp
  #Unit.cpp
  instance.cpp

  base_connection.cpp
  server_connection.cpp
  server.cpp
  server_main.cpp
  db_manager.cpp
  player.cpp
  sresource_manager.cpp
  match_finder.cpp
  lobby.cpp
  room.cpp

)


# generate lua bindings

SET(LuaProject "${PROJECT_NAME}Lua")
SET(LuaGeneratedSrc "lua_${PROJECT_NAME}.cpp")
SET(LuaGeneratedSrcPathRel "packages/${LuaGeneratedSrc}")
SET(LuaGeneratedSrcPath "${CMAKE_CURRENT_SOURCE_DIR}/${LuaGeneratedSrcPathRel}")

ADD_CUSTOM_COMMAND(
  OUTPUT ${LuaGeneratedSrcPathRel}
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/packages/parser.sh ${CMAKE_CURRENT_SOURCE_DIR}/packages
  "Generating Phantom lua bindings")
LIST(APPEND Project_SRCS ${LuaGeneratedSrcPath})

# the executable
ADD_EXECUTABLE(${PROJECT_NAME} ${Project_SRCS})
SET_SOURCE_FILES_PROPERTIES(${LuaGeneratedSrc} PROPERTIES GENERATED 1)

ADD_CUSTOM_TARGET(${LuaProject} DEPENDS ${LuaGeneratedSrcPathRel})

IF(NOT EXISTS ${LuaGeneratedSrcPath})
  MESSAGE(STATUS "Lua bindings are not yet generated, forcing generation. Generate manually using make ${LuaProject}")
  ADD_DEPENDENCIES(${PROJECT_NAME} ${LuaProject})
ENDIF()

ADD_SUBDIRECTORY(bot)
