//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_CLIENT_CONNECTION_HPP
#define HTTP_SERVER3_CLIENT_CONNECTION_HPP

#include "connection.hpp"

namespace Pixy {
namespace Net {

  /// Represents a single connection from a client.
  class client_connection : public connection
  {

  public:
    /// Construct a connection with the given io_service.
    explicit client_connection(boost::asio::io_service&, std::string host, std::string port);
    virtual ~client_connection();

    bool connect();
    bool is_connected();
    void disconnect();

    virtual boost::shared_ptr<client_connection> shared_from_this() {
      return boost::static_pointer_cast<client_connection>(connection::shared_from_this());
    }

    virtual void start();

  protected:
    tcp::resolver resolver_;
    std::string host_, port_;
    bool connected_;

    void handle_resolve(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);

  };

  typedef boost::shared_ptr<client_connection> client_connection_ptr;

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
