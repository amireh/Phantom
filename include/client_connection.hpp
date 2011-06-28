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
#ifndef H_PixyNet_Connection_H
#define H_PixyNet_Connection_H

#include "base_connection.hpp"

namespace Pixy {
namespace Net {

  /// Represents a single connection from a client.
  class connection : public base_connection
  {

  public:
    /// Construct a connection with the given io_service.
    explicit connection(boost::asio::io_service&, std::string host, std::string port);
    virtual ~connection();

    bool connect();
    bool is_connected();
    void disconnect();

    virtual boost::shared_ptr<connection> shared_from_this() {
      return boost::static_pointer_cast<connection>(base_connection::shared_from_this());
    }

    virtual void start();

  protected:
    void on_ping(const message &);

    tcp::resolver resolver_;
    std::string host_, port_;
    bool connected_;

    void handle_resolve(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);

  };

  typedef boost::shared_ptr<connection> connection_ptr;

} // namespace Net
} // namespace Pixy

#endif // H_PixyNet_Connection_H
