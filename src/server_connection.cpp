#include "server_connection.hpp"

namespace Pixy {
namespace Net {

  connection::connection(boost::asio::io_service& io_service)
    : base_connection(io_service)
  {
  }

  connection::~connection() {
    std::cout << "A server connection has been destroyed\n";
  }

} // namespace server3
} // namespace http
