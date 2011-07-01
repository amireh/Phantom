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

#include "dispatcher.hpp"

namespace Pixy {
namespace Net {

dispatcher::dispatcher(boost::asio::io_service& io_service)
: strand_(io_service) {
}

dispatcher::~dispatcher() {
  events.clear();
}

void dispatcher::deliver(const Event& evt) {
  events.push_back(Event(evt));

  strand_.post( boost::bind( &dispatcher::dispatch, this ) );

}

void dispatcher::dispatch() {
  const Event& evt = events.front();

  evt_handlers_t::const_iterator handlers = evt_handlers_.find(evt.UID);
  if (handlers != evt_handlers_.end())
    for (evt_handler_t handler : handlers->second)
      handler( evt );

  events.pop_front();
}

}
}
