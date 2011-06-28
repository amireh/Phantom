//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <iostream>
#include <fstream>
#include "server.hpp"

namespace Pixy {
namespace Net {

connection::connection(boost::asio::io_service& io_service)
  : strand_(io_service),
    socket_(io_service),
    message_parser_(),
    message_handler_(io_service),
    request_(message::max_length),
    response_(message::max_length)
{
  /*message_handler_.bind(message_id::pong, this, &connection::on_pong);
  message_handler_.bind(message_id::foo, this, &connection::on_foo);
  message_handler_.bind(message_id::disconnect, this, &connection::on_disconnect);*/
}

connection::~connection() {
  std::cout << "A connection has been destroyed\n";
}

boost::asio::ip::tcp::socket& connection::socket() {
  return socket_;
}
message_handler& connection::get_message_handler() {
  return message_handler_;
}

void connection::start() {
  read();
}

void connection::stop() {

  boost::system::error_code ignored_ec;

  // initiate graceful connection closure & stop all asynchronous ops
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  socket_.close(ignored_ec);

  std::cout << "A connection has been closed\n";
}

void connection::read() {
  std::cout << "reading a message\n";
  msg_.reset();
  async_read(
    socket_,
    request_,
    boost::asio::transfer_at_least(message::header_length),
    boost::bind(
      &connection::handle_read_header,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}


void connection::read_body() {
  std::cout << "reading a message's body\n";
  socket_.async_read_some(boost::asio::buffer(body_, msg_.length - msg_.body.size()),
    strand_.wrap(
      boost::bind(&connection::handle_read_body, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred)));
}


void connection::handle_read_header(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e)
  {
    bool result = message_parser_.parse_header(msg_, request_);

    if (result)
    {
      // message has a valid header
      if (msg_.length != 0)
        // let's read the body
        read_body();
      else {
        // it's a content-less msg, dispatch and read the next
        // ...
        message_handler_.deliver(msg_);
        read();
      }
    }
    else
    {
      // invalid request
      /*reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));*/
    }

  }

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}

void connection::handle_read_body(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e) {
    bool result = message_parser_.parse_body(msg_, request_);
    if (result) {
      // message is ready for dispatching
      message_handler_.deliver(msg_);

      // read next message
      read();
    } else {
      // need more data
      read_body();
    }

  }
}

void connection::send(const message& msg) {

  outbound = message(msg);
  message_parser_.dump(msg, response_);
  boost::asio::async_write(socket_, response_,//outbound.to_buffers(),
  strand_.wrap(
    boost::bind(&connection::handle_write, shared_from_this(),
      boost::asio::placeholders::error)));

}

void connection::handle_write(const boost::system::error_code& e)
{
  if (e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}

void connection::on_pong(const message &msg) {
  std::cout<<"got PONGED!\n";
}

void connection::on_foo(const message &msg) {
  std::cout << "got FOO: " << msg.body << "\n";
}

void connection::on_disconnect(const message &msg) {
  std::cout << "client disconnecting\n";
  strand_.post( boost::bind(&connection::stop, shared_from_this()));
}
} // namespace server3
} // namespace http
