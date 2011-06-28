//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_pconnection_HPP
#define HTTP_SERVER3_pconnection_HPP

#include "connection.hpp"
#include "message.hpp"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace http {
namespace server3 {

/// Represents a single connection from a client.
class pconnection : public connection, public boost::enable_shared_from_this<pconnection>
{
public:
  /// Construct a connection with the given io_service.
  explicit pconnection(boost::asio::io_service& io_service);
  virtual ~pconnection();

  void ping();

protected:
  virtual void shutdown();

private:
  void on_pong(boost::shared_ptr<message> msg);
  void on_foo(boost::shared_ptr<message> msg);

  int ping_timeouts; // if client doesnt respond to 3 ping requests, the connection is closed
};

typedef boost::shared_ptr<pconnection> pconnection_ptr;

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
