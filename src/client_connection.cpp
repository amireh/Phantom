/*
 *  Copyright (c) 2011 Ahmad Amireh <ahmad@amireh.net>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#include "client_connection.hpp"

namespace Pixy {
namespace Net {

  connection::connection(boost::asio::io_service& io_service, std::string server, std::string port)
    : base_connection(io_service),
      resolver_(io_service),
      host_(server),
      port_(port),
      connected_(false)
  {
    //message_handler_.bind(message_id::ping, this, &connection::on_ping);
    dispatcher_.bind(EventUID::Ping, this, &connection::on_ping);
  }

  connection::~connection() {
    std::cout << "A connection has been destroyed\n";
  }

  bool connection::connect() {
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

  bool connection::is_connected() {
    return connected_;
  }
  void connection::disconnect() {
    send(Event(EventUID::Logout));
  }

  void connection::start() {
    base_connection::start();

  }

  /*void connection::on_ping(const message &msg) {
    std::cout<<"got PINGED!\n";
    send(message(message_id::pong));
  }*/

  void connection::on_ping(const Event& evt) {
    std::cout<<"got PINGED!\n";
    send(Event(EventUID::Pong));
  }

} // namespace server3
} // namespace http
