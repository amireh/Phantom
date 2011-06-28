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

#include "base_connection.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

base_connection::base_connection(boost::asio::io_service& io_service)
  : strand_(io_service),
    socket_(io_service),
    message_parser_(),
    message_handler_(io_service),
    request_(message::max_length),
    response_(message::max_length)
{
  /*message_handler_.bind(message_id::pong, this, &base_connection::on_pong);
  message_handler_.bind(message_id::foo, this, &base_connection::on_foo);
  message_handler_.bind(message_id::disconnect, this, &base_connection::on_disconnect);*/
}

base_connection::~base_connection() {
  std::cout << "A base_connection has been destroyed\n";
}

boost::asio::ip::tcp::socket& base_connection::socket() {
  return socket_;
}
message_handler& base_connection::get_message_handler() {
  return message_handler_;
}

void base_connection::start() {
  read();
}

void base_connection::stop() {

  boost::system::error_code ignored_ec;

  // initiate graceful base_connection closure & stop all asynchronous ops
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  socket_.close(ignored_ec);

  std::cout << "Connection: closed\n";
}

void base_connection::read() {
  std::cout << "reading a message\n";
  msg_.reset();
  async_read(
    socket_,
    request_,
    boost::asio::transfer_at_least(message::header_length),
    boost::bind(
      &base_connection::handle_read_header,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}


void base_connection::read_body() {
  std::cout << "reading a message's body\n";
  socket_.async_read_some(boost::asio::buffer(body_, msg_.length - msg_.body.size()),
    strand_.wrap(
      boost::bind(&base_connection::handle_read_body, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred)));
}


void base_connection::handle_read_header(const boost::system::error_code& e,
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
            boost::bind(&base_connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));*/
    }

  }

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the base_connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The base_connection class's destructor closes the socket.
}

void base_connection::handle_read_body(const boost::system::error_code& e,
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

void base_connection::send(const message& msg) {

  outbound = message(msg);
  message_parser_.dump(msg, response_);
  boost::asio::async_write(socket_, response_,//outbound.to_buffers(),
  strand_.wrap(
    boost::bind(&base_connection::handle_write, shared_from_this(),
      boost::asio::placeholders::error)));

}

void base_connection::handle_write(const boost::system::error_code& e)
{
  if (e)
  {
    // Initiate graceful base_connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}

void base_connection::on_pong(const message &msg) {
  std::cout<<"got PONGED!\n";
}

void base_connection::on_foo(const message &msg) {
  std::cout << "got FOO: " << msg.body << "\n";
}

void base_connection::on_disconnect(const message &msg) {
  std::cout << "client disconnecting\n";
  strand_.post( boost::bind(&base_connection::stop, shared_from_this()));
}
} // namespace server3
} // namespace http
