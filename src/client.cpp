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

#include "client.hpp"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {

  client::client(boost::asio::io_service& io_service)
    : io_service_(io_service),
      conn_(new connection(io_service, "127.0.0.1", "60100")),
      timer_(io_service_)
  {
    conn_->connect();
    try {
      conn_->start();
    } catch (std::exception& e) {
      std::cerr << "couldnt start the conn .. " << e.what() << "\n";
      throw e;
    }

    std::cout << "Size of events : " << sizeof(Event) << "b\n";

    Event foo(EventUID::Login);
    foo.setProperty("Puppet", "ThisIsAVeryLongPuppetName");
    foo.setProperty("Spell", "ThisIsAVeryLongSpellName");
    foo.setProperty("Unit1", "ThisIsAVeryLongUnitName");
    foo.setProperty("Unit2", "ThisIsAVeryLongUnitName");
    foo.setProperty("Unit3", "ThisIsAVeryLongUnitName");

    timer_.expires_from_now(boost::posix_time::seconds(2));
    timer_.wait();
    conn_->send(foo);

    //message foo(message_id::foo);
    //foo.body = "HAI";
    for (int i=0; i < 3; ++i) {
      //conn_->send(Event("Pong"));

    }

    std::cout << "Size of events : " << sizeof(Event) << "b\n";

    //timer_.async_wait(boost::bind(&client::disconnect, this, boost::asio::placeholders::error));
    //send_req();
  }

  client::~client() {
    conn_->stop();
    conn_.reset();
  }



}
}
