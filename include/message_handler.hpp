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
#include <map>
#include <deque>
#include "event.hpp"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {
  class message_handler {
    public:
      //typedef boost::function<void(const message&)> msg_handler_t;
      typedef boost::function<void(const Event&)> evt_handler_t;

      message_handler(boost::asio::io_service&);
      ~message_handler();

      template <typename T>
      void bind(EventUID evt, T* inT, void (T::*handler)(const Event&)) {
        // register message if it isn't already
        evt_handlers_t::iterator binder = evt_handlers_.find(evt);
        if (binder == evt_handlers_.end())
        {
          std::vector<evt_handler_t> handlers;
          binder = evt_handlers_.insert(make_pair(evt, handlers)).first;
        }

        binder->second.push_back( boost::bind(handler, inT, _1) );
      }

      // delivers a local copy of the message to bound handlers
      // @note: the message might not be dispatched immediately, as it will be
      // queued internally and later on dispatched through the strand object
      //void deliver(const message&);
      void deliver(const Event&);

    private:
      void dispatch();

      boost::asio::strand strand_;

      typedef std::map< EventUID , std::vector<evt_handler_t> > evt_handlers_t;
      evt_handlers_t evt_handlers_;

      std::deque<Event> events;
  };

}
}
#endif
