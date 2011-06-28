//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef H_PixyNet_Client_H
#define H_PixyNet_Client_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "client_connection.hpp"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {

  class client
  {
    public:
      client(boost::asio::io_service& io_service);
      virtual ~client();

      void run();

    private:
      client_connection_ptr conn_;

      void on_ping(const message &);
      //void handle_write_request(const boost::system::error_code& err);
      //void handle_read_status_line(const boost::system::error_code& err);
      //void handle_read_headers(const boost::system::error_code& err);
      //void handle_read_content(const boost::system::error_code& err);
      //void do_read();
      //void handle_read(const boost::system::error_code& e,
      //    std::size_t bytes_transferred);
      //
      //tcp::resolver resolver_;
      //tcp::socket socket_;
      //boost::asio::streambuf request_;
      //boost::asio::streambuf response_;
      boost::asio::io_service &io_service_;
      //boost::asio::strand strand_;


      boost::asio::deadline_timer timer_;

      bool connected_;
  };

}
}
#endif
