#ifndef H_PixyNet_MessageParser_H
#define H_PixyNet_MessageParser_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <exception>
#include <stdexcept>
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
      //void parse(message &msg, boost::asio::mutable_buffer& in);
      void dump(const message &msg, boost::asio::streambuf& out);
      void dump(const message &msg, boost::asio::mutable_buffer& out);

    private:
      /// Handle the next character of input.
      boost::tribool consume(message& msg, char input);
  };

}
}
#endif
