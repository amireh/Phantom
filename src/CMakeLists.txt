# add sources
SET(Project_SRCS
  connection.cpp
  #gconnection.cpp
  #pconnection.cpp
  server.cpp
  message.cpp
  message_parser.cpp
  message_handler.cpp
  server_main.cpp
)
# the executable
ADD_EXECUTABLE(Server ${Project_SRCS})

# add sources
SET(Project_SRCS
  client.cpp
  message.cpp
  message_parser.cpp
  message_handler.cpp
  client_main.cpp
)
# the executable
ADD_EXECUTABLE(Client ${Project_SRCS})

