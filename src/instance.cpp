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
namespace Pixy {
namespace Net {

	instance::instance(players_t in_players, boost::asio::io_service& io_service)
  : players_(in_players),
    dispatcher_(io_service),
    strand_(io_service),
    active_puppet_(),
    active_player_()
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
		lua_getfield(lua_, LUA_GLOBALSINDEX, "registerinstance");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua event processor!";
			lua_pop(lua_,1);
			return;
		}

		tolua_pushusertype(lua_,(void*)this,"Pixy::instance");
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
    return spells_.find(inUID)->second;
  }

  Unit* instance::get_unit(int inUID) {
    return units_.find(inUID)->second;
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
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Main Routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void instance::start() {

		log_->debugStream() << "players are ready, starting turn";

    // start the first player's turn, and track them
		active_puppet_ = puppets_.front();
    active_player_ = get_player(active_puppet_);

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

    Puppet::hand_t const& lHand = inPuppet->getHand();
    while (inPuppet->nrSpellsInHand() > mMaxSpellsInHand) {
      Spell* lSpell = lHand.front();
      drawn_spells_ << lSpell->getUID() << ";";
      inPuppet->detachSpell(lSpell);
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
			// if yes, toggle into Charging state
			// and toggle opponent into Blocking state

		// otherwise, just start the opponent's turn
		log_->debugStream()
		<< "ending " << active_puppet_->getName() << "'s turn ";

    // find the next puppet
    if (active_puppet_ == puppets_.back())
      active_puppet_ = puppets_.front();
    else {
      bool found = false;
      for (auto puppet : puppets_) {
        if (found)
          active_puppet_ = puppet;
          break;
        if (puppet == active_puppet_)
          found = true;
      }
    }

    active_player_ = get_player(active_puppet_);

		log_->debugStream()
		<< "starting " << active_puppet_->getName() << "'s turn ";

		Event evt(EventUID::StartTurn);
    send(active_player_, evt);
    this->draw_spells(active_puppet_);

		return;
	}

	void instance::on_cast_spell(const Event& inEvt) {
		// dispatch to Lua
		lua_getfield(lua_, LUA_GLOBALSINDEX, "processSpell");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua event processor!";
			lua_pop(lua_,1);
			return;
		}

    if (!inEvt.hasProperty("Spell")) {
      Event evt(inEvt);
      reject(evt);
      return;
    }

		// find the spell object
		int lSpellId = convertTo<int>(inEvt.getProperty("Spell"));
		const Spell* lSpell = get_spell(lSpellId);
		Entity* lCaster = lSpell->getCaster();

		/*if (!lSpell) {
		  lSpell = mWaitingPuppet->get_spell(lSpellId);
		  lCaster = mWaitingPuppet;
		}*/

		if (!lSpell || !lCaster) {
		  log_->errorStream() << "couldn't find requested Spell with id " << lSpellId;
      Event evt(inEvt);
		  reject(evt);
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

		//return result;
	}
}
}
