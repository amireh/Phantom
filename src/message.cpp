#include "message.hpp"


//const char* message::footer = "\r\n\r\n";

namespace Pixy {
namespace Net {

std::vector<boost::asio::const_buffer> message::to_buffers() {
  std::string me;
  me.push_back((unsigned char)id);
  me.push_back(' ');
  me += body.size();
  me.push_back(' ');
  me += body;

  std::cout << "me in string mode: " << me;
  std::vector<boost::asio::const_buffer> buffers;
  //buffers.push_back(boost::asio::buffer(c));
  //buffers.push_back(boost::asio::buffer((char*)length));
  buffers.push_back(boost::asio::buffer(me));

  return buffers;
}

}
}
