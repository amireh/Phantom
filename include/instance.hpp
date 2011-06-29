/*
 *  Instance.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_Instance_H
#define H_Instance_H

#include "PixyLog.h"
#include "PixyShared.h"
#include "player.hpp"
#include "EventListener.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <iostream>
#include <list>
#include <vector>

// LUA
extern "C" {
#include "lua.h"
#include "lualib.h"
}

using std::list;
namespace Pixy {
namespace Net {
	typedef list<Player*>	players_t;
	typedef list<Puppet*>	puppets_t;
  typedef map<int, Spell*> spells_t;
  typedef map<int, Unit*> units_t;

	/*! \class Instance
	 *	\brief
	 *	The instance is where a combat match is played out. This component
	 *	handles all the game logic and validation, keeps the players in synch,
	 *	listens to events and transmits them to players and so on.
	 */
	class Server;
	class Instance : public EventListener {
	public:
		Instance(std::list<Player*> inPlayers);
		virtual ~Instance();

		void start();

		bool update();

		void end();

    void handlePlayerDisconnected(Player* inPlayer);

		//void setId(int inId);
		boost::uuids::uuid getUUID() const;

		void luaLog(std::string inMsg);

		players_t const& getPlayers() const;

		/*! \brief
		 *	helper method for transmitting an event to all registered players
		 */
		void broadcast(Event* inEvt, bool fForceResp = true);
    //void broadcast(BitStream const& inStream);
    //void broadcastCompressed(MessageID inMsgId, std::ostringstream const& inStream);
    //void broadcastCompressed(MessageID inMsgId, std::string const& inStream);

    void send(Player& inPlayer, Event* inEvt);
    //void send(Player& inPlayer, BitStream const& inEvt);

		void rejectRequest(Event* inEvt);

		const int generateUID();

		/*! \brief
		 *	convenience method for retrieving the puppet of the event sender
		 *
		 *	it uses the guid tracked in the server for matching against the
		 *	players tracked here, and then getting the attached puppet
		 */
		Puppet* getSender(Event* inEvt);

    bool operator==(Instance const& rhs);

	protected:

		void initLua();

		/*! \brief
		 *	returns true when both players notified that they're ready by sending
		 *	"Ready" event
		 */
		bool hasStarted();

		/* +-+-+-+-+-+-+-+ *
		 * EVENT HANNDLERS *
		 * +-+-+-+-+-+-+-+ */

		/*! \brief
		 *	notification received from player that he's done loading
		 *	once both players send this message, teh game will start by
		 *	sending event "StartTurn"
		 */
		bool evtPlayerReady(Event* inEvt);

		/*! \brief
		 *	generates X spells from the puppet's deck, inserts them
		 *	into its hand, and sends the newly drawn spells to the puppet
		 */
		void drawSpells(Puppet* inPuppet = 0, int inNrOfSpells = 2);

    /*
     * Once this event is received, it means the puppet received the StartTurn
     * order and has started their local timer. Here we begin the turn timer
     * and broadcast to all players that this puppet's turn has started.
     */
    bool evtStartTurn(Event* inEvt);

    /*
     * acknowledges player's request to end their turn, checks whether the
     * puppet is charging, if so, then signal opponent puppet to switch into
     * blocking phase. If sender is not charging, send StartTurn to opponent
     * puppet and wait for ack.
     */
    bool evtEndTurn(Event* inEvt);

		bool evtCastSpell(Event* inEvt);

		log4cpp::Category	*mLog;
		Server				*mServer;
		EventManager		*mEvtMgr;
		lua_State			*mLua;
		log4cpp::Category	*mLuaLog;

		//int	mId; /*! my unique id in the cluster */
    boost::uuids::uuid mUUID;
		players_t	mPlayers; //! my subscribed players
		puppets_t	mPuppets; //! and their puppets
    spells_t mSpells;
    units_t mUnits;

		bool		fStarted; //! are the players ready?
		int			nrPlayersReady; //! how many players are ready?
		Puppet		*mActivePuppet; //! the puppet whose turn is active
		Player		*mActivePlayer;

		int	mUIDGenerator; //! assigns ids to all entities
		int	nrSpellsPerTurn;

	private:

		/*! \brief
		 *	"subscribes" a player as a participant in this Instance, subscribed
		 *	players will receive events from this Instance
		 */
		void subscribe(Player* inPlayer);

		/*!	\brief
		 *	registers event callback handlers
		 */
		void bindHandlers();

		/*! \brief
		 *	sends to each player an event named "AssignPuppets" that contains
		 *	two properties:
		 *		1) "Puppet" -> name of the player's puppet
		 *		2) "EnemyPuppet" -> name of the enemy's puppet
		 *	these names are used as unique identifiers for every action taken
		 *	by the players from now on; Entities (puppets, units, and spells)
		 *	are _OWNED_ by so said profile
		 */
		//void assignPuppets();

		/*! \brief
		 *	transmits puppet info to all players so that they can create and
		 *	render them
		 */
		void createPuppets();

    Puppet* getPuppet(int inUID);
    Spell* getSpell(int inUID);
    Unit* getUnit(int inUID);
    Player* getPlayer(Puppet const* inPuppet);

    std::ostringstream mDrawnSpells;
    //BitStream mStream;

	};
}
}
#endif
