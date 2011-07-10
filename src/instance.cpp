/*
 *  instance.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "instance.hpp"
#include "server.hpp"

#include "tolua++.h"

TOLUA_API int  tolua_EServer_open (lua_State* tolua_S);
TOLUA_API int  tolua_EShared_open (lua_State* tolua_S);
TOLUA_API int  tolua_Event_open (lua_State* tolua_S);

namespace Pixy {
namespace Net {

	instance::instance(players_t in_players, boost::asio::io_service& io_service)
  : players_(in_players),
    dispatcher_(io_service),
    strand_(io_service),
    active_puppet_(),
    active_player_(),
    waiting_puppet_(),
    waiting_player_(),
    rmgr_(server::singleton().get_resmgr())
  {
		uuid_ = boost::uuids::random_generator()();

		started_ = false;
		nr_ready_players_ = 0;
		//nrSpellsPerTurn = 2;
		uid_generator_ = 0;

		active_player_.reset();
    active_puppet_.reset();

    lua_ = 0;

		log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "instance");
		lua_log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "Lua");

		// register our players
    //for (auto player : in_players)
    //  players_.push_back(player);

    running_ = true;
    std::cout << "an instance has started\n";
	}

	instance::~instance() {

    attackers_.clear();
    blockers_.clear();

		puppets_.clear();

		if (lua_)
			lua_close(lua_);

		log_->infoStream() << "instance shut down";
		if (lua_log_) {
			delete lua_log_;
	  }
		if (log_) {
			delete log_;
    }



    active_player_.reset();
    active_puppet_.reset();
    waiting_player_.reset();
    waiting_puppet_.reset();
	}

  bool instance::operator==(instance const& rhs) {
    return (uuid_ == rhs.uuid_);
  }

  void instance::bootstrap() {
    for (auto player : players_)
      subscribe(player);

		init_lua();

		bind_handlers();

		// give them the puppets so they can populate them
		create_puppets();

		log_->infoStream() << "a match is beginning";
  }



	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Lua
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void instance::init_lua() {
		log_->infoStream() << "opening Lua state";
		// init lua state
		lua_ = lua_open();

		luaL_openlibs(lua_);
		tolua_EServer_open(lua_);
    tolua_EShared_open(lua_);
    tolua_Event_open(lua_);

		//char lFilePath[PATH_MAX];
		//sprintf(lFilePath,  "%s%s/server/pixy_server.lua", PROJECT_ROOT, PROJECT_SCRIPTS );
		std::string lFilePath = "../resources/scripts/pixy_server.lua";

		log_->infoStream() << "using script '" << lFilePath << "'";

		int lErrorCode = luaL_dofile(lua_, lFilePath.c_str());
		if (lErrorCode == 1) {
			log_->errorStream() << "Lua: " << lua_tostring(lua_, -1);
			lua_pop(lua_, -1);
		}

		// give Lua a handle of this instance
		lua_getfield(lua_, LUA_GLOBALSINDEX, "register_instance");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua event processor!";
			lua_pop(lua_,1);
			return;
		}

		tolua_pushusertype(lua_,(void*)this,"Pixy::Net::instance");
		try {
			lua_call(lua_, 1, 1);
		} catch (std::exception& e) {
			log_->errorStream() << "Lua Handler: " << e.what();
		}

		lua_toboolean(lua_, lua_gettop(lua_));
		lua_remove(lua_, lua_gettop(lua_));

		log_->infoStream() << "Lua is up!";
	}

	void instance::lua_log(std::string inMsg) {
		lua_log_->infoStream() << inMsg;
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Misc
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	boost::uuids::uuid instance::get_uuid() const {
		return uuid_;
	}

	bool instance::has_started() {
		return started_;
	}

	/*players_t const& instance::get_players() const {
		return players_;
	}*/

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Helpers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	player_cptr instance::get_sender(const Event& evt) {
    //~ player_events_t::const_iterator itr = player_events_.find(&evt);
    //~ if (itr != player_events_.end())
      //~ return itr->second;
    for (auto player : players_)
      if (player == evt.Sender)
        return player;

    throw UnknownEventSender( std::string("in instance::get_sender: " + stringify((int)evt.UID)) );
	}

  puppet_ptr instance::get_puppet(int inUID) {
    for (auto puppet : puppets_)
      if (puppet->getUID() == inUID)
        return puppet;

    assert(false);
    //return NULL;
  }

  Spell* instance::get_spell(int inUID) {
    Spell* spell=0;
    for (auto puppet : puppets_)
      try {
        spell = puppet->getSpell(inUID);
        break;
      } catch (...) { spell = 0; }

    if (!spell)
      throw invalid_uid(std::string("couldn't find a spell with uid " + stringify(inUID)));

    return spell;
  }

  Unit* instance::get_unit(int inUID) {
    //return units_.find(inUID)->second;
  }

  player_cptr instance::get_player(puppet_ptr inPuppet) {
    for (auto player : players_)
      if (player->get_puppet()->getUID() == inPuppet->getUID())
        return player;


    assert(false); // shouldnt be here
  }

	void instance::broadcast(const Event& evt) {
    for (auto player : players_)
      send(player, evt);
	}

  void instance::send(player_cptr player, const Event& evt) {
    player->send(evt);
  }

  void instance::send(int puppet_uid, const Event& evt) {
    send(get_player(get_puppet(puppet_uid)), evt);
  }

	void instance::reject(Event& evt) {
		evt.Feedback = EventFeedback::InvalidRequest;
		send(get_sender(evt), evt);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Bootstrap
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void instance::bind_handlers() {
    dispatcher_.bind(EventUID::Ready, this, &instance::on_player_ready);
    dispatcher_.bind(EventUID::StartTurn, this, &instance::on_start_turn);
    dispatcher_.bind(EventUID::EndTurn, this, &instance::on_end_turn);
		dispatcher_.bind(EventUID::CastSpell, this, &instance::on_cast_spell);

    dispatcher_.bind(EventUID::Charge, this, &instance::on_charge);
    dispatcher_.bind(EventUID::CancelCharge, this, &instance::on_cancel_charge);
    dispatcher_.bind(EventUID::Block, this, &instance::on_block);
    dispatcher_.bind(EventUID::CancelBlock, this, &instance::on_cancel_block);
    dispatcher_.bind(EventUID::EndBlockPhase, this, &instance::on_end_block_phase);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Main Routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void instance::start() {

		log_->debugStream() << "players are ready, starting turn";

    // start the first player's turn, and track them
		active_puppet_ = puppets_.front();
    active_player_ = get_player(active_puppet_);
    waiting_puppet_ = puppets_.back();
    waiting_player_ = get_player(waiting_puppet_);

    log_->debugStream() << "it's now " << active_player_->get_username() << "'s turn";
		Event evt(EventUID::StartTurn);
		send(active_player_, evt);

    for (auto puppet : puppets_)
      draw_spells(puppet, 4);

		started_ = true;
	}


	void instance::subscribe(player_cptr player) {

		// validate player before hooking them
		if (!player->is_online()) {
			log_->warnStream()
			<< "an offline player attempted to join the instance";

			throw std::runtime_error("an offline player attempted to join the instance");
		}

		//players_.push_back(player);
    assert(player->get_puppet());

    player->get_puppet()->setUID(generate_uid());
    player->get_puppet()->live();

		puppets_.push_back(player->get_puppet());

		// attach them to this instance
		((Player*)player.get())->set_instance(shared_from_this());

		log_->debugStream()
		<< "a puppet named "
		<< player->get_puppet()->getName() << "(" << player->get_puppet()->getUID() << ")"
		<< " has joined the instance";
	}

	void instance::create_puppets() {
		// send profile info for players so they create the puppets
    log_->infoStream() << "sending puppets data to clients";

    list<Puppet const*> lPuppets;
    for (auto puppet : puppets_)
      lPuppets.push_back(puppet.get());

    std::ostringstream stream;
    server::singleton().get_resmgr().puppets_to_stream(stream, lPuppets);
    Event evt(EventUID::SyncPuppetData, EventFeedback::Ok, Event::NoFormat);
    evt.setProperty("Data", stream.str());
    broadcast(evt);
	}

  void instance::enqueue(const Event& evt, player_cptr sender) {
    if (!running_)
      return;

    assert(evt.Sender);
    std::cout << "instance: got evt from " << evt.Sender->get_username() << "\n";
    dispatcher_.deliver(evt);
  }

	const int instance::generate_uid() {
	  return ++uid_generator_;
	}

  void instance::on_dropout(player_cptr player) {
    strand_.post(
      [&, player]() {
      log_->infoStream()
        << "detaching player "
        << player->get_username()
        << " from instance";

      if (active_player_ == player)
        active_player_.reset();

      // TODO: ask opponents if they'd like to wait for player to rejoin
      players_.remove(player);

      if (players_.empty())
        server::singleton()._shutdown_instance(shared_from_this());
    });

    running_ = false;
  }

  /*
   * format:
   * $owner-uid;nr-spells;spell1-name;spell1-uid;spell2-name;spell2-uid;...;\n
   */
	void instance::draw_spells(puppet_ptr inPuppet, int inNrOfSpells) {

    if (!inPuppet)
      inPuppet = active_puppet_;

    const int mMaxSpellsInHand = 6;

    drawn_spells_.str("");
    drawn_spells_ << "[draw];" << inNrOfSpells << "\n";
    drawn_spells_ << "$" << inPuppet->getUID() << ";";

		// create nrSpellsPerTurn spells from the hero's deck
		Deck* lDeck = inPuppet->getDeck();

		int i;
		for (i=0; i< inNrOfSpells; ++i) {
			Spell* lSpell = lDeck->drawSpell();
			// assign UID and attach to puppet
			lSpell->setUID(generate_uid());
			inPuppet->attachSpell(lSpell);

      drawn_spells_ << lSpell->getName() << ";" << lSpell->getUID() << ";";

      lSpell = 0;
		}

    drawn_spells_ << "\n";

    // tell it to drop some spells if its hand is overflown
    drawn_spells_ << "[drop];";
    int nrOverflow = inPuppet->nrSpellsInHand() - mMaxSpellsInHand;
    std::cout << "Puppet has " << inPuppet->nrSpellsInHand() << " spells in hand, an overflow of= " << nrOverflow << "\n";
    if (nrOverflow > 0) {
      drawn_spells_ << nrOverflow << "\n$" << inPuppet->getUID() << ";";
    } else
      drawn_spells_ << 0;

    Entity::spells_t const& lHand = inPuppet->getHand();
    while (inPuppet->nrSpellsInHand() > mMaxSpellsInHand) {
      Spell* lSpell = lHand.front();
      drawn_spells_ << lSpell->getUID() << ";";
      inPuppet->detachSpell(lSpell->getUID());

      lSpell = 0;
    }
    drawn_spells_ << "\n";

    log_->infoStream() << "sending drawn spells to Puppet " << inPuppet->getName();
    std::cout << "drawn spells:\n" << drawn_spells_.str() << "\n";

    // broadcast the data
    Event evt(EventUID::DrawSpells, EventFeedback::Ok, Event::NoFormat);
    evt.setProperty("Data", drawn_spells_.str());
    broadcast(evt);
	}

  Unit& instance::_create_unit(std::string model, Puppet& owner) {
    Unit* unit_ = rmgr_.getUnit(model, owner.getRace());
    assert(unit_);
    unit_->setUID(generate_uid());
    owner.attachUnit(unit_);

    Event evt(EventUID::CreateUnit);
    unit_->toEvent(evt);
    broadcast(evt);

    unit_ = 0;
  }
  void instance::_destroy_unit(int inUID) {
  }
  void instance::_destroy_unit(Unit&) {
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void instance::on_player_ready(const Event& inEvt) {

		// are all players ready?
		if ((++nr_ready_players_) == players_.size())
			start();

		return;
	}

  void instance::on_start_turn(const Event& inEvt) {
    // start timer and shizzle

    // send to all players except the active one
    Event evt(EventUID::TurnStarted);
    evt.setProperty("Puppet", stringify(active_puppet_->getUID()));

    for (auto player : players_) {
      if (player == active_player_)
        continue;

      send(player, evt);
    }

    log_->debugStream() << "it's now " << active_puppet_->getName() << "'s turn";
  }

	void instance::on_end_turn(const Event& inEvt) {
    // validate: make sure the sender is the active puppet
    if (get_sender(inEvt)->get_puppet() != active_puppet_) {
      log_->errorStream()
        << get_sender(inEvt)->get_puppet()->getName()
        << " is trying to end his opponent's turn!";
      return;
    }

		// is any of its units charging?
    if (!attackers_.empty()) {
			// toggle into Charging state
			// and toggle opponent into Blocking state
      Event e(EventUID::StartBlockPhase);
      broadcast(e);
      return;
    }

		// otherwise, just start the opponent's turn
		log_->debugStream()
		<< "ending " << active_puppet_->getName() << "'s turn ";

    // find the next puppet
    if (active_puppet_ == puppets_.back())
      active_puppet_ = puppets_.front();
    else {
      for (puppets_t::const_iterator puppet = puppets_.begin(); puppet != puppets_.end(); ++puppet) {
        if ((*puppet) == active_puppet_) {
          ++puppet;
          waiting_puppet_ = active_puppet_;
          active_puppet_ = (*puppet);
          break;
        }
      }
      /*bool found = false;
      for (auto puppet : puppets_) {
        if (found)
          active_puppet_ = puppet;
          break;
        if (puppet == active_puppet_)
          found = true;
      }*/
    }

    waiting_player_ = active_player_;
    active_player_ = get_player(active_puppet_);

		log_->debugStream()
		<< "starting " << active_puppet_->getName() << "'s turn ";

		Event evt(EventUID::StartTurn);
    send(active_player_, evt);
    this->draw_spells(active_puppet_);

		return;
	}

	void instance::on_cast_spell(const Event& inEvt) {
    if (!inEvt.hasProperty("Spell")) {
      Event evt(inEvt);
      reject(evt);
      return;
    }

		// find the spell object
    Spell* lSpell;
		try {
      lSpell = get_spell(convertTo<int>(inEvt.getProperty("Spell")));
    } catch (invalid_uid& e) {
      // reject the event
      log_->errorStream() << "couldn't find requested Spell with id " << inEvt.getProperty("Spell");
      Event evt(inEvt);
		  reject(evt);
      return;
    }

		Entity* lCaster = lSpell->getCaster();

    assert(lCaster && lSpell);

    log_->debugStream() << "spell cast: " << lSpell->getUID() << "#" << lSpell->getName();
    log_->debugStream() << "caster: " << lCaster->getUID() << "#" << lCaster->getName();
		/*if (!lSpell) {
		  lSpell = mWaitingPuppet->get_spell(lSpellId);
		  lCaster = mWaitingPuppet;
		}*/

		// dispatch to Lua
		lua_getfield(lua_, LUA_GLOBALSINDEX, "processSpell");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua event processor!";
			lua_pop(lua_,1);
			return;
		}

		tolua_pushusertype(lua_,(void*)lCaster,"Pixy::Entity");
		tolua_pushusertype(lua_,(void*)lSpell,"Pixy::Spell");
		tolua_pushusertype(lua_,(void*)&inEvt,"Pixy::Event");
		try {
			lua_call(lua_, 3, 1);
		} catch (std::exception& e) {
			log_->errorStream() << "Lua Handler: " << e.what();
		}

		bool result = lua_toboolean(lua_, lua_gettop(lua_));

		lua_remove(lua_, lua_gettop(lua_));
    if (result) {
      lCaster->detachSpell(lSpell->getUID());
    }

    lSpell = 0;
    lCaster = 0;

		//return result;
	}

  void instance::on_charge(const Event& evt) {
    // verify the existence of the unit and that its the owner's turn
    bool valid = true;
    Unit *_unit = 0;
    try {
      _unit = active_puppet_->getUnit(convertTo<int>(evt.getProperty("UID")));
    } catch (invalid_uid &e) {
      valid = false;
    }

    assert(valid); // __DEBUG__

    // make sure the unit is not resting
    valid = !_unit->isResting();

    assert(valid); // __DEBUG__

    // finally make sure the unit has not already been flagged for attacking
    // (this really shouldn't happen)
    if (!attackers_.empty())
      for (Unit* unit : attackers_)
        assert(unit != _unit);

    attackers_.push_back(_unit);

    Event e(EventUID::Charge, EventFeedback::Ok);
    e.setProperty("UID", _unit->getUID());
    broadcast(e);

    log_->debugStream() << _unit->getUID() << _unit->getName() << " is charging";
  }

  void instance::on_cancel_charge(const Event& evt) {

    // verify the existence of the unit and that its the owner's turn
    Unit *_unit = 0;
    try {
      _unit = active_puppet_->getUnit(convertTo<int>(evt.getProperty("UID")));
    } catch (invalid_uid &e) {
    }

    assert(_unit); // __DEBUG__

    // make sure the unit was charging (this shouldn't go false)
    assert(!attackers_.empty());

    bool _found = false;
    for (Unit* unit : attackers_)
      if (unit == _unit) {
        _found = true;
        break;
      }

    assert(_found);

    attackers_.remove(_unit);

    Event e(EventUID::CancelCharge, EventFeedback::Ok);
    e.setProperty("UID", _unit->getUID());
    broadcast(e);

    log_->debugStream() << _unit->getUID() << _unit->getName() << " is no longer charging";
  }

  void instance::on_block(const Event& evt) {
    Unit *attacker, *blocker = 0;

    assert(evt.hasProperty("AUID") && evt.hasProperty("BUID"));
    try {
      attacker = active_puppet_->getUnit(convertTo<int>(evt.getProperty("AUID")));
      blocker = waiting_puppet_->getUnit(convertTo<int>(evt.getProperty("BUID")));
    } catch (invalid_uid& e) {
      log_->errorStream() << "invalid block event parameters : " << e.what();
    }

    assert(attacker && blocker);

    // __DEBUG__ : make sure the attacker is charging
    assert(!attackers_.empty());

    bool valid = false;
    for (auto unit : attackers_)
      if (unit == attacker) {
        valid = true;
        break;
      }

    assert(valid);

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
            valid = false;
            break;
          }

      if (!_valid) {
        log_->errorStream() << "blocker " << blocker->getUID() << " already marked for blocking a unit";
        return;
      }

      // mark it
      itr->second.push_back(blocker);
    }

    Event e(EventUID::Block, EventFeedback::Ok);
    e.setProperty("AUID", attacker->getUID());
    e.setProperty("BUID", blocker->getUID());
    broadcast(e);
  }

  void instance::on_cancel_block(const Event& evt) {
    // ...
  }
  void instance::on_end_block_phase(const Event& evt) {
    log_->debugStream()
      << "starting battle, I have " << attackers_.size()
      << " attackers and " << blockers_.size () << " units being blocked";

    // 1) tell the clients to start rendering the combat based on what they received
    // so far
    broadcast(evt);

    // 2) calculate the combat outcome
    // ...
    log_->debugStream() << "calculating battle results";

    // clear combat temps
    attackers_.clear();
    blockers_.clear();

    log_->debugStream() << "waiting for the blocker to start their turn";
    // 3) re-start the on_end_turn routine
    Event tmp(EventUID::EndTurn);
    tmp.Sender = active_player_;
    on_end_turn(tmp);

    // 4) wait for the blocker to acknowledge and start the new turn
    // (nothing to do)
  }
}
}
