//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include "message.hpp"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

namespace http {
namespace server3 {

server* server::__instance = 0;

server::server()
  : io_service_(),
    thread_pool_size_(4),
    acceptor_(io_service_),
    new_connection_(new connection(io_service_)),
    ping_timer_(io_service_),
    strand_(io_service_)
{
  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query("127.0.0.1", "60100");
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
  acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&server::handle_accept, this,
        boost::asio::placeholders::error));
}

server& server::singleton() {
  if (!__instance)
    __instance = new server();

  return *__instance;
}

void server::run()
{
  // Create a pool of threads to run all of the io_services.
  //std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
    workers.create_thread(boost::bind(&server::work, boost::ref(this)));

  std::cout << "Server up, accepting connections on " << thread_pool_size_ << " threads\n";
  std::cout
    << "Messages header length: " << message::header_length
    << ", footer length: " << message::footer_length << "\n";

  // start our player ping timer
  //refresh_timer();

  // Wait for all threads in the pool to exit.
  workers.join_all();
}

void server::work() {
  try {
    io_service_.run();
  } catch (std::exception& e) {
    std::cerr << "an exception caught in a worker, aborting: " << e.what() << "\n";
  }
}

void server::stop()
{
  for (connection_ptr conn : connections)
    conn->stop();

  connections.clear();

  new_connection_.reset();

  io_service_.stop();
  std::cout << "Server down\n";
}

void server::handle_accept(const boost::system::error_code& e)
{
  if (!e)
  {
    connections.push_back(new_connection_);

    new_connection_->start();
    new_connection_.reset(new connection(io_service_));
    acceptor_.async_accept(new_connection_->socket(),
        boost::bind(&server::handle_accept, this,
          boost::asio::placeholders::error));
  }
}

void server::close(connection_ptr conn) {
  strand_.post( boost::bind(&server::do_close, this, conn) );
}
void server::do_close(connection_ptr conn) {
  /*std::cout << "there r " << connections.size() << " conns open\n";
  for (connection_ptr local : connections)
    if (local == conn) {
      local.reset();
      break;
    }*/

  connections.remove(conn);
  //std::cout << "the conn has " << conn.use_count() << " referencers\n";
  //delete conn.get();
  conn.reset();
  //std::cout << "there r " << connections.size() << " conns open\n";
}
/*void server::close(gconnection_ptr conn) {
  guests.remove(conn);
  conn.reset();
}*/

void server::ping_clients(const boost::system::error_code& error) {
  if (!connections.empty())
    for (connection_ptr conn : connections) { conn->ping(); }

  refresh_timer();
}

void server::refresh_timer() {
  ping_timer_.expires_from_now(boost::posix_time::seconds(5));
  ping_timer_.async_wait(boost::bind(&server::ping_clients, this, boost::asio::placeholders::error));
}
} // namespace server3
} // namespace http
