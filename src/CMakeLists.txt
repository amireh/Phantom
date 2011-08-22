# add sources
SET(Project_SRCS
  #base_connection.cpp
  server_connection.cpp
  server.cpp
  #message_parser.cpp
  #dispatcher.cpp
  server_main.cpp
  #event.cpp
  db_manager.cpp
  player.cpp
  sresource_manager.cpp
  match_finder.cpp
  instance.cpp
)


# generate lua bindings
ADD_CUSTOM_COMMAND(
  OUTPUT packages/lua_EServer.cpp
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/packages/parser.sh ${CMAKE_CURRENT_SOURCE_DIR}/packages
  "Generating lua bindings"
)

# the executable
ADD_EXECUTABLE(Server ${Project_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/packages/lua_EServer.cpp)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/packages/lua_EServer.cpp PROPERTIES GENERATED 1)

ADD_CUSTOM_TARGET(EServerLua DEPENDS packages/lua_EServer.cpp )
IF(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/packages/lua_EServer.cpp")
  MESSAGE(STATUS "Lua bindings is not yet generated, forcing generation. Generate manually using make EServerLua")
  ADD_DEPENDENCIES(${PROJECT_NAME} EServerLua)
ENDIF()
