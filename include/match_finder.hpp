/*
 *  match_finder.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_match_finder_H
#define H_match_finder_H

#include <boost/asio.hpp>
#include "PixyLog.h"
#include "player.hpp"
//#include "instance.hpp"

#include <list>

/* TODO:
 * 1) validate puppet name on queue
 * 2) leave queue functionality
 * 3) match making rating
 */
using std::list;
namespace Pixy {
namespace Net {

	/*! \class match_finder
	 *	\brief
	 *	The match_finder tracks every player queued for a game in a list,
	 *	and matches them with an eligible opponent .. then informs the server
	 *	that a match is ready to start, and then a game instance is launched.
	 */
	class server;
	class match_finder {
	public:
		match_finder(boost::asio::io_service&);
		virtual ~match_finder();

		/*!
     * @brief
     * enlists the Player's assigned Puppet in the match making queue
     */
		void join_queue(player_cptr);

		/*!
     * @brief
		 * removes the player from the queue
		 */
		void leave_queue(player_cptr);

    bool already_queued(player_cptr);

	protected:

		/*!
     * @brief
		 * processes the queue of waiting players and fires the signal for a
		 * new instance if a match is found
		 */
		void seek();

    void do_join_queue(player_cptr);
    void do_leave_queue(player_cptr);

		void start_match(player_cptr, player_cptr);

    bool is_eligible(player_cptr, player_cptr);

		log4cpp::Category *log_;
    typedef list<player_cptr> seekers_t;
		seekers_t seekers_;
    boost::asio::strand strand_;
		//server* mServer;

	};
}
}
#endif
