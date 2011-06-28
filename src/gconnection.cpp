#include "gconnection.hpp"
#include "server.hpp"

namespace http {
namespace server3 {

gconnection::gconnection(boost::asio::io_service& io_service)
  : connection(io_service, message::gmax_length)
{
}

gconnection::~gconnection() {
}

void gconnection::shutdown() {
  connection::shutdown();
  // tell the server to remove us to tear this object down
  strand_.post(
    boost::bind(
      static_cast<void (server::*)( gconnection_ptr )>(&server::gclose),
      &server::singleton(),
      boost::shared_ptr<gconnection>(this)));
}

} // namespace server3
} // namespace http
