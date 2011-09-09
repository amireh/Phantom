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

#include "Pixy.h"
#include "player.hpp"
#include "dispatcher.hpp"
#include "sresource_manager.hpp"
#include "Spell.h"
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
  //typedef std::map<int, Spell*> spells_t;
  //typedef std::map<int, Unit*> units_t;

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
    void send(int in_uid, const Event&);

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

    /*
     * creates a unit frmo the given model, attaches it to the owner puppet,
     * and broadcasts a CreateUnit event to clients
     *
     * the event contains:
     * "UID": UID of the newly created unit
     * "OUID": UID of the owner puppet
     */
    Unit* _create_unit(std::string model, Puppet& owner);
    /*
     * removes a unit from play, and broadcasts a RemoveUnit with the UID
     */
    void  _destroy_unit(int inUID);
    void  _destroy_unit(Unit*);

    void _destroy_puppet(int inUID);

	protected:

		void init_lua();

    bool pass_to_lua(const char* inFunc, int argc, ...);

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
		 *	transmits puppet info to all players so that they can create and
		 *	render them
		 */
		void create_puppets();

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

    /*
     * validates the given spell object and passes off to its Lua handler
     */
		void on_cast_spell(const Event& inEvt);

    void on_charge(const Event&);
    void on_cancel_charge(const Event&);

    void on_block(const Event&);
    void on_cancel_block(const Event&);
    void on_end_block_phase(const Event&);
    void on_battle_over(const Event&);

	private:

    void finish(puppet_ptr);

    player_cptr get_player(puppet_ptr inPuppet);
    puppet_ptr get_puppet(int inUID);
    Spell* get_spell(int inUID);
    Unit* get_unit(int inUID);

		log4cpp::Category	*log_;
		lua_State			*lua_;
		log4cpp::Category	*lua_log_;

		//int	mId; /*! my unique id in the cluster */
    boost::uuids::uuid uuid_;
		players_t	players_; //! my subscribed players
		puppets_t	puppets_; //! and their puppets
    //spells_t spells_;
    //units_t units_;

		bool		started_; //! are the players ready?
		int			nr_ready_players_; //! how many players are ready?
    // on every attack, both clients must send a BattleOver event
    // to start the next turn
    int nr_battle_acks_;
		puppet_ptr		active_puppet_; //! the puppet whose turn is active
    puppet_ptr    waiting_puppet_;
		player_cptr		active_player_; //! owner of the active puppet
    player_cptr   waiting_player_;

    puppet_ptr winner_;

    bool in_battle_;
    bool finished_;

		int	uid_generator_; //! assigns ids to all entities

    //typedef std::map< const Event*, player_cptr > player_events_t;
    //player_events_t player_events_;

    std::ostringstream drawn_spells_;
    dispatcher dispatcher_;
    boost::asio::strand strand_;

    sresource_manager& rmgr_;

    typedef std::list<Unit*> attackers_t;
    attackers_t attackers_;

    typedef std::vector<Unit*> death_list_t;
    death_list_t death_list_;

    // key is the attacker, value is the list of blockers in order
    typedef std::map<Unit*, std::list<Unit*> > blockers_t;
    blockers_t blockers_;

    bool running_;

	};

  typedef boost::shared_ptr<instance> instance_ptr;
}
}
#endif
