/*
 *  Player.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "player.hpp"
#include "instance.hpp"
#include "server_connection.hpp"

namespace Pixy {
namespace Net {
	Player::Player(connection* conn, std::string username)
  : username_(username),
    conn_(conn),
    puppet_(),
    instance_(),
    online_(false),
    in_lobby_(false)
  {
		std::cout << "Player is alive!\n";
	}

	Player::~Player() {
    puppet_.reset();
    instance_.reset();
    for (auto puppet : puppets_)
      delete puppet;
    puppets_.clear();
    conn_ = 0;
    online_ = false;
    in_lobby_ = false;
		std::cout << "Player is destroyed!\n";
	}

	void Player::set_instance(instance_ptr inInstance) {
		instance_ = inInstance;
	}

	void Player::set_puppet(puppet_ptr inPuppet) {
		puppet_ = inPuppet;
	}

  void Player::set_online(bool inF) {
    online_ = inF;
  }
  void Player::set_in_lobby(bool inF) {
    in_lobby_ = inF;
  }


	puppet_ptr Player::get_puppet() const {
		return puppet_;
	}
	string const& Player::get_puppet_name() const {
		return puppet_name_;
	}

	string const& Player::get_username() const {
		return username_;
	}
	instance_ptr Player::get_instance() const {
		return instance_;
	}

  void Player::send(const Event& evt) const {
    conn_->send(evt);
  }

  bool Player::is_online() const {
    return online_;
  }
  bool Player::is_in_lobby() const {
    return in_lobby_;
  }

  void Player::leave_instance() {
    instance_.reset();
  }

  bool Player::operator==(const Player& rhs) const
  {
    return this->username_ == rhs.username_;
  }

  Player::puppets_t const& Player::get_puppets() const
  {
    return puppets_;
  }
}
}
