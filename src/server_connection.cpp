#include "server_connection.hpp"
#include "server.hpp"
#include "PixyUtility.h"

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
    dispatcher_.bind(EventUID::SyncPuppets, this, &connection::on_sync_puppets);
    dispatcher_.bind(EventUID::CreatePuppet, this, &connection::on_create_puppet);
    dispatcher_.bind(EventUID::RemovePuppet, this, &connection::on_remove_puppet);
    dispatcher_.bind(EventUID::UpdateDeck, this, &connection::on_update_deck);
    dispatcher_.bind(EventUID::RemoveDeck, this, &connection::on_remove_deck);
    dispatcher_.bind(EventUID::JoinQueue, this, &connection::on_join_queue);
    dispatcher_.bind(EventUID::JoinLobby, this, &connection::on_join_lobby);
    dispatcher_.bind(EventUID::LeaveQueue, this, &connection::on_leave_queue);
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

    if (is_authentic()) {
      player_->set_online(false);

      if (server::singleton().get_match_finder().already_queued(player_))
        server::singleton().get_match_finder().leave_queue(player_);

      if (player_->get_instance()) {
        // notify instance that player is going down
        player_->get_instance()->on_dropout(player_);
      }
      if (player_->is_in_lobby()) {
        server::singleton().get_lobby()->on_dropout(player_);
      }

      // log out from DB
      server::singleton().get_dbmgr().logout((std::string)player_->get_username());
    }

    std::cout << "Connection: stopped\n";
  }

  void connection::handle_inbound() {
    //~ std::cout << "got an event " << inbound << "\n";
    if (in_instance()) {
      Event clone(inbound);
      clone.Sender = player_;
      player_->get_instance()->enqueue(clone, player_);
    }
    if (in_lobby()) {
      Event clone(inbound);
      clone.Sender = player_;
      server::singleton().get_lobby()->enqueue(clone, player_);
    }

    base_connection::handle_inbound();
  }

  void connection::reject(Event& e)
  {
    e.Feedback = EventFeedback::Error;
    send(e);
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

    std::cout << "Guest wants to login:\n";

    if (!evt.hasProperty("Username") || !evt.hasProperty("Password"))
      return;

    std::string inUsername = evt.getProperty("Username");
    // TODO: use magic/salt to generate passwords (eg concatenate pw + salt and md5sum it)
    std::string _md5pw = MD5((unsigned char*)evt.getProperty("Password").c_str()).hex_digest();
    // __DEBUG__
    std::cout
      << "\tAuthenticating user {" << inUsername
      << "} with password " << _md5pw << "\n";

    // log the player in and send feedback when received
    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.login(
      inUsername,
      _md5pw,
      [&](db_result rc, std::string username) -> void {
        Event evt(EventUID::Login);
        if (rc != db_result::Ok) {
          evt.Feedback = EventFeedback::Error;
          std::cout << "\tGuest " << username << " couldnt log in\n";
        } else {
          std::cout << "\tGuest " << username << " logged in successfully\n";
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
    this->stop();
    //~ strand_.post( [&]() -> void { this->stop(); });
  }

  void connection::on_sync_game_data(const Event &evt) {
    Event resp(EventUID::SyncGameData);
    resp.Options = Event::NoFormat | Event::Compressed;
    resp.setProperty("Data", server::singleton().get_resmgr().get_game_data());
    resp.Feedback = EventFeedback::Ok;
    resp.Rawsize = server::singleton().get_resmgr().get_raw_game_data_size();
    send(resp);
  }

  void connection::on_sync_puppets(const Event&)
  {
    assert(is_authentic());

    std::cout << "loading puppets for player " << player_->get_username() << "\n";

    for (auto puppet : player_->puppets_)
      delete puppet;

    player_->puppets_.clear();

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.sync_puppets(
      player_->get_username(),
      &player_->puppets_,
      [&](db_result rc) -> void {

        Event e(EventUID::SyncPuppets, EventFeedback::Ok, Event::NoFormat);
        if (rc != db_result::Ok || player_->get_puppets().empty()) {
          e.Feedback = EventFeedback::Error;
          std::cerr << "couldn't load puppets for " << player_->get_username() << "\n";
        } else {
          e.Feedback = EventFeedback::Ok;
          // dump the puppets to a stream
          std::ostringstream out;
          server::singleton().get_resmgr().puppets_to_stream(out,
            (const std::list<const Puppet*>&)player_->get_puppets(),
            false /* don't do a full dump */);
          e.setProperty("Data",  out.str());

          //~ std::cout
            //~ << "sent puppet data to " << player_->get_username()
            //~ << ", total of " << player_->get_puppets().size() << " puppets: stream is\n";
          //~ std::cout << out.str() << "\n";
        }
        send(e);
      }
    );
  }

  void connection::on_create_puppet(const Event& e)
  {
    assert(is_authentic() && e.hasProperty("Name") && e.hasProperty("Race"));
    Puppet* tmp = new Puppet();
    tmp->setName(e.getProperty("Name"));
    tmp->setRace(raceFromString(e.getProperty("Race")));
    tmp->setIntelligence(10);
    tmp->setVitality(10);

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.create_puppet(player_->get_username(), tmp,
      [&, tmp](db_result rc) -> void {
        if (rc != db_result::Ok)
        {
          std::cerr
              << "couldn't create puppet " << tmp->getName()
              << ", race : " << tmp->getRace() << "\n";

          delete tmp;
          Event repl(EventUID::CreatePuppet, EventFeedback::Error);
          return send(repl);
        }

        // we need to create the starter decks for this puppet
        std::string deck_name = raceToString(tmp->getRace()) + " Template 1";
        dbmgr.create_template_deck(tmp, deck_name, [&, tmp](db_result _rc) -> void {
          if (_rc != db_result::Ok)
          {
            std::cerr
              << "couldn't create template deck for puppet " << tmp->getName()
              << ", race : " << tmp->getRace() << "\n";
            dbmgr.remove_puppet(player_->get_username(), tmp, [&, tmp](db_result rc) -> void {
              delete tmp;
            });

            Event repl(EventUID::CreatePuppet, EventFeedback::Error);
            return send(repl);
          }
          // everything went OK, reply with the puppet data
          player_->puppets_.push_back(tmp);
          //~ std::ostringstream out;
          //~ server::singleton().get_resmgr().puppets_to_stream(out, tmp, false);
          Event ok(EventUID::CreatePuppet, EventFeedback::Ok, Event::NoFormat);
          send(ok);
          //~ repl.setProperty("Data", out.str());

          // re-sync puppets with the client
          Event repl(EventUID::SyncPuppets, EventFeedback::Ok, Event::NoFormat);
          std::ostringstream out;
          server::singleton().get_resmgr().puppets_to_stream(out,
            (const std::list<const Puppet*>&)player_->get_puppets(),
            false /* don't do a full dump */);
          repl.setProperty("Data",  out.str());

          return send(repl);
        });
    });

  }

  void connection::on_remove_puppet(const Event& e)
  {
    assert(is_authentic() && e.hasProperty("Name"));
    Puppet* p = 0;
    std::string name_ = e.getProperty("Name");
    for (auto puppet : player_->puppets_)
      if (puppet->getName() == name_)
      {
        p = puppet;
        name_ = puppet->getName();
      }

    if (!p)
    {
      std::cerr << "no such puppet found for removal: " << name_ << " by " << player_->get_username() << "\n";
      Event error(e);
      return reject(error);
    }

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.remove_puppet(
      player_->get_username(),
      p,
      [&, p, e](db_result rc) -> void {
      if (rc != db_result::Ok)
      {
        std::cerr << "couldn't remove puppet from DB " << p->getName() << "\n";
        Event error(e);
        return reject(error);
      }

      // remove the puppet from the player's list
      {
        player_->puppets_.remove(p);
        delete p;
      }

      Event ok(EventUID::RemovePuppet, EventFeedback::Ok);
      send(ok);

      // resync puppets
      Event repl(EventUID::SyncPuppets, EventFeedback::Ok, Event::NoFormat);
      std::ostringstream out;
      server::singleton().get_resmgr().puppets_to_stream(out,
        (const std::list<const Puppet*>&)player_->get_puppets(),
        false /* don't do a full dump */);
      repl.setProperty("Data",  out.str());

      return send(repl);
    });
  }

  void connection::on_join_queue(const Event& evt) {
    assert(player_ && is_authentic() && evt.hasProperty("D"));

    //if (!evt.hasProperty("Puppet"))
    //  return;

    // make sure the puppet has that deck
    std::string deck_name = evt.getProperty("D");
    assert(player_->get_puppet()->hasDeck(deck_name));

    Deck *deck = 0;
    Puppet::decks_t const& decks = player_->get_puppet()->getDecks();
    for (Puppet::decks_t::const_iterator itr = decks.begin();
      itr != decks.end();
      ++itr)
    {
      if ((*itr)->getName() == deck_name)
      {
        deck = (Deck*)*itr;
        break;
      }
    }

    player_->get_puppet()->setDeck(deck);
    server::singleton().get_match_finder().join_queue(player_);

   /* std::string puppet_name = evt.getProperty("Puppet");

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

		player_->set_puppet(puppet);*/
  }

  bool connection::in_instance() const {
    return (is_authentic() && player_->is_in_instance());
  }

  bool connection::in_lobby() const {
    return (is_authentic() && player_->is_in_lobby());
  }

  bool connection::is_authentic() const {
    return (player_);
  }

  void connection::on_join_lobby(const Event& e)
  {
    if (!is_authentic())
    {
      std::cerr << "ERROR!! unauthorized attempt to join lobby!!\n";
      return;
    }

    assert(e.hasProperty("Puppet"));

    std::string puppet_name = e.getProperty("Puppet");

    // find the puppet
    Puppet* tmp = 0;
    for (auto puppet : player_->puppets_)
    {
      if (puppet->getName() == puppet_name)
      {
        tmp = puppet;
        break;
      }
    }

    assert(tmp);
    tmp = 0;

		// load the puppet
		puppet_ptr puppet( new Puppet() );
    player_->set_puppet(puppet);

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.load_puppet(
      puppet_name,
      *puppet,
      [&](db_result rc) -> void {
        Event e(EventUID::JoinLobby, EventFeedback::Ok);

        if (rc > db_result::Ok) {
          e.Feedback = EventFeedback::Error;
          throw BadEvent(std::string("unknown puppet named: ") + player_->get_puppet()->getName());
        }

        // sync puppet
        Event repl(EventUID::SyncPuppetData, EventFeedback::Ok, Event::NoFormat);
        std::ostringstream out;
        server::singleton().get_resmgr().puppets_to_stream(out,
          player_->get_puppet().get(),
          true /* do a full dump */);
        repl.setProperty("Data",  out.str());
        send(repl);

        e.setProperty("P", player_->get_puppet()->getName());
        player_->send(e);

        server::singleton().get_lobby()->enlist(player_);
      }
    );
  }

  void connection::on_update_deck(const Event& e)
  {
    assert(is_authentic() && player_->get_puppet() && e.hasProperty("Data"));

    // validate the spells
    sresource_manager& resmgr = server::singleton().get_resmgr();
    bool valid = true;
    std::vector<std::string> elements = Utility::split(e.getProperty("Data"), ',');
    assert(elements.size() == 17);

    std::string deck_name = elements.front();
    elements.erase(elements.begin());
    for (std::vector<std::string>::iterator element = elements.begin();
      element != elements.end();
      ++element)
    {
      valid = valid && (resmgr.getSpell(*element, player_->get_puppet()->getRace()) != NULL);
      if (!valid)
        break;

      (*element) = "\"" + (*element) + "\"";
    }

    if (!valid)
    {
      Event error(e);
      error.Feedback = EventFeedback::Error;
      return send(error);
    }

    std::string buffer = Utility::join(elements, ',');
    buffer = "{" + buffer + "}";

    // it's valid, tell the database manager to update or create the deck
    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.create_or_update_deck(
      player_->get_puppet().get(),
      deck_name,
      buffer,
      [&](db_result rc) -> void {
      if (rc != db_result::Ok)
      {
        Event error(e);
        error.Feedback = EventFeedback::Error;
        return send(error);
      }

      Event reply(EventUID::UpdateDeck, EventFeedback::Ok);
      send(reply);

      // sync puppet
      Event repl(EventUID::SyncPuppetData, EventFeedback::Ok, Event::NoFormat);
      std::ostringstream out;
      server::singleton().get_resmgr().puppets_to_stream(out,
        player_->get_puppet().get(),
        true /* do a full dump */);
      repl.setProperty("Data",  out.str());
      send(repl);
    });
  }

  void connection::on_remove_deck(const Event& e)
  {
    assert(is_authentic() && e.hasProperty("Name"));
    Deck* d = 0;
    std::string name_ = e.getProperty("Name");
    for (auto deck : player_->get_puppet()->getDecks())
      if (deck->getName() == name_)
      {
        d = (Deck*)deck;
        name_ = deck->getName();
      }

    if (!d)
    {
      std::cerr << "no such deck found for removal: " << name_ << " by " << player_->get_username() << "\n";
      Event error(e);
      return reject(error);
    }

    db_manager& dbmgr = server::singleton().get_dbmgr();
    dbmgr.remove_deck(
      player_->get_puppet()->getName(),
      name_,
      [&, name_](db_result rc) -> void {
      if (rc != db_result::Ok)
      {
        std::cerr << "couldn't remove deck from DB " << name_ << "\n";
        Event error(EventUID::RemoveDeck, EventFeedback::Error);
        return reject(error);
      }

      // remove the deck from the player's list
      {
        player_->get_puppet()->removeDeck(name_);
      }

      Event ok(EventUID::RemoveDeck, EventFeedback::Ok);
      send(ok);

      // resync puppets
      Event repl(EventUID::SyncPuppetData, EventFeedback::Ok, Event::NoFormat);
      std::ostringstream out;
      server::singleton().get_resmgr().puppets_to_stream(out,
        player_->get_puppet().get(),
        true /* do a full dump */);
      repl.setProperty("Data",  out.str());
      return send(repl);
    });
  }

  void connection::on_leave_queue(const Event& e)
  {
    assert(is_authentic());

    server::singleton().get_match_finder().leave_queue(player_);
  }

} // namespace Net
} // namespace Pixy
