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

namespace http {
namespace server3 {

connection::connection(boost::asio::io_service& io_service)
  : strand_(io_service),
    socket_(io_service),
    message_parser_(),
    message_handler_(socket_, strand_, message_parser_),
    request_(512),
    response_(512),
    ping_timeouts(0)
{
  message_handler_.bind(message_id::pong, this, &connection::on_pong);
  message_handler_.bind(message_id::foo, this, &connection::on_foo);
  message_handler_.bind(message_id::disconnect, this, &connection::on_disconnect);
}

connection::~connection() {
  //std::cout << "A connection has been destroyed\n";
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start()
{
  //std::cout << "A connection has been accepted\n";
  connected = true;
  read();
}
void connection::stop() {

  boost::system::error_code ignored_ec;

  // initiate graceful connection closure & stop all asynchronous ops
  //socket_.cancel(ignored_ec);
  //socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  socket_.close(ignored_ec);

  //std::cout << "A connection has been closed\n";
}

void connection::read() {
  if (!connected)
    return;
  strand_.post( boost::bind(&connection::do_read, shared_from_this()) );
}

void connection::do_read() {
  if (!connected)
    return;

  /*async_read_until(
    socket_,
    request_,
    message::footer,
    //strand_.wrap(
      boost::bind(
        &connection::handle_read,
        shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred
      )
    //)
  );*/

  async_read(
    socket_,
    request_,
    boost::asio::transfer_at_least(message::header_length),
    boost::bind(
      &connection::handle_read2,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!connected)
    return;

  if (!e && bytes_transferred != 0) {
    // no errors, handle request
    message_handler_.recv(request_);
  } else if (e == boost::asio::error::no_memory || e == boost::asio::error::not_found) {
    // bad request: discard the current request and free up the stream
    //std::cout << "request stream has " << request_.size() << "bytes\n";
    request_.consume(request_.size());
    //std::cout << "request stream now has " << request_.size() << "bytes\n";

  } else if (e != boost::asio::error::eof) {
    // we cannot recover from this, tear down the connection
    std::cerr << "an error occured: " << e.message() << "\n";
    //if (connected)
    return strand_.post( boost::bind(&connection::shutdown, shared_from_this()));
  }

  // wait for the next request
  do_read();
  /*
  if (!e)
  {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      request_handler_.handle_request(request_, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else
    {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }
  }*/

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}


void connection::handle_read2(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!connected)
    return;

  if (!e && bytes_transferred != 0) {
    // no errors, handle request
    try {
      message_handler_.recv(request_);
    } catch (bad_request& e) {
      message_handler_.send(message(message_id::bad_request), response_);
      return strand_.post( boost::bind(&connection::shutdown, shared_from_this()));
    }
  } else if (e == boost::asio::error::no_memory || e == boost::asio::error::not_found) {
    // bad request: discard the current request and free up the stream
    //std::cout << "request stream has " << request_.size() << "bytes\n";
    request_.consume(request_.size());
    //std::cout << "request stream now has " << request_.size() << "bytes\n";

  } else if (e != boost::asio::error::eof) {
    // we cannot recover from this, tear down the connection
    std::cerr << "an error occured: " << e.message() << "\n";
    //if (connected)
    return strand_.post( boost::bind(&connection::shutdown, shared_from_this()));
  }

  // wait for the next request
  read();
  /*
  if (!e)
  {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      request_handler_.handle_request(request_, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else
    {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }
  }*/

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}

/*void connection::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
  if (e) {
    std::cerr << "an error occured while writing: " << e.message() << "\n";
  }

  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}*/

void connection::shutdown() {
  if (!connected)
    return;

  connected = false;
  // stop all current asynchronous ops and block the socket
  stop();
  // tell the server to remove us to tear this object down
  strand_.post( boost::bind(&server::close, &server::singleton(), shared_from_this()));
}

void connection::ping() {
  try {
    //std::cout << "pinging client...\n";
    message ping(message_id::ping);
    message_handler_.send(ping, response_);

  } catch (std::exception& e) {
    std::cerr << "ping timeout, client possibly disconnected\n";
    if (++ping_timeouts == 3) {
      strand_.post( boost::bind(&connection::shutdown, shared_from_this()));
    }
  }
}

void connection::on_pong(boost::shared_ptr<message> msg) {
  //std::cout<<"got PONGED!\n";
  ping_timeouts = 0;
}

void connection::on_foo(boost::shared_ptr<message> msg) {
  //std::cout << "got FOO\n";
}

void connection::on_disconnect(boost::shared_ptr<message> msg) {
  //std::cout << "client disconnecting\n";
  strand_.post( boost::bind(&connection::shutdown, shared_from_this()));
}
} // namespace server3
} // namespace http
