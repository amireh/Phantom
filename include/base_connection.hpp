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

#ifndef H_PixyNet_BaseServerConnection_H
#define H_PixyNet_BaseServerConnection_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "dispatcher.hpp"
#include "Event.hpp"

namespace Pixy {
namespace Net {

  /// Represents a single base_connection from a client.
  class base_connection
    : public boost::enable_shared_from_this<base_connection>,
      protected boost::noncopyable
  {
  public:
    /// Construct a base_connection with the given io_service.
    explicit base_connection(boost::asio::io_service& io_service);
    virtual ~base_connection();

    /// Get the socket associated with the base_connection.
    boost::asio::ip::tcp::socket& socket();
    dispatcher& get_dispatcher();

    /// Start the first asynchronous operation for the base_connection.
    virtual void start();
    /// forcefully breaks all async ops and closes the socket
    virtual void stop();

    virtual void send(const Event& evt);

  protected:

    virtual void read();
    virtual void do_read();

    void
    handle_read( const boost::system::error_code& error, std::size_t bytes_transferred);

    virtual void handle_inbound();
    virtual void do_send(Event&);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::mutable_buffer body_;
    boost::asio::streambuf response_;
    boost::asio::strand strand_;

    dispatcher dispatcher_;
    Event outbound, inbound;

    bool closed_;
  };

  typedef boost::shared_ptr<base_connection> base_connection_ptr;

} // namespace Net
} // namespace Pixy

#endif
