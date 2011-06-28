#include "pconnection.hpp"
#include "server.hpp"

namespace http {
namespace server3 {

pconnection::pconnection(boost::asio::io_service& io_service)
  : connection(io_service, message::pmax_length),
    ping_timeouts(0)
{
  message_handler_.bind(message_id::pong, this, &pconnection::on_pong);
  message_handler_.bind(message_id::foo, this, &pconnection::on_foo);
}

pconnection::~pconnection() {
}

void pconnection::shutdown() {
  //connection::shutdown();
  // tell the server to remove us to tear this object down
  //socket_.get_io_service().post(
  strand_.post(
    boost::bind(
      &server::pclose,
      &server::singleton(),
      shared_from_this()));
}

void pconnection::ping() {
  try {
    std::cout << "pinging client...\n";
    message ping(message_id::ping);
    message_handler_.send(ping, response_);

  } catch (std::exception& e) {
    std::cerr << "ping timeout, client possibly disconnected\n";
    if (++ping_timeouts == 3) {
      strand_.dispatch( boost::bind(&pconnection::shutdown, shared_from_this()) );
    }
  }
}

void pconnection::on_pong(boost::shared_ptr<message> msg) {
  std::cout<<"got PONGED!\n";
  ping_timeouts = 0;
}

void pconnection::on_foo(boost::shared_ptr<message> msg) {
  std::cout << "got FOO\n";
}
} // namespace server3
} // namespace http
