#include "message_handler.hpp"
message_handler::message_handler(tcp::socket& in_socket, boost::asio::strand& in_strand, message_parser& in_parser)
: socket_(&in_socket),
  parser_(in_parser),
  strand_(in_strand) {

}

message_handler::~message_handler() {
  socket_ = 0;
}

void message_handler::send(const message &msg, boost::asio::streambuf& out) {
}

void message_handler::recv(boost::asio::streambuf& in) {

}

void message_handler::notify(boost::shared_ptr<message> msg) {
  handlers_t::const_iterator itr = handlers_.find(msg->id);
  if (itr == handlers_.end())
    return;

  for (msg_handler_t handler : itr->second)
    //socket_->get_io_service().post( boost::bind(handler, msg) );
    strand_.post( boost::bind(handler, msg) );
    //socket_->get_io_service().post( strand_.wrap( boost::bind(handler, msg) ) );
}
