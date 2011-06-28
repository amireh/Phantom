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

#include "message_handler.hpp"
namespace Pixy {
namespace Net {

message_handler::message_handler(boost::asio::io_service& io_service)
: strand_(io_service) {
}

message_handler::~message_handler() {
  messages.clear();
}

void message_handler::send(const message &msg, boost::asio::streambuf& out) {
}

void message_handler::recv(boost::asio::streambuf& in) {

}

void message_handler::deliver(const message& msg) {
  messages.push_back(message(msg));

  strand_.post( boost::bind( &message_handler::dispatch, this ) );
}

void message_handler::dispatch() {
  const message& msg = messages.front();

  handlers_t::const_iterator itr = handlers_.find(msg.id);
  if (itr == handlers_.end())
    return;

  for (msg_handler_t handler : itr->second)
    //socket_->get_io_service().post( boost::bind(handler, msg) );
    handler( msg );
    //socket_->get_io_service().post( strand_.wrap( boost::bind(handler, msg) ) );

  messages.pop_front();
}

}
}
