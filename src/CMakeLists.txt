# add sources
SET(Project_SRCS
  base_connection.cpp
  server_connection.cpp
  server.cpp
  #message_parser.cpp
  message_handler.cpp
  server_main.cpp
  event.cpp
  db_manager.cpp
  player.cpp
  sresource_manager.cpp
  #instance.cpp
)
# the executable
ADD_EXECUTABLE(Server ${Project_SRCS})

# add sources
SET(Project_SRCS
  client.cpp
  base_connection.cpp
  client_connection.cpp
  event.cpp
  #message_parser.cpp
  message_handler.cpp
  client_main.cpp
)
# the executable
ADD_EXECUTABLE(Client ${Project_SRCS})

