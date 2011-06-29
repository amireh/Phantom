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

namespace Pixy {
namespace Net {
	Player::Player() {
		mPuppet = 0;
    mInstance = 0;
		mId = 0;
		std::cout << "Player is alive!\n";
    fOnline = false;
	}

	Player::~Player() {
		mPuppet = 0;
    mInstance = 0;
		std::cout << "Player is destroyed!\n";
	}

	//~ void Player::setGUID(const RakNetGUID inGuid) {
		//~ mGUID = inGuid;
	//~ }

	void Player::setUserId(const int inId) {
		mId = inId;
	}

	//~ void Player::setInstance(Instance* inInstance) {
		//~ mInstance = inInstance;
	//~ }

	void Player::setPuppet(Puppet* inPuppet) {
		mPuppet = inPuppet;
	}
	void Player::setPuppetName(string inProfile) {
		mPuppetName = inProfile;
	}

	void Player::setUsername(string inUsername) {
		mUsername = inUsername;
	}

  void Player::setIsOnline(bool inF) {
    fOnline = inF;
  }


	//~ RakNetGUID Player::getGUID() const {
		//~ return mGUID;
	//~ }

	int Player::getUserId() const {
		return mId;
	}

	Puppet* Player::getPuppet() const {
		return mPuppet;
	}
	string const& Player::getPuppetName() const {
		return mPuppetName;
	}

	string const& Player::getUsername() const {
		return mUsername;
	}
	//~ Instance* Player::getInstance() const {
		//~ return mInstance;
	//~ }

  bool Player::isOnline() const {
    return fOnline;
  }
}
}
