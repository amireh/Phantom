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

    conn_->get_dispatcher().bind(EventUID::Login, this, &client::on_login);
    conn_->get_dispatcher().bind(EventUID::SyncGameData, this, &client::on_sync_game_data);
    conn_->get_dispatcher().bind(EventUID::JoinQueue, this, &client::on_join_queue);
    //conn_->get_dispatcher().bind(EventUID::MatchFound, this, &client::on_match_found);
    conn_->get_dispatcher().bind(EventUID::SyncPuppetData, this, &client::on_sync_puppet_data);
    conn_->get_dispatcher().bind(EventUID::StartTurn, this, &client::on_start_turn);
    conn_->get_dispatcher().bind(EventUID::TurnStarted, this, &client::on_turn_started);
    conn_->get_dispatcher().bind(EventUID::DrawSpells, this, &client::on_draw_spells);


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


	void client::register_puppet(Puppet* inPuppet) {
		puppets_.push_back(inPuppet);
    if (inPuppet->getName() == puppet_name_)
      assign_puppet(inPuppet);
	}

  void client::assign_puppet(Puppet* inPuppet) {
    assert(inPuppet);
    std::cout << "I'm playing with a puppet named " << inPuppet->getName() << "\n";
    puppet_ = inPuppet;
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Helpers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

  Puppet* client::get_puppet() {
    return puppet_;
  }

  client::puppets_t const& client::get_puppets() {
    return puppets_;
  }

  Puppet* client::get_puppet(int inUID) {
    puppets_t::const_iterator itr;
    for (itr = puppets_.begin(); itr != puppets_.end(); ++itr)
      if ((*itr)->getUID() == inUID)
        return *itr;

    return 0;
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

    std::string senc = evt.getProperty("Data");

    vector<unsigned char> encoded(senc.begin(), senc.end());
    vector<unsigned char> raw;

    if (Archiver::decodeLzma(raw, encoded, evt.Rawsize) != 1) {
      std::cerr << "decoding failed!! \n";
    }

    string raw2str(raw.begin(), raw.end());

    std::istringstream datastream(raw2str);

    rmgr_.populate(datastream);

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

  void client::on_sync_puppet_data(const Event& evt) {

    using std::string;
    using std::vector;

    std::istringstream datastream(evt.getProperty("Data"));
    list<Puppet*> lPuppets = rmgr_.puppetsFromStream(datastream);
    datastream.clear();

    for (auto puppet : lPuppets) {
      this->register_puppet(puppet);
      puppet->live();
    }

    Event foo(EventUID::Ready);
    conn_->send(foo);
  }


  void client::on_start_turn(const Event& evt) {
    conn_->send(evt);
    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait( [&](boost::system::error_code e) -> void {
      Event foo(EventUID::EndTurn);
      conn_->send(foo);
    });
  }

  void client::on_turn_started(const Event& evt) {

  }

  void client::on_draw_spells(const Event& evt) {
    using std::vector;
    using std::string;
    using std::istringstream;

    std::cout << "parsing drawn spells \n";

    //BitStream lStream(inPkt->data, inPkt->length, false);
    //lStream.IgnoreBytes(1); // skip the packet identifier

    //RakString tmp;
    //lStream.Read(tmp);

    istringstream lData(evt.getProperty("Data"));
    string lLine;

    // parse the drawn spells and add them to our hand
    {
      // how many spells to create
      getline(lData, lLine);
      int nrDrawSpells = atoi(Utility::split(lLine, ';').back().c_str());

      // the next line contains the actual string UIDs/names and the owner uid
      getline(lData, lLine);
      vector<string> elements = Utility::split(lLine, ';');

      int lPuppetUID = convertTo<int>(elements[0].erase(0,1).c_str()); // strip out the leading $
      Puppet* lPuppet = get_puppet(lPuppetUID);
      assert(lPuppet); // _DEBUG_

      //int nrSpells = convertTo<int>(elements[1].c_str());
      assert((elements.size()-1)/2 == nrDrawSpells);

      int spellsParsed = 0;
      int index = 0;
      while (++spellsParsed <= nrDrawSpells) {
        Spell* lSpell = rmgr_.getSpell(elements[++index]);
        lSpell->setUID(convertTo<int>(elements[++index]));
        lSpell->setCaster(lPuppet);
        lPuppet->attachSpell(lSpell);

        std::cout
          << "attaching spell with UID: " << lSpell->getUID()
          << " to puppet " << lPuppet->getUID() << "\n";

        lSpell = 0;
      }

      lPuppet = 0;
    }

    // parse the spells to be discarded
    {
      getline(lData, lLine);
      int nrDropSpells = atoi(Utility::split(lLine, ';').back().c_str());

      if (nrDropSpells > 0) {

        std::cout << "dropping " << nrDropSpells << " spells\n";

        // the next line contains the actual string UIDs/names and the owner uid
        getline(lData, lLine);
        vector<string> elements = Utility::split(lLine, ';');

        int lPuppetUID = convertTo<int>(elements[0].erase(0,1).c_str()); // strip out the leading $
        Puppet* lPuppet = get_puppet(lPuppetUID);
        assert(lPuppet); // _DEBUG_

        //int nrSpells = convertTo<int>(elements[1].c_str());
        assert((elements.size()-1) == nrDropSpells);

        int spellsParsed = 0;
        int index = 0;
        while (++spellsParsed <= nrDropSpells) {
          Spell* lSpell = (Spell*)lPuppet->getSpell(convertTo<int>(elements[++index]));
          std::cout
            << "removing spell with UID " << elements[index]
            << " from puppet " << lPuppet->getUID() << "\n";
          assert(lSpell); // _DEBUG_

          lPuppet->detachSpell(lSpell);
          lSpell = 0;
        }

        lPuppet = 0;
      }
    }
  }

}
}
