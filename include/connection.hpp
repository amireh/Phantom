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

namespace Pixy {
namespace Net {

  /// Represents a single connection from a client.
  class connection
    : public boost::enable_shared_from_this<connection>,
      protected boost::noncopyable
  {
  public:
    /// Construct a connection with the given io_service.
    explicit connection(boost::asio::io_service& io_service);
    virtual ~connection();

    /// Get the socket associated with the connection.
    boost::asio::ip::tcp::socket& socket();
    message_handler& get_message_handler();

    /// Start the first asynchronous operation for the connection.
    virtual void start();
    /// forcefully breaks all async ops and closes the socket
    virtual void stop();

    virtual void send(const message& msg);

  protected:

    void on_pong(const message& msg);
    void on_foo(const message& msg);
    void on_disconnect(const message& msg);

    virtual void read();
    virtual void read_body();
    //void do_read();

    virtual void
    handle_read_header(
      const boost::system::error_code& error,
      std::size_t bytes_transferred);
    virtual void
    handle_read_body(
      const boost::system::error_code& error,
      std::size_t bytes_transferred);

    virtual void handle_write(const boost::system::error_code& e);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::mutable_buffer body_;
    boost::asio::streambuf response_;
    boost::asio::strand strand_;

    message_parser message_parser_;
    message_handler message_handler_;
    message msg_, outbound;
  };

  typedef boost::shared_ptr<connection> connection_ptr;

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
