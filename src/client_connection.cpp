#include "client_connection.hpp"

namespace Pixy {
namespace Net {

  client_connection::client_connection(boost::asio::io_service& io_service, std::string server, std::string port)
    : connection(io_service),
      resolver_(io_service),
      host_(server),
      port_(port),
      connected_(false)
  {
  }

  client_connection::~client_connection() {
    std::cout << "A client_connection has been destroyed\n";
  }

  bool client_connection::connect() {
    if (connected_) {
      std::cerr << "already connected!\n";
      return true;
    }

    tcp::resolver::query query(host_, port_);

    // Start a synchronous resolve
    boost::system::error_code ec;
    tcp::resolver::iterator endpoint = resolver_.resolve(query);
    socket_.connect(*endpoint, ec);

    if (ec) {
      std::cerr << "couldn't connect to server, aborting\n";
      return false;
    }

    connected_ = true;

    // set KEEPALIVE to true
    socket_.set_option(boost::asio::socket_base::keep_alive(true));
    std::cout << "connected!\n";

    return connected_;
  }

  bool client_connection::is_connected() {
    return connected_;
  }
  void client_connection::disconnect() {
    send(message(message_id::disconnect));
  }

  void client_connection::start() {
    connection::start();
    message foo(message_id::foo);
    foo.body = "HAI";
    send(foo);
  }
} // namespace server3
} // namespace http
