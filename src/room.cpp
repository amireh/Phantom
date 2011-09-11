/*
 *  room.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "room.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

	room::room(boost::asio::io_service& io_service, std::string uid, bool is_permanent)
  : //dispatcher_(io_service),
    strand_(io_service),
    name_(uid),
    is_permanent_(is_permanent)
  {

		log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "room#" + name_);

    running_ = true;
    is_open_ = false;

    bind_handlers();

    log_->infoStream() << "now open";
	}

	room::~room() {

    players_.clear();

		log_->infoStream() << "closing down";

		if (log_) {
			delete log_;
      log_ = 0;
    }

	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Misc
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
  std::string const& room::get_name() const {
    return name_;
  }
	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Helpers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	player_cptr room::get_sender(const Event& evt) {
    for (auto player : players_)
      if (player == evt.Sender)
        return player;

    throw UnknownEventSender( std::string("in room::get_sender: " + stringify((int)evt.UID)) );
	}

	void room::broadcast(const Event& evt) {
    strand_.post( [&, evt]() -> void {
      for (auto player : players_)
        if (player && player->is_online())
          send(player, evt);
    });
      //else
      //  remove(player);
	}

  void room::send(player_cptr player, const Event& evt) {
    player->send(evt);
  }

	void room::reject(Event& evt) {
		evt.Feedback = EventFeedback::InvalidRequest;
		send(get_sender(evt), evt);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Bootstrap
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void room::bind_handlers() {
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Main Routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void room::subscribe(player_cptr player) {

		// validate player before hooking them
		if (!player->is_online()) {
			log_->warnStream()
			<< "an offline player attempted to join the room";

			throw std::runtime_error("an offline player attempted to join the room");
		}

    players_.push_back(player);
	}

  /*void room::enqueue(const Event& evt, player_cptr sender) {
    if (!running_)
      return;

    assert(evt.Sender);
    //~ std::cout << "room: got evt from " << evt.Sender->get_username() << "\n";
    dispatcher_.deliver(evt);
  }*/

  void room::enlist(player_cptr player)
  {
    strand_.post( [&, player]() -> void { do_enlist(player); } );
  }
  void room::do_enlist(player_cptr player) {
    log_->debugStream() << "a player has joined the channel: " << player->get_puppet()->getName();

    is_open_ = true;

    //_relay(player->get_puppet()->getName() + " has joined the channel.", name_);
    Event announcement(EventUID::JoinedRoom, EventFeedback::Ok);
    announcement.setProperty("R", name_);
    announcement.setProperty("S", player->get_puppet()->getName());
    broadcast(announcement);

    // send the player the room ID and a list of the current clients in it
    // note: the list can't be longer than 49 people
    std::ostringstream list;
    int i=0;
    for (auto player_ : players_) {
      list << player_->get_puppet()->getName() << ";";
      if (++i == max_viewable_contacts)
        break;
    }

    players_.push_back(player);

    Event confirmation(EventUID::JoinRoom, EventFeedback::Ok);
    confirmation.setProperty("R", name_);
    confirmation.setProperty("C", list.str());
    send(player, confirmation);
  }

  void room::remove(player_cptr player)
  {
    strand_.post( [&, player]() -> void { do_remove(player); } );
  }
  void room::do_remove(player_cptr in_player) {
    for (auto player_ : players_)
      if (player_ == in_player)
      {
        std::string player_name = player_->get_puppet()->getName();

        log_->debugStream() << "a player has left channel: " << player_name;

        //_relay(player->get_puppet()->getName() + " has left the channel.", name_);
        players_.remove(player_);

        // close this room down if there are no players left and it's
        // not a permanent one
        if (!is_permanent() && players_.empty())
          return server::singleton().get_lobby()->close_room(shared_from_this());

        Event e(EventUID::LeftRoom, EventFeedback::Ok);
        e.setProperty("R", name_);
        e.setProperty("S", player_name);
        broadcast(e);

        break;
      }

  }

  void room::relay(std::string const& msg, player_cptr sender)
  {
    _relay(msg, sender->get_puppet()->getName());
  }

  void room::_relay(std::string const& msg, std::string const& sender)
  {
    Event e(EventUID::IncomingMessage, EventFeedback::Ok);
    e.setProperty("R", name_);
    e.setProperty("S", sender);
    e.setProperty("M", msg);

    broadcast(e);
  }

  void room::tell(std::string const& msg, player_cptr sender, std::string const& recipient)
  {
    const Player* player_ = 0;
    for (auto player : players_)
      if (player->get_puppet()->getName() == recipient)
      {
        player_ = player.get();
        break;
      }

    if (!player_)
    {
      Event resp(EventUID::SendWhisper, EventFeedback::Error);
      resp.Sender = sender;
      return reject(resp);
    }

    Event reply(EventUID::IncomingWhisper, EventFeedback::Ok);
    reply.setProperty("M", msg);
    reply.setProperty("S", sender->get_puppet()->getName());
    player_->send(reply);
  }

  bool room::is_permanent() const
  {
    return is_permanent_;
  }

  bool room::is_open() const
  {
    return is_open_ || is_permanent_;
  }

  bool room::is_empty() const
  {
    return players_.empty();
  }

  player_cptr room::get_player(std::string const& name) const
  {
    for (auto player : players_)
      if (player->get_puppet()->getName() == name)
      {
        return player;
      }

    return player_cptr();
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
}
}
