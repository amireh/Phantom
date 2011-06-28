//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <list>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include "connection.hpp"
//#include "guest_connection.hpp"


namespace http {
namespace server3 {

/// The top-level class of the HTTP server.
class server
  : private boost::noncopyable
{
public:
  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit server();

  static server& singleton();

  /// Run the server's io_service loop.
  void run();

  /// Stop the server.
  void stop();



private:
  friend class connection;

  void work();
  void close(connection_ptr);

  void do_close(connection_ptr);


  void ping_clients(const boost::system::error_code& error);
  void refresh_timer();

  /// Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& e);

  /// The number of threads that will call io_service::run().
  const std::size_t thread_pool_size_;

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  boost::asio::strand strand_;

  /// The next connection to be accepted.
  connection_ptr new_connection_;

  std::list<connection_ptr> connections;
  //std::list<gconnection_ptr> guests;

  boost::asio::deadline_timer ping_timer_;

  static server* __instance;

  boost::thread_group workers;
};


} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_SERVER_HPP
