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
#include "PixyLua.h"
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
    winner_(),
    rmgr_(server::singleton().get_resmgr())
  {
		uuid_ = boost::uuids::random_generator()();

		started_ = false;
    in_battle_ = false;
    finished_ = false;
		nr_ready_players_ = 0;
    nr_battle_acks_ = 0;
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
    death_list_.clear();
    puppets_.clear();


    winner_.reset();
    active_puppet_.reset();
    waiting_puppet_.reset();

    active_player_.reset();
    waiting_player_.reset();

    players_.clear();

		if (lua_)
			lua_close(lua_);

		log_->infoStream() << "instance shut down";
		if (lua_log_) {
			delete lua_log_;
	  }
		if (log_) {
			delete log_;
    }
	}

  bool instance::operator==(instance const& rhs) {
    return (uuid_ == rhs.uuid_);
  }

  void instance::bootstrap() {
    for (auto player : players_)
      subscribe(player);

		init_lua();

		bind_handlers();

		// prepare the puppets stream that will be sent on SyncPuppetData
		create_puppets();

		log_->infoStream() << "a match is beginning";

    strand_.post([&]() -> void {
      Event e(EventUID::MatchFound);
      broadcast(e);
    });

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

  lua_State* instance::_get_lua() const
  {
    return lua_;
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

    throw invalid_uid(std::string("couldn't find a puppet with uid " + stringify(inUID)));
    //return NULL;
  }
  Puppet* instance::get_enemy(int inUID)
  {
    for (auto puppet : puppets_)
      if (puppet->getUID() != inUID)
        return puppet.get();

    throw invalid_uid(std::string("couldn't find enemy puppet of " + stringify(inUID)));
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
    for (auto puppet : puppets_)
      for (auto unit : puppet->getUnits())
        if (unit->getUID() == inUID)
          return unit;

    throw invalid_uid(std::string("couldn't find a unit with uid " + stringify(inUID)));
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
    dispatcher_.bind(EventUID::Unassigned, this, &instance::pass_evt_to_lua);
    dispatcher_.bind(EventUID::SyncMatchPuppets, this, &instance::on_sync_match_puppets);
    dispatcher_.bind(EventUID::Ready, this, &instance::on_player_ready);
    dispatcher_.bind(EventUID::StartTurn, this, &instance::on_start_turn);
    dispatcher_.bind(EventUID::EndTurn, this, &instance::on_end_turn);
		dispatcher_.bind(EventUID::CastSpell, this, &instance::on_cast_spell);

    dispatcher_.bind(EventUID::Charge, this, &instance::on_charge);
    dispatcher_.bind(EventUID::CancelCharge, this, &instance::on_cancel_charge);
    dispatcher_.bind(EventUID::Block, this, &instance::on_block);
    dispatcher_.bind(EventUID::CancelBlock, this, &instance::on_cancel_block);
    dispatcher_.bind(EventUID::EndBlockPhase, this, &instance::on_end_block_phase);
    dispatcher_.bind(EventUID::BattleOver, this, &instance::on_battle_over);

    dispatcher_.bind(EventUID::PlayerDroppedOut, this, &instance::on_remove_player);
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

    for (auto puppet : puppets_) {
      puppet->live();
      draw_spells(puppet, 4);
    }

		started_ = true;
	}

  void instance::pass_evt_to_lua(const Event& evt)
  {
    // pass to lua

    lua_getfield(lua_, LUA_GLOBALSINDEX, "processEvt");
    if(!lua_isfunction(lua_, 1))
    {
      log_->errorStream() << "could not find Lua event processor!"
        << " event: " << Event::_uid_to_string(evt.UID) << "#" << (int)evt.UID;
      lua_pop(lua_,1);
      return;
    }

    tolua_pushusertype(lua_,(void*)&evt,"Pixy::Event");
    lua_call(lua_, 1, 1);
    int result = lua_toboolean(lua_, lua_gettop(lua_));
    lua_remove(lua_, lua_gettop(lua_));

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

    ((Player*)player.get())->set_in_lobby(false);

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

    server::singleton().get_resmgr().puppets_to_stream(puppets_stream_, lPuppets);
	}

  void instance::enqueue(const Event& evt, player_cptr sender) {
    if (!running_)
      return;

    // if a battle is going on, reject any non-battle events (see EventUID in Event.hpp)
    if (in_battle_ && (evt.UID <= EventUID::MarkBattleEvents || evt.UID >= EventUID::EndMarkBattleEvents))
      return;

    assert(evt.Sender);
    //~ std::cout << "instance: got evt from " << evt.Sender->get_username() << "\n";
    dispatcher_.deliver(evt);
  }

	const int instance::generate_uid() {
	  return ++uid_generator_;
	}

  void instance::on_dropout(player_cptr in_player) {
    // remove the dead player
    ((Player*)in_player.get())->leave_instance();
    players_.remove(in_player);
    in_player.reset();
    strand_.post([&]() {

      // drop the other players out
      Event e(EventUID::PlayerDroppedOut);
      broadcast(e);
      for (auto player_ : players_)
      {
        ((Player*)player_.get())->leave_instance();
        player_.reset();
      }
      players_.clear();
      server::singleton()._shutdown_instance(shared_from_this());
    });
    /*strand_.post(
      [&, player]() {
      log_->infoStream()
        << "detaching player "
        << player->get_username()
        << " from instance";

      if (active_player_ == player)
        active_player_.reset();

      ((Player*)player.get())->leave_instance();

      // TODO: ask opponents if they'd like to wait for player to rejoin
      players_.remove(player);

      if (players_.empty())
        server::singleton()._shutdown_instance(shared_from_this());
      else {
        Event e(EventUID::PlayerDroppedOut);
        broadcast(e);
      }
    });*/

    running_ = false;
  }

  void instance::on_remove_player(const Event& e)
  {
    on_dropout(get_sender(e));
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

      pass_to_lua("onDrawSpell", 2, "Pixy::Puppet", inPuppet.get(), "Pixy::Spell", lSpell);

      drawn_spells_ << lSpell->getName() << ";" << lSpell->getUID() << ";";

      lSpell = 0;
		}

    drawn_spells_ << "\n";

    // tell it to drop some spells if its hand is overflown
    drawn_spells_ << "[drop];";
    int nrOverflow = inPuppet->nrSpellsInHand() - mMaxSpellsInHand;
    //~ std::cout << "Puppet has " << inPuppet->nrSpellsInHand() << " spells in hand, an overflow of= " << nrOverflow << "\n";
    if (nrOverflow > 0) {
      drawn_spells_ << nrOverflow << "\n$" << inPuppet->getUID() << ";";
    } else
      drawn_spells_ << 0;

    Entity::spells_t const& lHand = inPuppet->getHand();
    while (inPuppet->nrSpellsInHand() > mMaxSpellsInHand) {
      Spell* lSpell = lHand.front();
      drawn_spells_ << lSpell->getUID() << ";";

      pass_to_lua("onDropSpell", 2, "Pixy::Puppet", inPuppet.get(), "Pixy::Spell", lSpell);

      inPuppet->detachSpell(lSpell->getUID());

      lSpell = 0;
    }
    drawn_spells_ << "\n";

    //~ log_->infoStream() << "sending drawn spells to Puppet " << inPuppet->getName();
    std::cout << "drawn spells:\n" << drawn_spells_.str() << "\n";

    // broadcast the data
    Event evt(EventUID::DrawSpells, EventFeedback::Ok, Event::NoFormat);
    evt.setProperty("Data", drawn_spells_.str());
    broadcast(evt);
	}

  void instance::_draw_spells(int inPuppetUID, int inNrSpells)
  {
    this->draw_spells(get_puppet(inPuppetUID), inNrSpells);
  }

  Unit* instance::_create_unit(std::string model, Puppet& owner) {
    if (owner.getUnits().size() >= 10)
      return 0;

    Unit* unit_ = rmgr_.getUnit(model, owner.getRace());
    assert(unit_);
    unit_->setUID(generate_uid());
    owner.attachUnit(unit_);

    Event evt(EventUID::CreateUnit);
    unit_->toEvent(evt);
    broadcast(evt);

    log_->debugStream() << "creating unit named " << model
      << ", owner now has " << owner.getUnits().size() << " units under control";

    unit_->live();

    //unit_ = 0;
    return unit_;
  }
  void instance::_destroy_unit(int inUID) {
    Unit* unit = 0;
    for (auto puppet : puppets_)
    {
      try {
        unit = puppet->getUnit(inUID);
        //~ return death_list_.push_back(unit);
        strand_.post([&, unit, puppet, inUID]() -> void {
          puppet->detachUnit(inUID);

          // tell clients to remove this unit
          Event e(EventUID::RemoveUnit, EventFeedback::Ok);
          e.setProperty("UID", inUID);
          broadcast(e);

          log_->debugStream() << "unit with UID " << inUID << " is dead";
        });
        return;
      } catch (invalid_uid& e) { }
    }

    // can't be here
    assert(false);
  }
  void instance::_destroy_unit(Unit* inUnit) {
    // tell clients to remove this unit
    Event e(EventUID::RemoveUnit, EventFeedback::Ok);
    e.setProperty("UID", inUnit->getUID());
    broadcast(e);
    static_cast<Puppet*>(inUnit->getOwner())->detachUnit(inUnit->getUID());
  }

  bool instance::pass_to_lua(const char* inFunc, int argc, ...) {
    va_list argp;
    va_start(argp, argc);

		lua_getfield(lua_, LUA_GLOBALSINDEX, "arbitrary");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua arbitrary functor!";
			lua_pop(lua_,1);
			return true;
		}

    lua_pushfstring(lua_, inFunc);
    for (int i=0; i < argc; ++i) {
      const char* argtype = (const char*)va_arg(argp, const char*);
      void* argv = (void*)va_arg(argp, void*);
      tolua_pushusertype(lua_,argv,argtype);
    }

		try {
			lua_call(lua_, argc+1, 1);
		} catch (std::exception& e) {
			log_->errorStream() << "Lua Handler: " << e.what();
		}

		bool result = lua_toboolean(lua_, lua_gettop(lua_));
		lua_remove(lua_, lua_gettop(lua_));

    va_end(argp);
    return result;
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

  void instance::on_sync_match_puppets(const Event& e)
  {
    Event evt(EventUID::SyncMatchPuppets, EventFeedback::Ok, Event::NoFormat);
    evt.setProperty("Data", puppets_stream_.str());
    send(e.Sender, evt);
  }
	void instance::on_player_ready(const Event& inEvt) {
    log_->debugStream() << "received ready ack from " << inEvt.Sender->get_username();

		// are all players ready?
		if ((++nr_ready_players_) == players_.size())
			start();

		return;
	}

  void instance::on_start_turn(const Event& inEvt) {
    // start timer and shizzle

    pass_to_lua("tick_turn", 0);

    // send to all players except the active one
    Event evt(EventUID::TurnStarted);
    evt.setProperty("Puppet", stringify(active_puppet_->getUID()));

    for (auto player : players_) {
      if (player == active_player_)
        continue;

      send(player, evt);
    }

    // update the hero's channels and willpower
    {
      pass_to_lua("tick_resources", 1, "Pixy::Puppet", active_puppet_.get());
      /*
      active_puppet_->setChannels(active_puppet_->getChannels() + 1);
      active_puppet_->setWP(active_puppet_->getChannels());

      Event evt(EventUID::UpdatePuppet, EventFeedback::Ok);
      evt.setProperty("UID", active_puppet_->getUID());
      evt.setProperty("Channels", active_puppet_->getChannels());
      evt.setProperty("WP", active_puppet_->getWP());
      broadcast(evt);
      */
    }

    // apply all the buffs on the puppet and its units
    log_->debugStream()
    << "active puppet " << active_puppet_->getName() << " has "
    << active_puppet_->getBuffs().size() << " buffs";

    Event dummy(EventUID::Unassigned);
    for (auto buff : active_puppet_->getBuffs()) {
      log_->debugStream() << "applying buff " << buff->getName() << "#" << buff->getUID();
      lua_getfield(lua_, LUA_GLOBALSINDEX, "process_spell");

      tolua_pushusertype(lua_,(void*)buff->getCaster(),"Pixy::Puppet");
      tolua_pushusertype(lua_,(void*)buff->getTarget(),"Pixy::Entity");
      tolua_pushusertype(lua_,(void*)buff,"Pixy::Spell");
      tolua_pushusertype(lua_,(void*)&dummy,"Pixy::Event");
      try {
        lua_call(lua_, 4, 1);
      } catch (std::exception& e) {
        log_->errorStream() << "Lua Handler: " << e.what();
      }
      lua_remove(lua_, lua_gettop(lua_));

      // if a puppet is dead, the game is over
      if (active_puppet_->isDead())
      {
        return _destroy_puppet(active_puppet_->getUID());
      }
      // if the debuff's target was a puppet and it died, game is also over
      else if (buff->getTarget()->getRank() == Pixy::PUPPET
        && buff->getTarget()->isDead())
      {
        return _destroy_puppet(buff->getTarget()->getUID());
      }
      else if (buff->getTarget()->isDead())
        death_list_.push_back((Unit*)buff->getTarget());
    }

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

    // kill all the units in the death list
    for (auto unit : death_list_)
      _destroy_unit(unit);
      //static_cast<Puppet*>(unit->getOwner())->detachUnit(unit->getUID());

    death_list_.clear();

    // un-rest all the player's resting units
    for (auto unit : active_puppet_->getUnits()) {
      unit->getUp();

      // apply the active buffs
      for (auto buff : unit->getBuffs()) {
        lua_getfield(lua_, LUA_GLOBALSINDEX, "process_spell");

        tolua_pushusertype(lua_,(void*)buff->getCaster(),"Pixy::Unit");
        tolua_pushusertype(lua_,(void*)buff->getTarget(),"Pixy::Entity");
        tolua_pushusertype(lua_,(void*)buff,"Pixy::Spell");
        tolua_pushusertype(lua_,(void*)&dummy,"Pixy::Event");
        try {
          lua_call(lua_, 4, 1);
        } catch (std::exception& e) {
          log_->errorStream() << "Lua Handler: " << e.what();
        }
        lua_remove(lua_, lua_gettop(lua_));

        // check if either the caster or the target of the buff are dead
        if (buff->getTarget()->isDead()) {
          // if the target was a puppet and it died, game is over
          if (buff->getTarget()->getRank() == Pixy::PUPPET)
          {
            return _destroy_puppet(buff->getTarget()->getUID());
          }
          else // just mark the unit for death
            death_list_.push_back((Unit*)buff->getTarget());
        }
        else if (unit->isDead())
          death_list_.push_back(unit);
      }

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

    }

    // again, kill all the units in the death list
    for (auto unit : death_list_)
      _destroy_unit(unit);
      //static_cast<Puppet*>(unit->getOwner())->detachUnit(unit->getUID());

    death_list_.clear();

    // charge with all the Restless units
    {
      for (auto unit : active_puppet_->getUnits())
        if (unit->isRestless()) {
          Event e(EventUID::Charge, EventFeedback::Ok);
          e.setProperty("UID", unit->getUID());
          on_charge(e);
        }
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

    // get up all the opponent units with summoning sickness
    for (auto unit : waiting_puppet_->getUnits())
      if (unit->hasSummoningSickness())
        unit->getUp();

		// is any of its units charging?
    if (!attackers_.empty()) {
      in_battle_ = true;
      nr_battle_acks_ = 0;
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
    waiting_puppet_ = active_puppet_;
    if (active_puppet_ == puppets_.back())
      active_puppet_ = puppets_.front();
    else {
      for (puppets_t::const_iterator puppet = puppets_.begin(); puppet != puppets_.end(); ++puppet) {
        if ((*puppet) == active_puppet_) {
          ++puppet;
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

    Entity* lTarget = 0;
    if (inEvt.hasProperty("T")) {
      try {
        // is the target a puppet?
        lTarget = get_puppet(convertTo<int>(inEvt.getProperty("T"))).get();
        log_->debugStream() << "target: " << lTarget->getUID() << "#" << lTarget->getName();
      } catch (invalid_uid& e) {
        try {
          // a unit?
          lTarget = get_unit(convertTo<int>(inEvt.getProperty("T")));
          log_->debugStream() << "target: " << lTarget->getUID() << "#" << lTarget->getName();
        } catch (invalid_uid& e) {
          // invalid UID
          log_->errorStream() << "couldn't find spell target with id " << inEvt.getProperty("T");
          Event evt(inEvt);
          reject(evt);
          return;
        }
      }

      assert(lTarget);
      lSpell->setTarget(lTarget);
    } else {
      // if the spell requires a target, it must be given
#ifdef PARANOID
      assert(!lSpell->requiresTarget());
#else
      // gracefully reject the event
      if (lSpell->requiresTarget())
      {
        log_->errorStream()
          << "an invalid spell request#" << lSpell->getUID()
          << "; target is required but not given";
        Event e(inEvt);
        return reject(e);
      }
#endif
      // otherwise, the spell's target is the caster itself
      lSpell->setTarget(lCaster);
    }

    // verify the caster having enough resources to cast the spell
    {
      bool valid = true;
      if (lCaster->getRank() == PUPPET)
      {
        if (lSpell->getCostWP() > ((Puppet*)lCaster)->getWP())
          valid = valid && false;

        // heroes can't have less than 1 channel
        if (lSpell->getCostChannels() >= ((Puppet*)lCaster)->getChannels())
          valid = valid && false;
      }

      if (lSpell->getCostHP() > lCaster->getHP())
        valid = valid && false;


      if (!valid)
      {
        if (lCaster->getRank() == PUPPET)
        {
          Puppet* tCaster = (Puppet*)lCaster;
          log_->errorStream()
            << "caster" << tCaster->getUID()
            << " failed the resources requirements of the spell" << lSpell->getUID()
            << " : \t "
            << lSpell->getCostWP() << ":" << lSpell->getCostHP() << ":" << lSpell->getCostChannels()
            << " vs "
            << tCaster->getWP() << ":" << tCaster->getHP() << ":" << tCaster->getChannels();
        }

        Event e(inEvt);
        return reject(e);
      }
    }

    // prepare the response event
    Event resp(inEvt);
    resp.setProperty("Spell", lSpell->getUID());
    resp.setProperty("C", lSpell->getCaster()->getUID());
    if (lSpell->requiresTarget())
      resp.setProperty("T", lSpell->getTarget()->getUID());

		// dispatch to Lua
		lua_getfield(lua_, LUA_GLOBALSINDEX, "process_spell");
		if(!lua_isfunction(lua_, 1))
		{
			log_->errorStream() << "could not find Lua event processor!";
			lua_pop(lua_,1);
      Event e(inEvt);
      return reject(e);
		}

    log_->debugStream() << "\t things are looking good, passing to lua: "
      << ", cost: " << lSpell->getCostWP() << ":" << lSpell->getCostHP() << ":"
      << lSpell->getCostChannels();

		tolua_pushusertype(lua_,(void*)lCaster,"Pixy::Entity");
    tolua_pushusertype(lua_,(void*)lTarget,"Pixy::Entity");
		tolua_pushusertype(lua_,(void*)lSpell,"Pixy::Spell");
		tolua_pushusertype(lua_,(void*)&inEvt,"Pixy::Event");
		try {
			lua_call(lua_, 4, 1);
		} catch (std::exception& e) {
			log_->errorStream() << "Lua Handler: " << e.what();
		}

		bool result = lua_toboolean(lua_, lua_gettop(lua_));

		lua_remove(lua_, lua_gettop(lua_));

    log_->debugStream() << "\t back from lua: "
      << ", cost: " << lSpell->getCostWP() << ":" << lSpell->getCostHP() << ":"
      << lSpell->getCostChannels();

    // if the spell cast was successful, we first broadcast the command to
    // the clients, then detach the spell from the caster, and finally
    // we apply any resource changes to the caster and broadcast them too
    if (result) {

      // broadcast the CastSpell event to players, confirming it
      {
        resp.Feedback = EventFeedback::Ok;
        broadcast(resp);
      }

      // update the caster stats and broadcast them
      {
        Event evt(EventUID::Unassigned, EventFeedback::Ok);
        evt.setProperty("UID", lCaster->getUID());

        if (lCaster->getRank() == PUPPET)
        {
          Puppet* tCaster = static_cast<Puppet*>(lCaster);
          evt.UID = EventUID::UpdatePuppet;
          // apply WP cost, if any
          if (lSpell->getCostWP() > 0) {
            tCaster->setWP(tCaster->getWP() - lSpell->getCostWP());
            evt.setProperty("WP", tCaster->getWP());
            log_->debugStream()
              << tCaster->getName() << " paid " << lSpell->getCostWP() << " wp,"
              << " and now has " << tCaster->getWP() << " wp.";
          }
          // apply the Channels cost, if any
          if (lSpell->getCostChannels() > 0) {
            tCaster->setChannels(tCaster->getChannels() - lSpell->getCostChannels());
            evt.setProperty("Channels", tCaster->getChannels());
            log_->debugStream()
              << tCaster->getName() << " paid " << lSpell->getCostChannels() << " channels,"
              << " and now has " << tCaster->getChannels() << " channels.";
          }
        } else
          evt.UID = EventUID::UpdateUnit;

        // apply HP cost, if any
        if (lSpell->getCostHP() > 0) {
          lCaster->setHP(lCaster->getHP() - lSpell->getCostHP());
          evt.setProperty("HP", lCaster->getHP());
          log_->debugStream()
              << lCaster->getName() << " paid " << lSpell->getCostHP() << " hp,"
              << " and now has " << lCaster->getHP() << " hp.";
        }

        broadcast(evt);
      }

      // don't delete the spell object if it's a buff
      lCaster->detachSpell(lSpell->getUID(), lSpell->getDuration() == 0);

    } else {
      // we reject the request
      //Event e(inEvt);
      resp.Feedback = EventFeedback::Error;
      return reject(resp);
    }

    lSpell = 0;
    lCaster = 0;
    lTarget = 0;

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

    std::cout
      << "attempting to find unit with UID : " << evt.getProperty("UID")
      << " and active puppet " << active_puppet_->getUID() << "#" << active_puppet_->getName()
      << " has: \n";
    for (auto unit : active_puppet_->getUnits())
      std::cout << "\t+" << unit->getUID() << "#" << unit->getName() << "\n";


    if (!valid) {
      log_->errorStream() << "invalid charge attempt, uid: " << evt.getProperty("UID");
      return;
    }

    //assert(valid); // __DEBUG__

    // make sure the unit is not resting
    valid = !_unit->isResting();

    assert(valid); // __DEBUG__

    if (_unit->getBaseAP() <= 0)
      return;

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
    assert(!_unit->isRestless()); // Restless units can't not charge

    attackers_.remove(_unit);

    Event e(EventUID::CancelCharge, EventFeedback::Ok);
    e.setProperty("UID", _unit->getUID());
    broadcast(e);

    log_->debugStream() << _unit->getUID() << _unit->getName() << " is no longer charging";
  }

  void instance::on_block(const Event& evt) {
    Unit *attacker, *blocker = 0;

    assert(evt.hasProperty("A") && evt.hasProperty("B"));
    try {
      attacker = active_puppet_->getUnit(convertTo<int>(evt.getProperty("A")));
      blocker = waiting_puppet_->getUnit(convertTo<int>(evt.getProperty("B")));
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

    // is the blocker resting? is the attacker unblockable?
    valid = !blocker->isResting() && !attacker->isUnblockable();

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

    log_->infoStream() << blocker->getUID() << " is blocking " << attacker->getUID();

    Event e(EventUID::Block, EventFeedback::Ok);
    e.setProperty("A", attacker->getUID());
    e.setProperty("B", blocker->getUID());
    broadcast(e);
  }

  void instance::on_cancel_block(const Event& evt) {
    Unit *blocker = 0;
    try {
      blocker = waiting_puppet_->getUnit(convertTo<int>(evt.getProperty("UID")));
    } catch (invalid_uid &e) {
    }

    assert(blocker);
    assert(!blockers_.empty());
    // find the blocker's target

    Unit *attacker = 0;
    for (auto pair : blockers_)
      for (auto _blocker : pair.second)
        if (_blocker == blocker) {
          attacker = pair.first;
          break;
        }

    assert(attacker);
    blockers_.find(attacker)->second.remove(blocker);

    Event e(EventUID::CancelBlock, EventFeedback::Ok);
    e.setProperty("B", blocker->getUID());
    e.setProperty("A", attacker->getUID());
    broadcast(e);

  }
  void instance::on_end_block_phase(const Event& evt) {
    log_->debugStream()
      << "starting battle, I have " << attackers_.size()
      << " attackers and " << blockers_.size () << " units being blocked";

    // 1) tell the clients to start rendering the combat based on what they received
    // so far
    broadcast(evt);

    // 2) calculate the combat outcome
    /*
     * for all unit X in attackers_ do:
     *  for all blocker Y for X in blockers_ do:
     *    - attack(X,Y)
     *    - if X is dead, break and get next X
     *    - if Y is dead, get next Y
     * for all dead unit Z do:
     *  - detach from puppet
     */
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
      } else { // no blockers
        std::cout
          << "attacker " << attacker->getUID()
          << " is attacking puppet " << waiting_puppet_->getName()
          << " which has " << waiting_puppet_->getHP();
        attacker->attack(waiting_puppet_.get());
        std::cout
          << " and is dead? " << (waiting_puppet_->isDead() ? "yes" : "no") << "\n";
        if (waiting_puppet_->isDead()) {
          // game over
          return finish(active_puppet_);
        }
      }
    }
    log_->debugStream() << "calculating battle results";

    // rest what's left of the attackers
    for (auto unit : attackers_)
      unit->rest();

    // clean up dead units
    log_->debugStream() << death_list_.size() << " dead units";
    for (auto unit : death_list_)
      _destroy_unit(unit);
      //static_cast<Puppet*>((Entity*)unit->getOwner())->detachUnit(unit->getUID());

    // clear combat temps
    death_list_.clear();
    attackers_.clear();
    blockers_.clear();

    log_->debugStream() << "waiting for the blocker to start their turn";
    // 3) re-start the on_end_turn routine
    //~ Event tmp(EventUID::EndTurn);
    //~ tmp.Sender = active_player_;
    //~ on_end_turn(tmp);

    // 4) wait for the blocker to acknowledge and start the new turn
    // (nothing to do)
  }

  void instance::on_battle_over(const Event& evt) {
    ++nr_battle_acks_;
    if (nr_battle_acks_ == players_.size()) {
      in_battle_ = false;

      // match is over
      if (waiting_puppet_->isDead()) {
        winner_ = active_puppet_;
        log_->infoStream() << "match is over, winner is " << winner_->getName();

        Event evt(EventUID::MatchFinished);
        evt.setProperty("W", winner_->getUID());
        broadcast(evt);

        running_ = false;
      }
      else {
        Event tmp(EventUID::EndTurn);
        tmp.Sender = active_player_;
        on_end_turn(tmp);
      }
    }
  }

  void instance::finish(puppet_ptr inWinner) {

    winner_ = inWinner;
    finished_ = true;

    // clean up
    death_list_.clear();
    attackers_.clear();
    blockers_.clear();
  }

  void instance::_destroy_puppet(int inUID) {
    puppet_ptr _winner, _loser;

    for (auto puppet : puppets_)
      if (puppet->getUID() == inUID) {
        _loser = puppet;
      } else
        _winner = puppet;

    assert(_winner && _loser);

    winner_ = _winner;
    log_->infoStream() << "match is over, winner is " << winner_->getName();

    Event evt(EventUID::MatchFinished);
    evt.setProperty("W", winner_->getUID());
    broadcast(evt);

    running_ = false;
  }
}
}
