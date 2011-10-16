# add sources
INCLUDE_DIRECTORIES(include)
SET(Bot_SRCS
  src/bot.cpp
  src/base_connection.cpp
  src/bot_connection.cpp
  src/bot_main.cpp
  src/bresource_manager.cpp
  ../sresource_manager.cpp
  ../Puppet.cpp
  ../Unit.cpp
  ../Entity.cpp
)
# the executable
ADD_EXECUTABLE(Bot ${Bot_SRCS})
