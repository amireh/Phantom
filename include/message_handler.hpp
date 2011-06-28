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

#ifndef H_PixyNet_MessageHandler_H
#define H_PixyNet_MessageHandler_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <exception>
#include <stdexcept>
#include "message_parser.hpp"
#include <map>
#include <deque>

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {
  class message_handler {
    public:
      typedef boost::function<void(const message&)> msg_handler_t;

      message_handler(boost::asio::io_service&);
      ~message_handler();

      void send(const message &msg, boost::asio::streambuf& out);
      void recv(boost::asio::streambuf& in);

      template <typename T>
      void bind(message_id msg, T* inT, void (T::*handler)(const message &)) {
        // register message if it isn't already
        handlers_t::iterator binder = handlers_.find(msg);
        if (binder == handlers_.end())
        {
          std::vector<msg_handler_t> handlers;
          binder = handlers_.insert(make_pair(msg, handlers)).first;
        }

        binder->second.push_back( boost::bind(handler, inT, _1) );
      }

      // delivers a local copy of the message to bound handlers
      // @note: the message might not be dispatched immediately, as it will be
      // queued internally and later on dispatched through the strand object
      void deliver(const message&);

    private:
      void dispatch();

      boost::asio::strand strand_;

      typedef std::map< message_id, std::vector<msg_handler_t> > handlers_t;
      handlers_t handlers_;

      std::deque<message> messages;
  };

}
}
#endif
