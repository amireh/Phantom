/*
 *  lobby.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "lobby.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

	lobby::lobby(boost::asio::io_service& io_service)
  : dispatcher_(io_service),
    strand_(io_service)
  {

		log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "lobby");
    bind_handlers();

    log_->infoStream() << "lobby is open";

    room_ptr new_room_(new room(strand_.get_io_service(), "General"));
    rooms_.insert(std::make_pair(new_room_->get_name(), new_room_));
    new_room_.reset();
	}

	lobby::~lobby() {

    rooms_.clear();

		log_->infoStream() << "lobby shut down";

		if (log_) {
			delete log_;
      log_ = 0;
    }
	}


	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Misc
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Helpers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void lobby::broadcast(const Event& evt) {
    for (auto room : rooms_)
      room.second->broadcast(evt);
	}

  void lobby::send(player_cptr player, const Event& evt) {
    player->send(evt);
  }

	void lobby::reject(Event& evt) {
		evt.Feedback = EventFeedback::InvalidRequest;
		send(evt.Sender, evt);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Bootstrap
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

	void lobby::bind_handlers() {
    dispatcher_.bind(EventUID::ListRooms, this, &lobby::on_list_rooms);
    dispatcher_.bind(EventUID::JoinRoom, this, &lobby::on_join_room);
    dispatcher_.bind(EventUID::LeaveRoom, this, &lobby::on_leave_room);
    dispatcher_.bind(EventUID::SendMessage, this, &lobby::on_send_message);
    dispatcher_.bind(EventUID::SendWhisper, this, &lobby::on_send_whisper);
	}

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Main Routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

  void lobby::enqueue(const Event& evt, player_cptr sender) {
    assert(evt.Sender);
    dispatcher_.deliver(evt);
  }

  void lobby::on_dropout(player_cptr player) {
    for (auto room_ : rooms_)
    {
      room_.second->remove(player);
    }
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

  void lobby::enlist(player_ptr player)
  {
    rooms_["General"]->enlist(player);
    player->set_in_lobby(true);
  }

  void lobby::on_list_rooms(const Event&)
  {
  }
  void lobby::on_join_room(const Event& e)
  {
    assert(e.hasProperty("R"));
    rooms_t::iterator room_ = rooms_.find(e.getProperty("R"));
    if (room_ == rooms_.end())
    {
      Event resp(e);
      return reject(resp);
    }

    room_->second->enlist(e.Sender);
  }
  void lobby::on_leave_room(const Event& e)
  {
    assert(e.hasProperty("R"));
    rooms_t::iterator room_ = rooms_.find(e.getProperty("R"));
    if (room_ == rooms_.end())
    {
      Event resp(e);
      return reject(resp);
    }

    room_->second->remove(e.Sender);
  }
  void lobby::on_send_message(const Event& e)
  {
    assert(e.hasProperty("R") && e.hasProperty("M"));
    rooms_t::iterator room_ = rooms_.find(e.getProperty("R"));
    std::string msg_ = e.getProperty("M");
    if (room_ == rooms_.end() || msg_.size() > room::max_msg_length)
    {
      Event resp(e);
      return reject(resp);
    }

    room_->second->relay(msg_, e.Sender);
  }

  void lobby::on_send_whisper(const Event& e)
  {
    assert(e.hasProperty("R") && e.hasProperty("T") && e.hasProperty("M"));
    rooms_t::iterator room_ = rooms_.find(e.getProperty("R"));
    std::string msg_ = e.getProperty("M");
    if (room_ == rooms_.end() || msg_.size() > room::max_msg_length)
    {
      Event resp(e);
      return reject(resp);
    }

    room_->second->tell(msg_, e.Sender, e.getProperty("T"));
  }

}
}
