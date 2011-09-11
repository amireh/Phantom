/*
 *  lobby.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 Shroom Studios. All rights reserved.
 *
 */

#ifndef H_lobby_H
#define H_lobby_H

#include "Pixy.h"
#include "room.hpp"
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
	typedef std::map<std::string, room_ptr>	rooms_t;

	/*! \class lobby
	 *	\brief
	 * The lobby contains rooms which host chat participants. The lobby is
   * responsible for joining players into rooms, spawning new rooms, and
   * closing them when they're empty.
	 */
	class lobby : public boost::enable_shared_from_this<lobby> {
	public:
		lobby(boost::asio::io_service&);
    lobby() = delete;
    lobby(const lobby&) = delete;
    lobby& operator=(const lobby&) = delete;
		virtual ~lobby();

    /*!
     * @brief
     * Enlists the client in the General room and flags it as "In Lobby".
     */
    void enlist(player_ptr);

    void on_dropout(player_cptr);

    void enqueue(const Event&, player_cptr);

    /*!
     * @brief
     * Closes a room. This is called internally in room::remove()
     */
    void close_room(room_ptr);

	protected:

    room_ptr open_room(std::string const& name, bool is_permanent=false);

    /*!
     * @brief
     * Replies with a list of available rooms.
     */
    void on_list_rooms(const Event&);

    /*!
     * @brief
     * If no room was found with UID = Event.RUID, a new room is spawned,
     * otherwise it enlists the client into the given room.
     */
    void on_join_room(const Event&);

    void on_leave_room(const Event&);

    /*!
     * @brief
     * Delivers Event.Msg to the room with UID of Event.RUID.
     */
    void on_send_message(const Event&);

    /*!
     * @brief
     * Delivers Event.Msg to the target client with UID of Event.TUID.
     */
    void on_send_whisper(const Event&);


    /*!
     * @brief
     * Sends a message to all the Rooms that will be delivered to all online
     * users.
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

		/*!	\brief
		 *	registers event callback handlers
		 */
		void bind_handlers();

	private:
		log4cpp::Category	*log_;

		rooms_t	rooms_; //! my subscribed players

    dispatcher dispatcher_;
    boost::asio::strand strand_;
	};

  typedef boost::shared_ptr<lobby> lobby_ptr;
}
}
#endif
