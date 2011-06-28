//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_CONNECTION_HPP
#define HTTP_SERVER3_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "message.hpp"
#include "message_parser.hpp"
#include "message_handler.hpp"

namespace http {
namespace server3 {

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service& io_service);
  virtual ~connection();

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

  void ping();

private:
  friend class server;

  // calls stop() and server::close(connection*) to remove us from tracker
  void shutdown();

  // forcefully breaks all async ops and closes the socket
  void stop();

  void on_pong(boost::shared_ptr<message> msg);
  void on_foo(boost::shared_ptr<message> msg);
  void on_disconnect(boost::shared_ptr<message> msg);

  void read();
  void do_read();
  void handle_read(
    const boost::system::error_code& e,
    std::size_t bytes_transferred);
  void handle_read2(
    const boost::system::error_code& e,
    std::size_t bytes_transferred);

  //void handle_write(const boost::system::error_code& e);
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
  boost::asio::strand strand_;

  message_parser message_parser_;
  message_handler message_handler_;

  int ping_timeouts; // if client doesnt respond to 3 ping requests, the connection is closed
  bool connected;
};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
