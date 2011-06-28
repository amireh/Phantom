#include "server_connection.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

  connection::connection(boost::asio::io_service& io_service)
    : base_connection(io_service),
      pingmsg_(message_id::ping),
      ping_timeouts_(0)
  {
    message_handler_.bind(message_id::pong, this, &connection::on_pong);
    message_handler_.bind(message_id::foo, this, &connection::on_foo);
    message_handler_.bind(message_id::disconnect, this, &connection::on_disconnect);

    message_parser_.dump(pingmsg_, pinger_);
  }

  connection::~connection() {
    std::cout << "A server connection has been destroyed\n";
  }

  void connection::stop() {
    base_connection::stop();

    strand_.post( boost::bind(&server::_close, &server::singleton(), shared_from_this()));
  }

  void connection::ping() {
    try {
      std::cout << "pinging client... ";
      size_t n = socket_.send(pinger_.data());
      std::cout << "sent " << n << "bytes\n";
    } catch (std::exception& e) {
      std::cerr << "ping timeout, client possibly disconnected\n";
      if (++ping_timeouts_ == 3) {
        strand_.post( boost::bind(&connection::stop, shared_from_this()));
      }
    }
  }

  void connection::on_pong(const message &msg) {
    std::cout<<"got PONGED!\n";
    ping_timeouts_ = 0;
  }

  void connection::on_foo(const message &msg) {
    std::cout << "got FOO: " << msg.body << "\n";
  }

  void connection::on_disconnect(const message &msg) {
    std::cout << "client disconnecting\n";
    strand_.post( boost::bind(&connection::stop, shared_from_this()));
  }

} // namespace Net
} // namespace Pixy
