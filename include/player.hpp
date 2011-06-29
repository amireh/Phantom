/*
 *  Player.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 5/29/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_Player_H
#define H_Player_H

#include "Puppet.h"
#include <string>

using std::string;
namespace Pixy {
namespace Net {
  class Instance;
	class Player {
	public:
		Player();
		virtual ~Player();

		//void setGUID(const RakNetGUID inGuid);
		void setUserId(const int inId);
		void setPuppet(Puppet* inPuppet);
		void setPuppetName(string inProfile); // name of the puppet for later retrieval by the instance
		void setUsername(string inUsername);
    void setIsOnline(bool inF);
		void setInstance(Instance* inInstance);

		//RakNetGUID getGUID() const;
		int getUserId() const;
		Instance* getInstance() const;
		Puppet* getPuppet() const;
		string const& getPuppetName() const;
		string const& getUsername() const;
    bool isOnline() const;


	protected:
		//RakNetGUID mGUID;
		Puppet* mPuppet;
		int mId; // represents the account the player authenticates with
		Instance* mInstance;
		string mUsername;
		string mPuppetName;
    bool fOnline;
	};
}
}
#endif
