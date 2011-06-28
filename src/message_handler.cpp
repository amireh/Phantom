#include "message_handler.hpp"
message_handler::message_handler(boost::asio::io_service& io_service, tcp::socket& in_socket)
: socket_(&in_socket),
  strand_(io_service) {
}

message_handler::~message_handler() {
  socket_ = 0;
  messages.clear();
}

void message_handler::send(const message &msg, boost::asio::streambuf& out) {
}

void message_handler::recv(boost::asio::streambuf& in) {

}

void message_handler::notify(const message& msg) {
  messages.push_back(message(msg));

  strand_.post( boost::bind( &message_handler::dispatch, this ) );
}

void message_handler::dispatch() {
  const message& msg = messages.front();

  handlers_t::const_iterator itr = handlers_.find(msg.id);
  if (itr == handlers_.end())
    return;

  for (msg_handler_t handler : itr->second)
    //socket_->get_io_service().post( boost::bind(handler, msg) );
    handler( msg );
    //socket_->get_io_service().post( strand_.wrap( boost::bind(handler, msg) ) );

  messages.pop_front();
}
