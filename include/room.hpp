/*
 *  room.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 Shroom Studios. All rights reserved.
 *
 */

#ifndef H_room_H
#define H_room_H

#include "Pixy.h"
#include "player.hpp"
#include "dispatcher.hpp"
#include "Event.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>


using std::list;
namespace Pixy {
namespace Net {
	typedef list<player_cptr>	players_t;

	/*! \class room
	 *	\brief
	 *	The room is where a combat match is played out. This component
	 *	handles all the game logic and validation, keeps the players in synch,
	 *	listens to events and transmits them to players and so on.
	 */
  class lobby;
	class room : public boost::enable_shared_from_this<room> {
	public:

    enum {
      max_msg_length = 255,
      max_viewable_contacts = 49
    };

		room(boost::asio::io_service&, std::string uid);
    room() = delete;
    room(const room&) = delete;
    room& operator=(const room&) = delete;
		virtual ~room();

    void enqueue(const Event&, player_cptr);
    void enlist(player_cptr);
    void remove(player_cptr);

    std::string const& get_name() const;

    void relay(std::string const& msg, player_cptr sender);
    void tell(std::string const& msg, player_cptr sender, std::string const& recipient);

	protected:
    friend class lobby;

   /*!
     * @brief
     * Sends the Event to all the players registered in this room.
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
		 * convenience method for retrieving the puppet of the event sender
		 */
		player_cptr get_sender(const Event& inEvt);

		/*! \brief
		 *	"subscribes" a player as a participant in this room, subscribed
		 *	players will receive events from this room
		 */
		void subscribe(player_cptr);

		/*!	\brief
		 *	registers event callback handlers
		 */
		void bind_handlers();

	private:
		log4cpp::Category	*log_;

    void _relay(std::string const& msg, std::string const& sender);

		players_t	players_; //! clients connected to this room

    dispatcher dispatcher_;
    boost::asio::strand strand_;

    bool running_;
    std::string name_;
	};

  typedef boost::shared_ptr<room> room_ptr;
}
}
#endif
