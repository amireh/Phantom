/*
 *  instance.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_instance_H
#define H_instance_H

#include "PixyLog.h"
#include "PixyShared.h"
#include "player.hpp"
#include "dispatcher.hpp"
#include "Event.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>

// LUA
extern "C" {
#include "lua.h"
#include "lualib.h"
}

using std::list;
namespace Pixy {
namespace Net {
	typedef list<player_cptr>	players_t;
	typedef list<puppet_ptr>	puppets_t;
  typedef std::map<int, Spell*> spells_t;
  typedef std::map<int, Unit*> units_t;

	/*! \class instance
	 *	\brief
	 *	The instance is where a combat match is played out. This component
	 *	handles all the game logic and validation, keeps the players in synch,
	 *	listens to events and transmits them to players and so on.
	 */
	class Server;
	class instance : public boost::enable_shared_from_this<instance> {
	public:
		instance(players_t, boost::asio::io_service&);
    instance() = delete;
    instance(const instance&) = delete;
    instance& operator=(const instance&) = delete;
		virtual ~instance();

    // every instance has a UUID
    bool operator==(instance const&);

    void bootstrap();
		void start();

    void on_dropout(player_cptr);

		boost::uuids::uuid get_uuid() const;

		void lua_log(std::string);

    void enqueue(const Event&, player_cptr);

		//players_t const& get_players() const;

    /*!
     * @brief
     * Sends the Event to all the players registered in this instance.
     */
		void broadcast(const Event&);

    /*!
     * @brief
     * Sends the Event to the given player.
     */
    void send(player_cptr, const Event&);

    /*!
     * @brief
     * Rejects the player's request by sending the event back with its
     * Feedback property set to EventFeedback::InvalidRequest
     */
		void reject(Event& inEvt);

    /*!
     * @brief
     * Generates an instance-unique UID to reference every game entity.
     */
		const int generate_uid();

		/*!
     * @brief
		 * convenience method for retrieving the puppet of the event sender
		 */
		player_cptr get_sender(const Event& inEvt);

	protected:

		void init_lua();

		/*! \brief
		 *	returns true when both players notified that they're ready by sending
		 *	"Ready" event
		 */
		bool has_started();

		/*! \brief
		 *	generates X spells from the puppet's deck, inserts them
		 *	into its hand, and sends the newly drawn spells to the puppet
     *
     * if inPuppet was not passed, the active puppet is assumed to be chosen
		 */
		void draw_spells(puppet_ptr inPuppet, int inNrOfSpells = 2);

		/* +-+-+-+-+-+-+-+ *
		 * EVENT HANNDLERS *
		 * +-+-+-+-+-+-+-+ */

		/*! \brief
		 *	notification received from player that he's done loading
		 *	once both players send this message, teh game will start by
		 *	sending event "StartTurn"
		 */
		void on_player_ready(const Event& inEvt);

    /*
     * Once this event is received, it means the puppet received the StartTurn
     * order and has started their local timer. Here we begin the turn timer
     * and broadcast to all players that this puppet's turn has started.
     */
    void on_start_turn(const Event& inEvt);

    /*
     * acknowledges player's request to end their turn, checks whether the
     * puppet is charging, if so, then signal opponent puppet to switch into
     * blocking phase. If sender is not charging, send StartTurn to opponent
     * puppet and wait for ack.
     */
    void on_end_turn(const Event& inEvt);

		void on_cast_spell(const Event& inEvt);

		log4cpp::Category	*log_;
		//Server				*mServer;
		//EventManager		*mEvtMgr;
		lua_State			*lua_;
		log4cpp::Category	*lua_log_;

		//int	mId; /*! my unique id in the cluster */
    boost::uuids::uuid uuid_;
		players_t	players_; //! my subscribed players
		puppets_t	puppets_; //! and their puppets
    spells_t spells_;
    units_t units_;

		bool		started_; //! are the players ready?
		int			nr_ready_players_; //! how many players are ready?
		puppet_ptr		active_puppet_; //! the puppet whose turn is active
		player_cptr		active_player_; //! owner of the active puppet

		int	uid_generator_; //! assigns ids to all entities

    typedef std::map< const Event*, player_cptr > player_events_t;
    player_events_t player_events_;

	private:

		/*! \brief
		 *	"subscribes" a player as a participant in this instance, subscribed
		 *	players will receive events from this instance
		 */
		void subscribe(player_cptr);

		/*!	\brief
		 *	registers event callback handlers
		 */
		void bind_handlers();

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
		void create_puppets();

    puppet_ptr get_puppet(int inUID);
    Spell* get_spell(int inUID);
    Unit* get_unit(int inUID);
    player_cptr get_player(puppet_ptr inPuppet);

    std::ostringstream drawn_spells_;
    dispatcher dispatcher_;
    boost::asio::strand strand_;
    //BitStream mStream;

    bool running_;

	};

  typedef boost::shared_ptr<instance> instance_ptr;
}
}
#endif
