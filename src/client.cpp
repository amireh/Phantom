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
#include "ResourceManager.h"
#include "Archiver.h"

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

    conn_->get_message_handler().bind(EventUID::Login, this, &client::on_login);
    conn_->get_message_handler().bind(EventUID::JoinQueue, this, &client::on_join_queue);
    conn_->get_message_handler().bind(EventUID::SyncGameData, this, &client::on_sync_game_data);

    std::cout << "Size of events : " << sizeof(Event) << "b\n";

    Event foo(EventUID::Login);
    foo.setProperty("Username", "Kandie");
    foo.setProperty("Password", "tuonela");
    conn_->send(foo);


    //foo.Options &= 0;
    //foo.Options |= Event::NoFormat;
    //foo.setProperty("Data", "ThisIsAVeryLongPuppetName;asd,lzxoc$!\r\n\r\nFOO");
    //foo.dump();
    //return;
    /*foo.setProperty("Username", "Kandie");
    foo.setProperty("Password", "tuonela");*/

    //timer_.expires_from_now(boost::posix_time::seconds(2));
    //timer_.async_wait( boost::bind( &connection::send, conn_, foo ));
    //conn_->send(foo);



    std::cout << "Size of events : " << sizeof(Event) << "b\n";

    //timer_.async_wait(boost::bind(&client::disconnect, this, boost::asio::placeholders::error));
    //send_req();
  }

  client::~client() {
    conn_->stop();
    conn_.reset();
  }

  void client::on_login(const Event& evt) {
    if (evt.Feedback == EventFeedback::Ok) {
      std::cout << "logged in! syncing game data\n";
      conn_->send(Event(EventUID::SyncGameData));
      /*Event foo(EventUID::JoinQueue);
      foo.setProperty("Puppet", "Kandie");
      conn_->send(foo);*/
    } else {
      std::cerr << "err!! couldn't log in\n";
    }
  }

  void client::on_sync_game_data(const Event& evt) {
    //std::cout << (evt.Feedback == EventFeedback::Ok ? "logged in!" : "couldn't log in") << "\n";
    //std::cout << evt.getProperty("Data");
    ResourceManager rmgr;

    std::string senc = evt.getProperty("Data");

    vector<unsigned char> encoded(senc.begin(), senc.end());
    vector<unsigned char> raw;

    if (Archiver::decodeLzma(raw, encoded, evt.Rawsize) != 1) {
      std::cerr << "decoding failed!! \n";
    }

    string raw2str(raw.begin(), raw.end());

    std::istringstream datastream(raw2str);

    rmgr.populate(datastream);

    std::cout << "attmepting to join the queue now\n";

    Event foo(EventUID::JoinQueue);
    foo.setProperty("Puppet", "Kandie");
    conn_->send(foo);

  }

  void client::on_join_queue(const Event& evt) {
    if (evt.Feedback == EventFeedback::Ok)
      std::cout << "I've joined the queue!\n";
    else
      std::cout << "I couldn't join the queue :(\n";
  }
}
}
