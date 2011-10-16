# add sources
SET(Project_SRCS
  bot.cpp
  base_connection.cpp
  bot_connection.cpp
  bot_main.cpp
  bresource_manager.cpp
)
# the executable
ADD_EXECUTABLE(Bot ${Project_SRCS})
