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
    puppet_(0),
    instance_(),
    online_(false)
  {
		std::cout << "Player is alive!\n";
	}

	Player::~Player() {
    if (puppet_)
      delete puppet_;

		puppet_ = 0;
    instance_.reset();
    conn_ = 0;
		std::cout << "Player is destroyed!\n";
	}

	void Player::set_instance(instance_ptr inInstance) {
		instance_ = inInstance;
	}

	void Player::set_puppet(Puppet* inPuppet) {
		puppet_ = inPuppet;
	}

  void Player::set_online(bool inF) {
    online_ = inF;
  }


	Puppet* Player::get_puppet() const {
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
}
}
