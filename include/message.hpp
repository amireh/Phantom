#ifndef H_PixyNet_Message_H
#define H_PixyNet_Message_H

#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>

enum class message_id : unsigned char {
  unassigned,
  disconnect,
  bad_request,
  ping,
  pong,
  foo,

  placeholder
};

struct message
{
  message_id id;
  uint16_t length;
  std::string body;
  static const char* footer;

  message(message_id in_id)
  : id(in_id),
    body(0),
    length(0)
  {
  }
  message() {
    reset();
  }
  ~message() {
    reset();
  }

  void reset() {
    id = message_id::unassigned;
    body.clear();
    length = 0;
  }

  enum {
    header_length = sizeof(message_id) + sizeof(uint16_t),
    footer_length = sizeof(char) * 4, // strlen("\r\n\r\n")
    max_length = 65536, // no single message can be longer than this (stored in uint16_t)
    //gmax_length = 512
  };
};

#endif
