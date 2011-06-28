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

namespace Pixy {
namespace Net {

  /// The top-level class of the HTTP server.
  class server : private boost::noncopyable
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
    // connection can mark itself as dead by calling close()
    friend class connection;

    void do_stop();

    // a thread handling io_service::run()
    void work();

    // marks the connection as dead
    // @note: called by the connection itself when it needs to shutdown
    void close(connection_ptr);
    void _mark_dead(connection_ptr);

    // actually removes the connection from its respective container
    void do_close(connection_ptr);
    // cleans up all dead connections awaiting removal
    void cleanup();

    // orders every connection to send a PING msg to track dead connections
    // @note: after all connections are pinged, server::cleanup() is called
    // right before the ping timer is refreshed, this is done by the use of
    // the server strand object
    void ping_clients(const boost::system::error_code& error);
    // resets the ping timer to reactivate in another ping_interval seconds
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

    //std::list<connection_ptr> connections;
    //std::vector<connection_ptr> dead_connections;
    //std::list<gconnection_ptr> guests;

    //boost::asio::deadline_timer ping_timer_;
    const int ping_interval;

    static server* __instance;

    boost::thread_group workers;
  };


} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_SERVER_HPP
