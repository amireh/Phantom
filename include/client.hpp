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
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "message.hpp"
#include "message_parser.hpp"
#include "message_handler.hpp"

using boost::asio::ip::tcp;
class client
{
  public:
    client(boost::asio::io_service& io_service);
    virtual ~client();

    void send_req();
    void run();



  private:
    message_parser message_parser_;
    message_handler message_handler_;

    void disconnect(const boost::system::error_code& error);

    void connect();

    void on_ping(boost::shared_ptr<message>);

    void handle_resolve(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_write_request(const boost::system::error_code& err);
    void handle_read_status_line(const boost::system::error_code& err);
    void handle_read_headers(const boost::system::error_code& err);
    void handle_read_content(const boost::system::error_code& err);
    void do_read();
    void handle_read(const boost::system::error_code& e,
        std::size_t bytes_transferred);

    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    boost::asio::io_service &io_service_;
    boost::asio::strand strand_;
    std::string server_, port_;

    boost::asio::deadline_timer timer_;

    bool connected_;
};

#endif
