#include "server_connection.hpp"
#include "server.hpp"

namespace Pixy {
namespace Net {

  connection::connection(boost::asio::io_service& io_service)
    : base_connection(io_service),
      //pingmsg_(message_id::ping),
      pingevt_(EventUID::Ping),
      ping_timeouts_(0),
      player_(0)
  {
    message_handler_.bind(EventUID::Pong, this, &connection::on_pong);
    message_handler_.bind(EventUID::Login, this, &connection::on_login);
    message_handler_.bind(EventUID::SyncGameData, this, &connection::on_sync_game_data);
  }

  connection::~connection() {
    if (player_)
      delete player_;

    std::cout << "Connection: destroyed\n";
  }

  void connection::stop() {
    base_connection::stop();

    strand_.post( boost::bind(&server::_close, &server::singleton(), shared_from_this()));
  }

  void connection::ping() {
    send(pingevt_);
    ++ping_timeouts_;
    if (ping_timeouts_ >= 3)
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
    std::cout << "guest wants to login\n";

    if (!evt.hasProperty("Username") || !evt.hasProperty("Password"))
      return;

    db_manager& dbmgr = server::singleton().get_dbmgr();
    bool success = true;
    std::string inUsername, inPassword;

    inUsername = evt.getProperty("Username");
    // TODO: use magic/salt to generate passwords (eg concatenate pw + salt and md5sum it)
    std::string _md5pw = MD5((unsigned char*)evt.getProperty("Password").c_str()).hex_digest();
    std::cout
      << "authenticating user " << inUsername
      << " with password " << inPassword << " hashed " << _md5pw << "\n";

    dbmgr.login(inUsername, _md5pw, boost::bind(&connection::on_login_feedback, this, _1, _2) );
  }

  void connection::on_login_feedback(db_result rc, std::string username) {
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

  void connection::promote(std::string& username) {
    assert(!player_);

    player_ = new Player();
    //player_->setUserId(inUserId);
    //player_->setGUID(inGuest);
    player_->setUsername(username);
    player_->setIsOnline(true);
  }

  void connection::on_disconnect(const Event &evt) {
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

} // namespace Net
} // namespace Pixy
