/*
 *  Instance.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "instance.hpp"
#include "Server.h"

#include "tolua++.h"

TOLUA_API int  tolua_EServer_open (lua_State* tolua_S);
TOLUA_API int  tolua_EShared_open (lua_State* tolua_S);
namespace Pixy {


	Instance::Instance(std::list<Player*> inPlayers) {
		mUUID = boost::uuids::random_generator()();

		fStarted = false;
		nrPlayersReady = 0;
		nrSpellsPerTurn = 2;
		mUIDGenerator = 0;

		mActivePlayer = 0;
    mActivePuppet = 0;

    mLua = 0;

		mLog = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "Instance");
		mLuaLog = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "Lua");

		mServer = Server::getSingletonPtr();
		mEvtMgr = EventManager::getSingletonPtr();

		// register our players
    list<Player*>::const_iterator itr;
    for (itr = inPlayers.begin(); itr != inPlayers.end(); ++itr)
      subscribe(*itr);

		//initLua();

		bindHandlers();

		// give them the puppets so they can populate them
		createPuppets();

		mLog->infoStream() << "a match is beginning";
	}

	Instance::~Instance() {
    while (!mPlayers.empty()) {
			mLog->infoStream()
			<< "detaching player "
			<< mPlayers.back()->getUsername()
			<< " from instance";
      delete mPlayers.back()->getPuppet();
      mPlayers.back()->setPuppet(0);
      mPlayers.back()->setInstance(0);
      mPlayers.pop_back();
    }

		mPuppets.clear();

		if (mLua)
			lua_close(mLua);

		mLog->infoStream() << "instance shut down";
		if (mLuaLog) {
			delete mLuaLog;
	  }
		if (mLog) {
			delete mLog;
    }

    mActivePlayer = 0;
    mActivePuppet = 0;
	}

  bool Instance::operator==(Instance const& rhs) {
    return (mUUID == rhs.mUUID);
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Lua
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void Instance::initLua() {
		mLog->infoStream() << "opening Lua state";
		// init lua state
		mLua = lua_open();

		luaL_openlibs(mLua);
		tolua_EServer_open(mLua);
    tolua_EShared_open(mLua);

		//char lFilePath[PATH_MAX];
		//sprintf(lFilePath,  "%s%s/server/pixy_server.lua", PROJECT_ROOT, PROJECT_SCRIPTS );
		std::string lFilePath = "../resources/scripts/pixy_server.lua";

		mLog->infoStream() << "using script '" << lFilePath << "'";

		int lErrorCode = luaL_dofile(mLua, lFilePath.c_str());
		if (lErrorCode == 1) {
			mLog->errorStream() << "Lua: " << lua_tostring(mLua, -1);
			lua_pop(mLua, -1);
		}

		// give Lua a handle of this instance
		lua_getfield(mLua, LUA_GLOBALSINDEX, "registerInstance");
		if(!lua_isfunction(mLua, 1))
		{
			mLog->errorStream() << "could not find Lua event processor!";
			lua_pop(mLua,1);
			return;
		}

		tolua_pushusertype(mLua,(void*)this,"Pixy::Instance");
		try {
			lua_call(mLua, 1, 1);
		} catch (std::exception& e) {
			mLog->errorStream() << "Lua Handler: " << e.what();
		}

		lua_toboolean(mLua, lua_gettop(mLua));
		lua_remove(mLua, lua_gettop(mLua));

		mLog->infoStream() << "Lua is up!";
	}

	void Instance::luaLog(std::string inMsg) {
		mLuaLog->infoStream() << inMsg;
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Misc
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	boost::uuids::uuid Instance::getUUID() const {
		return mUUID;
	}

	bool Instance::hasStarted() {
		return fStarted;
	}

	players_t const& Instance::getPlayers() const {
		return mPlayers;
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Helpers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	Puppet* Instance::getSender(Event* inEvt) {
		Player* lPlayer = mServer->getPlayer(inEvt->getSender());// mServer->getEventSender(inEvt);
		if (!lPlayer) {
			throw UnknownEventSender( "in Instance::getSender" );
			return false;
		}

    //inEvt->addHandler();

		/*
		 players_t::iterator lSeeker = mPlayers.begin();
		for (lSeeker; lSeeker != mPlayers.end(); ++lSeeker)
			if (*lSeeker == lPlayer)
				return lPlayer->getPuppet();
		*/
		if (!lPlayer || !lPlayer->getPuppet())
			throw new UnknownEventSender("Instance couldn't find event's player!");

		//inEvt->removeHandler();
		return lPlayer->getPuppet();
	}

  Puppet* Instance::getPuppet(int inUID) {
    puppets_t::const_iterator lPuppet;
    for (lPuppet = mPuppets.begin(); lPuppet != mPuppets.end(); ++lPuppet)
      if ((*lPuppet)->getObjectId() == inUID)
        return (*lPuppet);

    return 0;
  }

  Spell* Instance::getSpell(int inUID) {
    return mSpells.find(inUID)->second;
  }

  Unit* Instance::getUnit(int inUID) {
    return mUnits.find(inUID)->second;
  }

  Player* Instance::getPlayer(Puppet const *inPuppet) {
		players_t::const_iterator lPlayer;
		for (lPlayer = mPlayers.begin(); lPlayer != mPlayers.end(); ++lPlayer) {
			if ((*lPlayer)->getPuppet() == inPuppet)
        return (*lPlayer);
		}

    return 0;
  }

	void Instance::broadcast(Event* inEvt, bool fForceResp) {
		players_t::iterator lPlayer = mPlayers.begin();

		if (fForceResp)
			inEvt->setType(EVT_RESP);

		for (lPlayer; lPlayer != mPlayers.end(); ++lPlayer) {
			mServer->sendToPlayer(inEvt, *lPlayer);
		}

    inEvt->removeHandler();

	}

	void Instance::broadcast(BitStream const& inStream) {
		players_t::const_iterator lPlayer;
		for (lPlayer = mPlayers.begin(); lPlayer != mPlayers.end(); ++lPlayer) {
			mServer->sendToPlayer(inStream, *lPlayer);
		}
	}

  void Instance::send(Player& inPlayer, Event* inEvt) {
		inEvt->setType(EVT_RESP);
		mServer->sendToPlayer(inEvt, &inPlayer);
    inEvt->removeHandler();
  }

  void Instance::send(Player& inPlayer, BitStream const& inStream) {
		mServer->sendToPlayer(inStream, &inPlayer);
  }

	void Instance::rejectRequest(Event* inEvt) {
		inEvt->setFeedback(EVT_ERROR);
		mServer->sendToPlayer(inEvt);
    inEvt->removeHandler();
	}

  void
  Instance::broadcastCompressed(
    MessageID inMsgId,
    std::ostringstream const& raw)
  {
    broadcastCompressed(inMsgId, raw.str());
  }

  void
  Instance::broadcastCompressed(
    MessageID inMsgId,
    std::string const& raw)
  {
    using std::string;
    using std::vector;

    size_t rawSize = raw.size();

    // prepare the raw stream
    vector<unsigned char> venc, vraw(raw.begin(), raw.end());

    // encode it
    Archiver::encodeLzma(venc, vraw);

    // convert to std::string so we can write it into
    string senc(venc.begin(), venc.end());
    venc.clear();
    vraw.clear();

    // hash it
    const char *sum = MD5((unsigned char*)senc.c_str()).hex_digest();

		mStream.Reset();
		mStream.AssertStreamEmpty(); // _DEBUG_
    mStream.Write((unsigned char)inMsgId);
    mStream.Write(sum, strlen(sum));
    mStream.Write<size_t>(rawSize);
    mStream.Write(senc.c_str(), senc.size());

    broadcast(mStream);
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Bootstrap
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void Instance::bindHandlers() {
		bindToName("Ready", this, &Instance::evtPlayerReady);
    bindToName("StartTurn", this, &Instance::evtStartTurn);
    bindToName("EndTurn", this, &Instance::evtEndTurn);
		bindToName("CastSpell", this, &Instance::evtCastSpell);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Main Routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void Instance::start() {

		mLog->debugStream() << "players are ready, starting turn";

    // start the first player's turn, and track them
		mActivePuppet = mPuppets.front();
    mActivePlayer = getPlayer(mActivePuppet);
		Event* lEvt = mEvtMgr->createEvt("StartTurn");
		send(*mActivePlayer, lEvt);

    puppets_t::const_iterator itr;
    for (itr = mPuppets.begin(); itr != mPuppets.end(); ++itr)
      this->drawSpells(*itr, 4);


		fStarted = true;
	}

	void Instance::end() {
	}

	bool Instance::update() {
		return processEvents();
	}

	void Instance::subscribe(Player* inPlayer) {

		// validate player before hooking them
		if (!inPlayer) {
			mLog->warnStream()
			<< "an invalid player attempted to join the instance";

			return;
		}

		mPlayers.push_back(inPlayer);

		// assert profile name existence
		if (inPlayer->getPuppetName().empty()) {
			std::ostringstream lMsg;
			lMsg
				<< "Player '"
				<< inPlayer->getUsername()
				<< "' has no profile selected!";
			throw UnassignedProfile(lMsg.str());
		}

		// load the puppet
		Puppet* lPuppet = mServer->getResMgr().getPuppet(inPlayer, inPlayer->getPuppetName().c_str());
		if (!lPuppet) {
			throw BadEvent(std::string("unknown puppet named: ") + inPlayer->getPuppetName());
		}
    lPuppet->setObjectId(generateUID());
    lPuppet->live();

		inPlayer->setPuppet(lPuppet);

		mPuppets.push_back(inPlayer->getPuppet());
		lPuppet = 0;

		// attach them to this instance
		inPlayer->setInstance(this);

		mLog->debugStream()
		<< "a puppet named "
		<< mPlayers.back()->getPuppet()->getName()
		<< " has joined the instance";
	}

	void Instance::createPuppets() {
		// send profile info for players so they create the puppets
    mLog->infoStream() << "sending puppets data to clients";

    BitStream lStream;
    list<Puppet const*> lPuppets;
    for (puppets_t::const_iterator itr = mPuppets.begin(); itr != mPuppets.end(); ++itr)
      lPuppets.push_back(*itr);

    mServer->getResMgr().puppetsToStream(lStream, lPuppets);

    broadcast(lStream);
	}

	const int Instance::generateUID() {
	  return ++mUIDGenerator;
	}

  void Instance::handlePlayerDisconnected(Player* inPlayer) {
    mServer->addToShutdownQueue(this);
  }

  /*
   * format:
   * $owner-uid;nr-spells;spell1-name;spell1-uid;spell2-name;spell2-uid;...;\n
   */
	void Instance::drawSpells(Puppet* inPuppet, int inNrOfSpells) {

    if (!inPuppet)
      inPuppet = mActivePuppet;

    const int mMaxSpellsInHand = 6;

    mDrawnSpells.str("");
    mDrawnSpells << "[draw];" << inNrOfSpells << "\n";
    mDrawnSpells << "$" << inPuppet->getObjectId() << ";";

		// create nrSpellsPerTurn spells from the hero's deck
		Deck* lDeck = inPuppet->getDeck();

		int i;
		for (i=0; i< inNrOfSpells; ++i) {
			Spell* lSpell = lDeck->drawSpell();
			// assign UID and attach to puppet
			lSpell->setUID(generateUID());
			inPuppet->attachSpell(lSpell);

      mDrawnSpells << lSpell->getName() << ";" << lSpell->getUID() << ";";
      lSpell = 0;
		}

    mDrawnSpells << "\n";

    // tell it to drop some spells if its hand is overflown
    mDrawnSpells << "[drop];";
    int nrOverflow = inPuppet->nrSpellsInHand() - mMaxSpellsInHand;
    std::cout << "Puppet has " << inPuppet->nrSpellsInHand() << " spells in hand, an overflow of= " << nrOverflow << "\n";
    if (nrOverflow > 0) {
      mDrawnSpells << nrOverflow << "\n$" << inPuppet->getObjectId() << ";";
    } else
      mDrawnSpells << 0;

    Puppet::hand_t const& lHand = inPuppet->getHand();
    while (inPuppet->nrSpellsInHand() > mMaxSpellsInHand) {
      Spell* lSpell = lHand.front();
      mDrawnSpells << lSpell->getUID() << ";";
      inPuppet->detachSpell(lSpell);
      lSpell = 0;
    }
    mDrawnSpells << "\n";

    mLog->infoStream() << "sending drawn spells to Puppet " << inPuppet->getName();
    std::cout << "drawn spells:\n" << mDrawnSpells.str() << "\n";

    // broadcast the data
		mStream.Reset();
		mStream.AssertStreamEmpty(); // _DEBUG_
    mStream.Write((unsigned char)ID_DRAW_SPELLS);
    mStream.Write(RakString(mDrawnSpells.str().c_str()));
    broadcast(mStream);
    //broadcastCompressed(ID_DRAW_SPELLS, mDrawnSpells);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	bool Instance::evtPlayerReady(Event* inEvt) {

		++nrPlayersReady;

		// are all players ready?
		if (nrPlayersReady == mPlayers.size())
			start();

		return true;
	}

  bool Instance::evtStartTurn(Event* inEvt) {
    // start timer and shizzle

    // send to all players except the active one
    Event* lEvt = mEvtMgr->createEvt("TurnStarted");
    lEvt->setProperty("Puppet", stringify(mActivePuppet->getObjectId()));

    players_t::const_iterator lPlayer;
		for (lPlayer = mPlayers.begin(); lPlayer != mPlayers.end(); ++lPlayer) {
      if ((*lPlayer) == mActivePlayer)
        continue;

			mServer->sendToPlayer(lEvt, *lPlayer);
		}
    lEvt->removeHandler();

    mLog->debugStream() << "it's now " << mActivePuppet->getName() << "'s turn";
  }

	bool Instance::evtEndTurn(Event* inEvt) {
    // validate: make sure the sender is the active puppet
    if (getSender(inEvt) != mActivePuppet) {
      mLog->errorStream()
        << getSender(inEvt)->getName()
        << " is trying to end his opponent's turn!";
      return true;
    }
		// is any of its units charging?
			// if yes, toggle into Charging state
			// and toggle opponent into Blocking state

		// otherwise, just start the opponent's turn
		mLog->debugStream()
		<< "ending " << mActivePuppet->getName() << "'s turn ";

    // find the next puppet
    if (mActivePuppet == mPuppets.back())
      mActivePuppet = mPuppets.front();
    else {
      puppets_t::const_iterator lPuppet;
      for (lPuppet = mPuppets.begin(); lPuppet != mPuppets.end(); ++lPuppet)
        if ((*lPuppet) == mActivePuppet) {
          mActivePuppet = (*(++lPuppet));
          break;
        }
    }
    mActivePlayer = getPlayer(mActivePuppet);

		mLog->debugStream()
		<< "starting " << mActivePuppet->getName() << "'s turn ";

		Event* lEvt = mEvtMgr->createEvt("StartTurn");
    send(*mActivePlayer, lEvt);
    this->drawSpells();

		return true;
	}

	bool Instance::evtCastSpell(Event* inEvt) {
		// dispatch to Lua
		lua_getfield(mLua, LUA_GLOBALSINDEX, "processSpell");
		if(!lua_isfunction(mLua, 1))
		{
			mLog->errorStream() << "could not find Lua event processor!";
			lua_pop(mLua,1);
			return true;
		}

		// find the spell object
		int lSpellId = convertTo<int>(inEvt->getProperty("SpellId"));
		const Spell* lSpell = getSpell(lSpellId);
		Entity* lCaster = lSpell->getCaster();

		/*if (!lSpell) {
		  lSpell = mWaitingPuppet->getSpell(lSpellId);
		  lCaster = mWaitingPuppet;
		}*/

		if (!lSpell || !lCaster) {
		  mLog->errorStream() << "couldn't find requested Spell with id " << lSpellId;
		  rejectRequest(inEvt);
		  return true;
		}

		tolua_pushusertype(mLua,(void*)lCaster,"Pixy::Entity");
		tolua_pushusertype(mLua,(void*)lSpell,"Pixy::Spell");
		tolua_pushusertype(mLua,(void*)inEvt,"Pixy::Event");
		try {
			lua_call(mLua, 3, 1);
		} catch (std::exception& e) {
			mLog->errorStream() << "Lua Handler: " << e.what();
		}

		bool result = lua_toboolean(mLua, lua_gettop(mLua));

		lua_remove(mLua, lua_gettop(mLua));

		return result;
	}

}
