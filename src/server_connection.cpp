#include "server_connection.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

  connection::connection(boost::asio::io_service& io_service)
    : base_connection(io_service),
      //pingmsg_(message_id::ping),
      pingevt_(EventUID::Ping),
      ping_timeouts_(0),
      player_()
  {
    dispatcher_.bind(EventUID::Pong, this, &connection::on_pong);
    dispatcher_.bind(EventUID::Login, this, &connection::on_login);
    dispatcher_.bind(EventUID::Logout, this, &connection::on_logout);
    dispatcher_.bind(EventUID::SyncGameData, this, &connection::on_sync_game_data);
    dispatcher_.bind(EventUID::JoinQueue, this, &connection::on_join_queue);
  }

  connection::~connection() {
    if (player_) {
      // remove player from match queue

      // remove from instance

      // stop referencing it

      std::cout
        << "Connection: my player " << player_->get_username() << " has "
        << player_.use_count() << " users\n";
    }

    player_.reset();

    std::cout << "Connection: destroyed\n";
  }

  void connection::stop() {
    base_connection::stop();

    strand_.post( boost::bind(&server::_close, &server::singleton(), shared_from_this()));

    if (player_) {
      player_->set_online(false);

      if (server::singleton().get_match_finder().already_queued(player_))
        server::singleton().get_match_finder().leave_queue(player_);

      if (player_->get_instance()) {
        // notify instance that player is going down
        player_->get_instance()->on_dropout(player_);
      }

      // log out from DB
      server::singleton().get_dbmgr().logout((std::string)player_->get_username());
    }
  }

  void connection::handle_inbound() {
    if (in_instance()) {
      Event clone(inbound);
      std::cout << "got an event from " << player_->get_username() << "\n";
      clone.Sender = player_;
      player_->get_instance()->enqueue(clone, player_);
    }

    base_connection::handle_inbound();
  }

  void connection::ping() {
    send(pingevt_);

    if ((++ping_timeouts_) >= 3)
      strand_.post( boost::bind(&connection::stop, shared_from_this()));

    /*try {
      std::cout << "pinging client... ";
      size_t n = socket_.send(pinger_.data());
      std::cout << "sent " << n << "bytes\n";
    } catch (std::exception& e) {
      std::cerr << "ping timeout, client possibly disconnected\n";
      if (++ping_timeouts_ == 3) {
        strand_.post( boost::bind(&connection::stop, shared_from_this()));
      }
    }*/
  }

  void connection::on_pong(const Event &evt) {
    std::cout<<"got PONGED!\n";
    ping_timeouts_ = 0;
  }

  void connection::on_login(const Event &evt) {
    assert(!is_authentic());

    std::cout << "guest wants to login\n";

    if (!evt.hasProperty("Username") || !evt.hasProperty("Password"))
      return;

    std::string inUsername = evt.getProperty("Username");
    // TODO: use magic/salt to generate passwords (eg concatenate pw + salt and md5sum it)
    std::string _md5pw = MD5((unsigned char*)evt.getProperty("Password").c_str()).hex_digest();
    // __DEBUG__
    std::cout
      << "authenticating user " << inUsername
      << " with password " << " hashed " << _md5pw << "\n";

    // log the player in and send feedback when received
    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.login(
      inUsername,
      _md5pw,
      [&](db_result rc, std::string username) -> void {
        Event evt(EventUID::Login);
        if (rc != db_result::Ok) {
          evt.Feedback = EventFeedback::Error;
          std::cout << "client " << username << " couldnt log in\n";
        } else {
          std::cout << "client " << username << " logged in successfully\n";
          evt.Feedback = EventFeedback::Ok;
          promote(username);
        }

        send(evt);
      }
    );
  }

  void connection::promote(std::string& username) {
    assert(!is_authentic());

    player_.reset( new Player(this, username) );
    player_->set_online(true);
  }

  void connection::on_logout(const Event &evt) {
    std::cout << "client disconnecting\n";
    stop();
  }

  void connection::on_sync_game_data(const Event &evt) {
    Event resp(EventUID::SyncGameData);
    resp.Options = Event::NoFormat | Event::Compressed;
    resp.setProperty("Data", server::singleton().get_resmgr().get_game_data());
    resp.Feedback = EventFeedback::Ok;
    resp.Rawsize = server::singleton().get_resmgr().get_raw_game_data_size();
    send(resp);
  }

  void connection::on_join_queue(const Event& evt) {
    assert(player_);

    if (!evt.hasProperty("Puppet"))
      return;

    std::string puppet_name = evt.getProperty("Puppet");

		// load the puppet
		puppet_ptr puppet( new Puppet());
    puppet->setName(puppet_name);

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.load_puppet(
      puppet_name,
      *puppet,
      [&](db_result rc) -> void {
        if (rc > db_result::Ok) {
          throw BadEvent(std::string("unknown puppet named: ") + player_->get_puppet()->getName());
        }

        server::singleton().get_match_finder().join_queue(player_);
      }
    );

		player_->set_puppet(puppet);
  }

  bool connection::in_instance() const {
    return (player_ && player_->get_instance());
  }

  bool connection::is_authentic() const {
    return (player_);
  }

} // namespace Net
} // namespace Pixy
