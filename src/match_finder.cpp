#include "match_finder.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

	match_finder::match_finder(boost::asio::io_service& io_service)
  : strand_(io_service) {
		log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "match_finder");
    log_->infoStream() << "up and running";
	}

	match_finder::~match_finder() {
    log_->infoStream() << "shutting down";

		if (log_)
			delete log_;

		log_ = 0;
	}

	bool match_finder::already_queued(player_cptr in_player) {
		// look for matches
    for (auto player : seekers_)
      if (player == in_player)
        return true;

    return false;
	}

  void match_finder::join_queue(player_cptr player) {
    strand_.post( boost::bind(&match_finder::do_join_queue, this, player) );
  }
  void match_finder::leave_queue(player_cptr player) {
    strand_.post( boost::bind(&match_finder::do_leave_queue, this, player) );
  }

	void match_finder::do_leave_queue(player_cptr player) {
		std::cout << "puppet " << player->get_puppet()->getName() << " has left the queue\n";
    seekers_.remove(player);
	}

	void match_finder::do_join_queue(player_cptr player) {
    std::cout << "a player is attempting to join the queue\n";

		if (already_queued(player)) {
			log_->warnStream() << "player " << player->get_username() << " is already queued";
			return;
		}

		seekers_.push_back(player);

    player->send(Event(EventUID::JoinQueue, EventFeedback::Ok));

    std::cout
      << "a puppet has joined the queue: "
      << player->get_puppet()->getName() << ", there are "
      << seekers_.size() << " players in queue" << "\n";

    strand_.post( boost::bind(&match_finder::seek, this) );
	}



	void match_finder::seek() {
    if (seekers_.size() < 2)
      return;

    std::cout << "match finder: finding a match between players\n";
		player_cptr player = seekers_.front();

		// look for matches
    for (auto opponent : seekers_)
      if (is_eligible(player, opponent)) {
        start_match(player, opponent);
       // strand_.post( boost::bind(&match_finder::start_match, this, player, opponent) );
        break;
      }

	}

	void match_finder::start_match(player_cptr player1, player_cptr player2) {

    // has any of them gone offline?
    bool success = (player1->is_online() && player2->is_online());

    if (!success) {
      std::cerr << "aborting match: a player seems to have dropped out, re-seeking...\n";
      // requeue the player who's still online
      if (player1->is_online())
        do_join_queue(player1);
      else
        do_join_queue(player2);
      //strand_.post( boost::bind(&match_finder::seek, this) );
      return;
    }

    assert(player1->get_puppet() && player2->get_puppet());

    log_->infoStream()
    << "starting match between "
    << player1->get_puppet()->getName() << " and " << player2->get_puppet()->getName();

    Event evt(EventUID::MatchFound);

		player1->send(evt);
		player2->send(evt);

    std::list<player_cptr> players{player1, player2};

		server::singleton()._launch_instance(players);

    // remove the players from the queue
    seekers_.remove(player1);
    seekers_.remove(player2);
	}

  bool match_finder::is_eligible(player_cptr p1, player_cptr p2) {
    bool eligible = true;
    if (p1 == p2)
      return false;

    return eligible;
  }

}
}
