#include "message_parser.hpp"
#include "message_handler.hpp"

message_parser::message_parser() {
}

bool message_parser::parse_header(message& msg,  boost::asio::streambuf& in) {

  if (in.size() < message::header_length) {
    in.consume(in.size());
    return false;
  }

  char sp1;
  unsigned char msg_id;
  char msg_length[4];
  std::istream is(&in);
  is.unsetf(std::ios_base::skipws);
  is >> msg_id >> sp1 >> msg_length;

  msg.id = (message_id)msg_id;
  msg.length = atoi(msg_length);

  std::cout<< "msg id=" << (unsigned char)msg.id << ", length=" << msg.length << "\n";
  //assert(msg.id > message_id::unassigned && msg.id < message_id::placeholder);
  //assert(atoi(msg_length) <= message::max_length);

  if (msg.id > message_id::unassigned && msg.id < message_id::placeholder && msg.length <= message::max_length)
    return true;

  return false;
}

bool message_parser::parse_body(message& msg,  boost::asio::streambuf& in) {
  int bytes_received = in.size();
  int bytes_left = msg.length - msg.body.size() - bytes_received;

  char c;
  std::istream is(&in);
  // read until either the message length is reached or the buffer is empty
  while (!msg.body.size() == msg.length || !in.size() == 0)
    msg.body.push_back(is.get());

  return bytes_left > 0;
}

void message_parser::dump(message const& msg, boost::asio::streambuf& out) {
  //out.consume(out.size());

  std::ostream stream(&out);
  //boost::asio::mutable_buffer stream(&out);
  stream << (unsigned char)msg.id << "\n";
  stream << (uint16_t)msg.length;
  if (!msg.body.empty()) // a message might not have a body
    stream << msg.body;
  stream << message::footer;

}
