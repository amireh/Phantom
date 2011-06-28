#include "message_parser.hpp"
#include "message_handler.hpp"

namespace Pixy {
namespace Net {

message_parser::message_parser() {
}

bool message_parser::parse_header(message& msg,  boost::asio::streambuf& in) {

  if (in.size() < message::header_length) {
    in.consume(in.size());
    return false;
  }

  std::cout << "received " << in.size() << "bytes of data\n";

  char sp1;
  unsigned char msg_id;
  char msg_length[4];
  std::istream is(&in);
  is.unsetf(std::ios_base::skipws);
  is >> msg_id >> sp1 >> msg_length >> sp1;

  msg.id = (message_id)msg_id;
  msg.length = atoi(msg_length);

  std::cout<< "msg id=" << (unsigned char)msg.id << ", length=" << msg.length << "(" << msg_length << ")\n";
  //assert(msg.id > message_id::unassigned && msg.id < message_id::placeholder);
  //assert(atoi(msg_length) <= message::max_length);

  if (msg.id > message_id::unassigned && msg.id < message_id::placeholder && msg.length <= message::max_length)
    return true;

  return false;
}

bool message_parser::parse_body(message& msg,  boost::asio::streambuf& in) {
  int bytes_received = in.size();
  int bytes_left = msg.length - msg.body.size() - bytes_received;
  int bytes_to_process = (bytes_received > msg.length) ? msg.length : bytes_received;

  // read until either the message length is reached or the buffer is empty
  std::istream is(&in);
  while (msg.body.size() != msg.length || in.size() != 0)
    msg.body.push_back(is.get());

  //std::cout << "body now is " << msg.body.size() << "bytes long and has " << msg.body << "\n";
  return bytes_left <= 0;
}

void message_parser::dump(message const& msg, boost::asio::streambuf& out) {
  std::ostream stream(&out);
  stream << (unsigned char)msg.id << " ";
  stream << (uint16_t)msg.body.size() << " ";
  if (!msg.body.empty()) // a message might not have a body
    stream << msg.body;

}

}
}
