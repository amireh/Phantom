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

#ifndef H_PixyNet_MessageParser_H
#define H_PixyNet_MessageParser_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <exception>
#include <stdexcept>
#include <iostream>
#include "message.hpp"

namespace Pixy {
namespace Net {

  class request_incomplete : public std::runtime_error {
    public:

    request_incomplete(std::string msg, int in_bytes_left)
    : std::runtime_error(msg),
      bytes_left(in_bytes_left) {
    }
    virtual ~request_incomplete() throw() {
    }

    int bytes_left;
  };

  class bad_request : public std::runtime_error {
    public:

    bad_request(std::string msg)
    : std::runtime_error(msg) {
    }
    virtual ~bad_request() throw() {
    }

  };

  class message_parser {
    public:
    typedef boost::asio::buffers_iterator<
      boost::asio::streambuf::const_buffers_type> iterator;

      message_parser();

      bool parse_header(message &msg, boost::asio::streambuf& in);
      bool parse_body(message& msg, boost::asio::streambuf& in);
      bool parse_all(message &msg, boost::asio::streambuf& in);

      void dump(const message &msg, boost::asio::streambuf& out);
      void dump(const message &msg, boost::asio::mutable_buffer& out);

    private:
      /// Handle the next character of input.
      boost::tribool consume(message& msg, char input);
  };

}
}
#endif
