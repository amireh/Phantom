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
}

base_connection::~base_connection() {
  //std::cout << "A base_connection has been destroyed\n";
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
  strand_.post( boost::bind(&base_connection::do_read, shared_from_this() ) );
}

void base_connection::do_read() {
  std::cout << "reading a message\n";
  msg_.reset();
  std::cout << "inbound buffer has " << request_.size() << "bytes (expected 0), ";
  request_.consume(request_.size());
  std::cout << "and now: " << request_.size() << "\n";

  async_read_until(
    socket_,
    request_,
    message::footer,
    boost::bind(
      &base_connection::handle_read_all,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );

#if 0 // __DISABLED__
  /*async_read(
    socket_,
    request_,
    boost::asio::transfer_at_least(message::header_length),
    boost::bind(
      &base_connection::handle_read_header,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );*/
#endif

}

#if 0 // __DISABLED__
void base_connection::read_body() {
  //std::cout << "reading a message's body\n";
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

      stop();
    }

  } else
    // If an error occurs then no new asynchronous operations are started. This
    // means that all shared_ptr references to the base_connection object will
    // disappear and the object will be destroyed automatically after this
    // handler returns. The base_connection class's destructor closes the socket.

    stop();
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

  } else
    stop();
}
#endif // __DISABLED__

void base_connection::handle_read_all(
  const boost::system::error_code& e,
  std::size_t bytes_transferred)
{
  if (!e) {
    bool result = message_parser_.parse_all(msg_, request_);
    if (result) {
      request_.consume(bytes_transferred);
      // message is ready for dispatching
      message_handler_.deliver(msg_);

      // read next message
      strand_.post( boost::bind(&base_connection::read, shared_from_this()) );
    } else {
      stop();
    }

  } else
    stop();
}

void base_connection::send(const message& msg) {
  strand_.post( boost::bind(&base_connection::do_send, shared_from_this(), msg));
}

void base_connection::do_send(const message& msg) {

  std::cout << "outbound buffer has " << response_.size() << "bytes (expected 0)";

  boost::system::error_code ec;

  outbound = message(msg);
  message_parser_.dump(msg, response_);
  size_t n = boost::asio::write(socket_, response_.data(), boost::asio::transfer_all(), ec);

  if (!ec) {
    std::cout << " sent " << n << "bytes";
    response_.consume(n);
    std::cout << " cleared response buf ( " << response_.size() << ")\n";
  } else
    stop();

#if 0 // __DISABLED__
  /*boost::asio::async_write(socket_, response_,//outbound.to_buffers(),
  strand_.wrap(
    boost::bind(&base_connection::handle_write, shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred)));*/
#endif

}

#if 0 // __DISABLED__
void base_connection::handle_write(const boost::system::error_code& e, std::size_t bytes_transferred)
{
  if (e)
  {
    stop();
  } else {
    std::cout << "clearing outbound buffer which had " << response_.size() << ", it said it xferred " << bytes_transferred <<" and post-clearing:";
    response_.consume(bytes_transferred);
    std::cout << response_.size() << "\n";
  }
}
#endif // __DISABLED__

} // namespace server3
} // namespace http
