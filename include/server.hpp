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

#ifndef H_PixyNet_Server_H
#define H_PixyNet_Server_H

#include <string>
#include <vector>
#include <list>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include "message.hpp"
#include "server_connection.hpp"

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

  protected:
    // connection can mark itself as dead by calling close()
    friend class connection;

    // marks the connection as dead and will be removed sometime later
    // @note: called by the connection itself when it needs to shutdown
    void _close(connection_ptr);

  private:

    void do_stop();

    // a thread handling io_service::run()
    void work();

    // marks the connection as dead
    void mark_dead(connection_ptr);

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

    std::list<connection_ptr> connections;
    std::vector<connection_ptr> dead_connections;
    //std::list<gconnection_ptr> guests;

    boost::asio::deadline_timer ping_timer_;
    const int ping_interval;

    static server* __instance;

    boost::thread_group workers;
  };


} // namespace Net
} // namespace Pixy

#endif // H_PixyNet_Server_H
