//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_gconnection_HPP
#define HTTP_SERVER3_gconnection_HPP

#include "connection.hpp"

namespace http {
namespace server3 {

/// Represents a single connection from a client.
class gconnection :
  public connection
{
public:
  /// Construct a connection with the given io_service.
  explicit gconnection(boost::asio::io_service& io_service);
  virtual ~gconnection();

  //virtual void ping() = delete;

protected:
  virtual void shutdown();
};

typedef boost::shared_ptr<gconnection> gconnection_ptr;

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
