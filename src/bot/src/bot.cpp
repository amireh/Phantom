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

#include "bot.hpp"
#include "ResourceManager.h"
#include "Archiver.h"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {

  int client::client_id = 0;

  client::client(boost::asio::io_service& io_service, bool oddOrEven)
    : io_service_(io_service),
      odd_(oddOrEven),
      conn_(),
      timer_(io_service_),
      puppet_(0),
      active_puppet_(0),
      waiting_puppet_(0)
  {
    conn_.reset(new connection(io_service, "0.0.0.0", "60100"));
    if (!conn_->connect())
      throw std::runtime_error("couldn't connect to server!");

    try {
      conn_->start();
    } catch (std::exception& e) {
      std::cerr << "couldnt start the conn .. " << e.what() << "\n";
      throw e;
    }

    conn_->get_dispatcher().bind(EventUID::Login, this, &client::on_login);
    conn_->get_dispatcher().bind(EventUID::SyncGameData, this, &client::on_sync_game_data);
    conn_->get_dispatcher().bind(EventUID::SyncPuppets, this, &client::on_sync_puppets);
    conn_->get_dispatcher().bind(EventUID::JoinLobby, this, &client::on_join_lobby);
    conn_->get_dispatcher().bind(EventUID::JoinQueue, this, &client::on_join_queue);

    conn_->get_dispatcher().bind(EventUID::MatchFound, this, &client::on_match_found);
    conn_->get_dispatcher().bind(EventUID::SyncMatchPuppets, this, &client::on_sync_match_puppets);
    conn_->get_dispatcher().bind(EventUID::StartTurn, this, &client::on_start_turn);
    conn_->get_dispatcher().bind(EventUID::TurnStarted, this, &client::on_turn_started);
    conn_->get_dispatcher().bind(EventUID::DrawSpells, this, &client::on_draw_spells);
    conn_->get_dispatcher().bind(EventUID::CastSpell, this, &client::on_cast_spell);
    conn_->get_dispatcher().bind(EventUID::CreateUnit, this, &client::on_create_unit);
    conn_->get_dispatcher().bind(EventUID::UpdatePuppet, this, &client::on_update_puppet);
    conn_->get_dispatcher().bind(EventUID::UpdateUnit, this, &client::on_update_unit);
    conn_->get_dispatcher().bind(EventUID::RemoveUnit, this, &client::on_remove_unit);

    // combat
    conn_->get_dispatcher().bind(EventUID::Charge, this, &client::on_charge);
    conn_->get_dispatcher().bind(EventUID::CancelCharge, this, &client::on_cancel_charge);
    conn_->get_dispatcher().bind(EventUID::StartBlockPhase, this, &client::on_start_block_phase);
    conn_->get_dispatcher().bind(EventUID::Block, this, &client::on_block);
    conn_->get_dispatcher().bind(EventUID::CancelBlock, this, &client::on_cancel_block);
    conn_->get_dispatcher().bind(EventUID::EndBlockPhase, this, &client::on_end_block_phase);



    std::cout << "Size of events : " << sizeof(Event) << "b\n";

    if (odd_) {
      account_name_ = "Kandie";
      puppet_name_ = "Cranberry";
      deck_name_ = "Earth Template 1";
    } else {
      account_name_ = "Sugarfly";
      puppet_name_ = "Batata";
      deck_name_ = "Fire Template 1";
    }

    Event foo(EventUID::Login);
    foo.setProperty("Username", account_name_);
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
    if (puppet_) // __DEBUG__ since both puppets r named the same
      return;
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

  Unit* client::get_unit(int inUID) {
    for (auto puppet : puppets_)
      for (auto unit : puppet->getUnits())
        if (unit->getUID() == inUID)
          return unit;

    throw invalid_uid("in client::get_unit() : " + stringify(inUID));
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



    Event foo(EventUID::SyncPuppets);
    conn_->send(foo);

  }

  void client::on_sync_puppets(const Event& e)
  {
    Event foo(EventUID::JoinLobby);
    foo.setProperty("Puppet", puppet_name_);
    conn_->send(foo);
  }
  void client::on_join_lobby(const Event& evt) {
    Event foo(EventUID::JoinQueue);
    foo.setProperty("D", deck_name_);
    //~ foo.setProperty("Puppet", puppet_name_);
    conn_->send(foo);
  }

  void client::on_join_queue(const Event& evt) {
    if (evt.Feedback == EventFeedback::Ok)
      std::cout << "I've joined the queue!\n";
    else
      std::cout << "I couldn't join the queue :(\n";
  }

  void client::on_match_found(const Event& evt) {
    Event foo(EventUID::SyncMatchPuppets);
    conn_->send(foo);
  }

  void client::on_sync_match_puppets(const Event& evt) {

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

  void client::handle_new_turn() {


    // remove all expired buffs
    {
      std::vector<Spell*> expired;
      for (auto buff : active_puppet_->getBuffs())
        if (buff->hasExpired()) {
          expired.push_back(buff);
        }
      for (auto expired_spell : expired)
        active_puppet_->detachBuff(expired_spell->getUID());
    }
    // apply all the buffs on the puppet and its units
    for (auto buff : active_puppet_->getBuffs()) {
      /*
       * Spell: Nourish
       * Action:
       *  increase the puppet's HP by 3
       */
      if (buff->getName() == "Nourish") {
        active_puppet_->setHP(active_puppet_->getHP() + 3);
      }

      // reduce the duration left of the buff, and mark as expired for removal
      // if it's over
      buff_durations_.find(buff)->second -= 1;
      if (buff_durations_.find(buff)->second == 0)
        buff->setExpired(true);
    }

    for (auto unit : puppet_->getUnits()) {
      unit->getUp();
      // remove all expired buffs
      {
        std::vector<Spell*> expired;
        for (auto buff : unit->getBuffs())
          if (buff->hasExpired()) {
            expired.push_back(buff);
          }
        for (auto expired_spell : expired)
          unit->detachBuff(expired_spell->getUID());
      }
      for (auto buff : unit->getBuffs()) {
        // handle buff TODO
      }
    }
  }
  void client::on_start_turn(const Event& evt) {
    waiting_puppet_ = active_puppet_;
    active_puppet_ = puppet_;

    conn_->send(evt);

    handle_new_turn();

    // cast a spell
    if (!puppet_->getSpells().empty())
    for (auto spell : puppet_->getSpells()) {
      //if (spell->getName() == "Summon: Fetish Zij") {
        Event evt_(EventUID::CastSpell);
        evt_.setProperty("Spell", spell->getUID());
        conn_->send(evt_);
        //break;
      //}
    }

    // charge with units
    if (!puppet_->getUnits().empty()) {
      timer_.expires_from_now(boost::posix_time::seconds(1));
      timer_.wait();
      for (auto unit : puppet_->getUnits()) {
        if (unit->isRestless() || unit->isDead()) // restless units automatically charge by the server
          continue;
        Event evt_(EventUID::Charge);
        evt_.setProperty("UID", unit->getUID());
        conn_->send(evt_);
      }
    }

    // end our turn
    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait( [&](boost::system::error_code e) -> void {
      Event foo(EventUID::EndTurn);
      conn_->send(foo);
    });
  }

  void client::on_turn_started(const Event& evt) {
    waiting_puppet_ = active_puppet_;
    active_puppet_ = get_puppet(convertTo<int>(evt.getProperty("P")));
    assert(active_puppet_);

    handle_new_turn();
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
        lSpell->_setUID(convertTo<int>(elements[++index]));
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
          Spell *lSpell = lPuppet->getSpell(convertTo<int>(elements[++index]));
          std::cout
            << "removing spell with UID " << elements[index]
            << " from puppet " << lPuppet->getUID() << "\n";
          assert(lSpell); // _DEBUG_

          lPuppet->detachSpell(lSpell->getUID());
          lSpell = 0;
        }

        lPuppet = 0;
      }
    }
  }

  void client::on_cast_spell(const Event& evt) {
    if (evt.Feedback == EventFeedback::InvalidRequest) {
      // the UID was invalid
      std::cout << "my request to cast a spell was rejected!\n";
      return;
    }

    // it's ok, let's find the spell
    Spell* _spell = 0;
    Puppet* _puppet = 0;
    for (auto puppet : puppets_)
      try {
        _spell = puppet->getSpell(convertTo<int>(evt.getProperty("Spell")));
        _puppet = puppet;
        break;
      } catch (...) { _spell = 0; }
    assert(_spell && _puppet);


    // ...

    // if it's a buff, track its duration
    if (_spell->getDuration() > 0) {
      buff_durations_.insert(std::make_pair(_spell, _spell->getDuration()));
    }

    /*
     * Spell: Chains of Command
     * Action:
     *  detach target unit from its current owner
     *  and move ownership to the caster puppet
     */
    {
      if (_spell->getName() == "Chains of Command") {
        Unit* _unit = 0;
        for (auto puppet : puppets_)
          for (auto unit : puppet->getUnits()) {
            if (unit->getUID() == convertTo<int>(evt.getProperty("T"))) {
              _unit = unit;
              break;
            }
          }
        assert(_unit);

        ((Puppet*)_unit->getOwner())->detachUnit(_unit->getUID(), false /* don't delete */);
        _puppet->attachUnit(_unit);

        std::cout
          << "**MIND CONTROLLED! Unit: " << _unit->getName() << "#"<<_unit->getUID()
          <<"is now owned by " <<_puppet->getName()<<"#"<<_puppet->getUID()<<"\n";
      }
    } // end of Spell: Chains of Command

    /*
     * Spell: Nourish
     * Action:
     *  attach the spell as a buff
     */
    {
      if (_spell->getName() == "Nourish") {
        _puppet->attachBuff(_spell);
        _spell->setExpired(false);

        std::cout
          << "**NOURISH! Puppet: " << _puppet->getName() << "#"<<_puppet->getUID() << "\n";
      }
    } // end of Spell: Chains of Command

    std::cout << "casted a spell! " << _spell->getName() << "#" << _spell->getUID() << "\n";
    // remove it from the puppet's hand
    _puppet->detachSpell(_spell->getUID(), _spell->getDuration() == 0 /* delete only if it's not a buff */);
  }

  void client::on_create_unit(const Event& evt) {
    assert(evt.hasProperty("Name") && evt.hasProperty("OUID") && evt.hasProperty("UID"));

    Puppet* _owner = get_puppet(convertTo<int>(evt.getProperty("OUID")));
    assert(_owner);

    std::cout << "CreateUnit name: " << evt.getProperty("Name") << "\n";
    Unit* _unit = rmgr_.getUnit(evt.getProperty("Name"), _owner->getRace());
    assert(_unit);

    _unit->deserialize(evt);
    _owner->attachUnit(_unit);

    _unit = 0;
    _owner = 0;
  }

  void client::on_update_puppet(const Event& evt) {
    assert(evt.hasProperty("UID"));

    Puppet* _puppet = get_puppet(convertTo<int>(evt.getProperty("UID")));
    assert(_puppet);

    std::cout << "Updating puppet named: " << _puppet->getName() << "\n";

    _puppet->deserialize(evt);
  }

  void client::on_charge(const Event& evt) {
    Unit *attacker = 0;

    assert(evt.hasProperty("UID"));
    try {
      attacker = active_puppet_->getUnit(convertTo<int>(evt.getProperty("UID")));
    } catch (invalid_uid& e) {
      std::cout  << "invalid charge event parameters : " << e.what();
    }

    // move the unit
    std::cout << evt.getProperty("UID") << " is charging for an attack\n";

    attackers_.push_back(attacker);
  }

  void client::on_update_unit(const Event& evt) {
    assert(evt.hasProperty("UID"));

    Unit* _unit = get_unit(convertTo<int>(evt.getProperty("UID")));
    assert(_unit);

    std::cout << "Updating unit named: " << _unit->getName() << "#" << _unit->getUID() << "\n";

    _unit->deserialize(evt);
    if (_unit->isDead())
      ((Puppet*)_unit->getOwner())->detachUnit(_unit->getUID());

  }

  void client::on_remove_unit(const Event& e) {
    Unit* _unit = get_unit(convertTo<int>(e.getProperty("UID")));
    ((Puppet*)_unit->getOwner())->detachUnit(_unit->getUID());
  }

  void client::on_cancel_charge(const Event& evt) {
    Unit *attacker = 0;

    assert(evt.hasProperty("UID"));
    try {
      attacker = active_puppet_->getUnit(convertTo<int>(evt.getProperty("UID")));
    } catch (invalid_uid& e) {
      std::cout  << "invalid charge event parameters : " << e.what();
    }

    // move the unit back
    std::cout << evt.getProperty("UID") << " is not charging anymore\n";

    attackers_.remove(attacker);
  }

  void client::on_start_block_phase(const Event& e) {
    // is it me being attacked?
    // if not just return
    if (active_puppet_ == puppet_) {
      std::cout << "Opponent's blocking phase is starting"
       << active_puppet_->getUID() << " vs " << puppet_->getUID() << "\n";
      return;
    }

    std::cout << "I'm being attacked, in blocking phase now\n";

    // block with all the units ive got
    Event resp(EventUID::EndBlockPhase);
    conn_->send(resp);
  }


  void client::on_block(const Event& evt) {
    Unit *attacker, *blocker = 0;

    assert(evt.hasProperty("A") && evt.hasProperty("B"));
    try {
      attacker = active_puppet_->getUnit(convertTo<int>(evt.getProperty("A")));
      blocker = waiting_puppet_->getUnit(convertTo<int>(evt.getProperty("B")));
    } catch (invalid_uid& e) {
      std::cout  << "invalid block event parameters : " << e.what();
    }

    // mark the blocker
    blockers_t::iterator itr = blockers_.find(attacker);
    if (itr == blockers_.end()) {
      // this is the first unit to block the attacker
      std::list<Unit*> tmp;
      tmp.push_back(blocker);
      blockers_.insert(std::make_pair(attacker, tmp));
    } else {
      // there are already some blockers marked for this attacker

      // verify that the blocker is not already marked
      bool _valid = true;
      for (auto pair : blockers_)
        for (auto _blocker : pair.second)
          if (_blocker == blocker) {
            _valid = false;
            break;
          }

      if (!_valid) {
        std::cout  << "blocker " << blocker->getUID() << " already marked for blocking a unit";
        return;
      }

      // mark it
      itr->second.push_back(blocker);
    }

    std::cout << "a blocker " << blocker->getUID()
      << " was registered to block " << attacker->getUID() << "\n";
  }

  void client::on_cancel_block(const Event& e) {

  }


  void client::on_end_block_phase(const Event& e) {
    // simulate the battle
    std::cout << "simulating battle with : "
      << attackers_.size() << " attacking units and "
      << blockers_.size() << " units being blocked\n";

    for (auto attacker : attackers_) {
      attacker->reset();

      blockers_t::iterator blockers = blockers_.find(attacker);
      if (blockers != blockers_.end()) {
        for (auto blocker : blockers->second) {
          blocker->reset();
          attacker->attack(blocker, true);

          if (blocker->isDead())
            death_list_.push_back(blocker);

          if (attacker->isDead()) {
            death_list_.push_back(attacker);
            break;
          }
        }
      } else {
        std::cout
          << "attacker " << attacker->getUID()
          << " is attacking puppet " << waiting_puppet_->getName()
          << " which has " << waiting_puppet_->getHP();
        attacker->attack(waiting_puppet_);
        std::cout
          << " and is dead? " << (waiting_puppet_->isDead() ? "yes" : "no") << "\n";
        if (waiting_puppet_->isDead()) {
          std::cout << "Game over! " << waiting_puppet_->getName() << " is dead!\n";
        }
      }
    }
    for (auto unit : attackers_)
      unit->rest();

    // clean up dead units
    for (auto unit : death_list_)
      static_cast<Puppet*>((Entity*)unit->getOwner())->detachUnit(unit->getUID());

    // clear combat temps
    death_list_.clear();
    attackers_.clear();
    blockers_.clear();

    conn_->send(Event(EventUID::BattleOver));
  }

}
}
