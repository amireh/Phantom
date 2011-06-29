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

#ifndef H_PixyNet_BaseConnection_H
#define H_PixyNet_BaseConnection_H

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
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include "message.hpp"
#include "message_parser.hpp"
#include "message_handler.hpp"
#include "event.hpp"

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
    message_handler& get_message_handler();

    /// Start the first asynchronous operation for the base_connection.
    virtual void start();
    /// forcefully breaks all async ops and closes the socket
    virtual void stop();

    virtual void send(const message& msg);
    virtual void send(const Event& evt);

    /// Asynchronously write a data structure to the socket.
    template <typename T, typename Handler>
    void async_write(const T& t, Handler handler)
    {
      // Serialize the data first so we know how large it is.
      std::ostringstream archive_stream;
      boost::archive::text_oarchive archive(archive_stream);
      archive << t;
      outbound_data_ = archive_stream.str();

      // Format the header.
      std::ostringstream header_stream;
      header_stream << std::setw(header_length)
        << std::hex << outbound_data_.size();
      if (!header_stream || header_stream.str().size() != header_length)
      {
        // Something went wrong, inform the caller.
        boost::system::error_code error(boost::asio::error::invalid_argument);
        socket_.io_service().post(boost::bind(handler, error));
        return;
      }
      outbound_header_ = header_stream.str();

      // Write the serialized data to the socket. We use "gather-write" to send
      // both the header and the data in a single write operation.
      std::vector<boost::asio::const_buffer> buffers;
      buffers.push_back(boost::asio::buffer(outbound_header_));
      buffers.push_back(boost::asio::buffer(outbound_data_));
      boost::asio::async_write(socket_, buffers, handler);
    }

    /// Asynchronously read a data structure from the socket.
    template <typename T, typename Handler>
    void async_read(T& t, Handler handler)
    {
      // Issue a read operation to read exactly the number of bytes in a header.
      void (base_connection::*f)(
          const boost::system::error_code&,
          T&, boost::tuple<Handler>)
        = &base_connection::handle_read_header<T, Handler>;
      boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
          boost::bind(f,
            this, boost::asio::placeholders::error, boost::ref(t),
            boost::make_tuple(handler)));
    }

    /// Handle a completed read of a message header. The handler is passed using
    /// a tuple since boost::bind seems to have trouble binding a function object
    /// created using boost::bind as a parameter.
    template <typename T, typename Handler>
    void handle_read_header(const boost::system::error_code& e,
        T& t, boost::tuple<Handler> handler)
    {
      if (e)
      {
        boost::get<0>(handler)(e);
      }
      else
      {
        // Determine the length of the serialized data.
        std::istringstream is(std::string(inbound_header_, header_length));
        std::size_t inbound_data_size = 0;
        if (!(is >> std::hex >> inbound_data_size))
        {
          // Header doesn't seem to be valid. Inform the caller.
          boost::system::error_code error(boost::asio::error::invalid_argument);
          boost::get<0>(handler)(error);
          return;
        }

        // Start an asynchronous call to receive the data.
        inbound_data_.resize(inbound_data_size);
        void (base_connection::*f)(
            const boost::system::error_code&,
            T&, boost::tuple<Handler>)
          = &base_connection::handle_read_data<T, Handler>;
        boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
          boost::bind(f, this,
            boost::asio::placeholders::error, boost::ref(t), handler));
      }
    }

    /// Handle a completed read of message data.
    template <typename T, typename Handler>
    void handle_read_data(const boost::system::error_code& e,
        T& t, boost::tuple<Handler> handler)
    {
      if (e)
      {
        boost::get<0>(handler)(e);
      }
      else
      {
        // Extract the data structure from the data just received.
        try
        {
          std::string archive_data(&inbound_data_[0], inbound_data_.size());
          std::istringstream archive_stream(archive_data);
          boost::archive::text_iarchive archive(archive_stream);
          archive >> t;
        }
        catch (std::exception& e)
        {
          // Unable to decode data.
          boost::system::error_code error(boost::asio::error::invalid_argument);
          boost::get<0>(handler)(error);
          return;
        }

        // Inform caller that data has been received ok.
        boost::get<0>(handler)(e);
      }
    }

  protected:

    virtual void read();
    virtual void do_read();


    virtual void send_msg(const message& msg);
    virtual void send_evt(const Event& msg);

#if 0 // __DISABLED__
    virtual void
    handle_read_header(
      const boost::system::error_code& error,
      std::size_t bytes_transferred);

    virtual void read_body();

    virtual void
    handle_read_body(
      const boost::system::error_code& error,
      std::size_t bytes_transferred);

    virtual void
    handle_read_all(
      const boost::system::error_code& error,
      std::size_t bytes_transferred);
#endif // __DISABLED__
    virtual void handle_write( const boost::system::error_code& e);
    virtual void handle_read( const boost::system::error_code& e);


    boost::asio::ip::tcp::socket socket_;
    //boost::asio::streambuf request_;
    //boost::asio::mutable_buffer body_;
    //boost::asio::streambuf response_;
    boost::asio::strand strand_;

    //message_parser message_parser_;
    message_handler message_handler_;
    //message outbound, inbound;
    Event outbound, inbound;

  /// The size of a fixed length header.
  enum { header_length = 2 };
  /// Holds an outbound header.
  std::string outbound_header_;
  /// Holds the outbound data.
  std::string outbound_data_;
  /// Holds an inbound header.
  char inbound_header_[header_length];
  /// Holds the inbound data.
  std::vector<char> inbound_data_;
  };

  typedef boost::shared_ptr<base_connection> base_connection_ptr;

} // namespace Net
} // namespace Pixy

#endif
