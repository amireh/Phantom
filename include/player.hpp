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
#include "event.hpp"
#include <string>

using std::string;
namespace Pixy {
namespace Net {

  class server;
  class connection;
  class Instance;
	class Player {
	public:
		Player(connection*, std::string username);
		virtual ~Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;



    Instance* get_instance() const;
		Puppet* get_puppet() const;
		std::string const& get_puppet_name() const;
		std::string const& get_username() const;
    bool is_online() const;

    void send(const Event& evt) const;

	protected:
    friend class connection;
    friend class server;

    /// name of the puppet the player joined the queue with
		void set_puppet(Puppet* inPuppet);
		void set_instance(Instance*);
    void set_connection(connection*);
    void set_online(bool);

  private:
    connection* conn_;
		string username_;
    Instance* instance_;
		Puppet* puppet_;
		string puppet_name_;
    bool online_;

	};

  typedef boost::shared_ptr<Player> player_ptr;
  typedef boost::shared_ptr<const Player> player_cptr;
}
}
#endif
