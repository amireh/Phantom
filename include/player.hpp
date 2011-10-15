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
#include "Event.hpp"
#include <string>

using std::string;
namespace Pixy {
namespace Net {

  class server;
  class connection;
  class instance;
  class lobby;

  typedef boost::shared_ptr<instance> instance_ptr;

	class Player {
	public:
    typedef std::list<Puppet*> puppets_t;

		Player(connection*, std::string username);
		virtual ~Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    bool operator==(const Player&) const;

    instance_ptr get_instance() const;
		puppet_ptr get_puppet() const;
		std::string const& get_puppet_name() const;
		std::string const& get_username() const;
    bool is_online() const;
    bool is_in_lobby() const;
    bool is_in_instance() const;

    void send(const Event& evt) const;
    puppets_t const& get_puppets() const;

	protected:
    friend class connection;
    friend class server;
    friend class instance;
    friend class lobby;

    /// name of the puppet the player joined the queue with
		void set_puppet(puppet_ptr inPuppet);
		void set_instance(instance_ptr);
    void set_online(bool);
    void set_in_lobby(bool);
    void set_in_instance(bool);

    void leave_instance();

    puppets_t puppets_;

  private:
    connection* conn_;
		string username_;
    instance_ptr instance_;
		puppet_ptr puppet_;
		string puppet_name_;
    bool online_;
    bool in_lobby_;
    bool in_instance_;
	};

  typedef boost::shared_ptr<Player> player_ptr;
  typedef boost::shared_ptr<const Player> player_cptr;
}
}
#endif
