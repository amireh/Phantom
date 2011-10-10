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

    open_room("General", true);
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
    dispatcher_.bind(EventUID::LeaveLobby, this, &lobby::on_leave_lobby);
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
    this->remove(player);
  }

  room_ptr lobby::open_room(const std::string& name, bool is_permanent)
  {
    room_ptr new_room_(new room(strand_.get_io_service(), name, is_permanent));
    rooms_.insert(std::make_pair(new_room_->get_name(), new_room_));
    return new_room_;
  }

  void lobby::close_room(room_ptr in_room)
  {
    strand_.post([&, in_room]() -> void { rooms_.erase(in_room->get_name()); });
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	Event Handlers
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

  void lobby::enlist(player_ptr player)
  {
    if (player->is_in_lobby())
      return;

    rooms_["General"]->enlist(player);
    player->set_in_lobby(true);
  }

  void lobby::remove(player_cptr player)
  {
    for (auto room_ : rooms_)
      room_.second->remove(player);

    ((Player*)player.get())->set_in_lobby(false);
  }

  void lobby::on_leave_lobby(const Event& e)
  {
    strand_.post([&, e]() -> void { this->remove(e.Sender); });
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
      room_ptr new_room_ = open_room(e.getProperty("R"));
      new_room_->enlist(e.Sender);
      new_room_.reset();
      return;
      //Event resp(e);
      //return reject(resp);
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
    assert(e.hasProperty("T") && e.hasProperty("M"));
    //~ rooms_t::iterator room_ = rooms_.find(e.getProperty("R"));
    std::string msg_ = e.getProperty("M");
    std::string target_ = e.getProperty("T");
    if (msg_.size() > room::max_msg_length)
    {
      Event resp(e);
      return reject(resp);
    }

    // find the target
    const Player* player = 0;
    for (auto room_ : rooms_) {
      player = room_.second->get_player(target_);
      if (player)
      {
        log_->debugStream()
          << "delivering whisper from " << e.Sender->get_puppet()->getName()
          << " to " << player->get_puppet()->getName();

        Event tell(EventUID::IncomingWhisper, EventFeedback::Ok);
        tell.setProperty("M", msg_);
        tell.setProperty("S", target_);
        return player->send(tell);
      }
    }

    Event resp(e);
    resp.setProperty("E", "UnknownTarget");
    log_->debugStream()
          << "invalid whisper from " << e.Sender->get_puppet()->getName()
          << " to " << target_ << ", target not found";
    return reject(resp);
    //room_->second->tell(msg_, e.Sender, e.getProperty("T"));
  }

}
}
