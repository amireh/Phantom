/*
 *  Copyright (c) 2011 Ahmad Amireh <ahmad@amireh.net>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#include "server.hpp"
#include "server_connection.hpp"
#include "db_manager.hpp"
#include "instance.hpp"
#include <ctime>
#include <csignal>

namespace Pixy {
namespace Net {

  server* server::__instance = 0;

  server::server()
    : io_service_(),
      acceptor_(io_service_),
      strand_(io_service_),
      //~ work_(io_service_),
      thread_pool_size_(4),

      new_connection_(),
      nr_connections_(0),

      ping_interval(5),
      ping_timer_(io_service_),

      dbmgr_(0),
      resmgr_(0),
      match_finder_(0),

      new_instance_(),
      nr_instances_(0)
  {
    time(&uptime);

  }

  server& server::singleton() {
    if (!__instance)
      __instance = new server();

    return *__instance;
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	accessors
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
  std::string const& server::get_root_path() {
    return root_path_;
  };
  std::string const& server::get_data_path() {
    return data_path_;
  };
  std::string const& server::get_bin_path() {
    return bin_path_;
  };

  db_manager& server::get_dbmgr() {
    return *dbmgr_;
  }

  sresource_manager& server::get_resmgr() {
    return *resmgr_;
  }
  match_finder& server::get_match_finder() {
    return *match_finder_;
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	bootstrap
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
  void server::init_logger() {

    if (!boost::filesystem::exists(log_path_))
      boost::filesystem::create_directory(log_path_);

    log_appender_ =	new log4cpp::FileAppender("FileAppender", log_path_ + "/EServer.log", false);
    log_layout_ = new PixyLogLayout();
    log_appender_->setLayout(log_layout_);
    log_category_ = &log4cpp::Category::getInstance(PIXY_LOG_CATEGORY);

    log_category_->setAdditivity(false);
    log_category_->setAppender(log_appender_);
    log_category_->setPriority(log4cpp::Priority::DEBUG);

    log_ = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "Server");

    static_cast<PixyLogLayout*>(log_layout_)->setVanilla(true);
    log_->infoStream() << "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+";
    log_->infoStream() << "\n+                        Elementum v" << ELEMENTUM_VERSION << " - Server Log                          +";
    log_->infoStream() << "\n+                      (http://www.elementum-game.com)                        +";
    log_->infoStream() << "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
    static_cast<PixyLogLayout*>(log_layout_)->setVanilla(false);

    log_appender_->setLayout(log_layout_);

    log_category_ = 0;
    log_layout_ = 0;
    log_appender_ = 0;
  }

  void server::resolve_paths() {
    using boost::filesystem::path;
    using boost::filesystem::create_directory;
    using boost::filesystem::is_directory;

    // locate the binary and build its path
    // use binreloc and boost::filesystem to build up our paths
    int brres = br_init(0);
    if (brres == 0) {
      std::cerr << "binreloc could not be initialised\n";
    }
    char *p = br_find_exe_dir(".");
    bin_path_ = std::string(p);
    free(p);
    bin_path_ = path(bin_path_).make_preferred().string();

    // root is ../
    path lRoot = path(bin_path_);
    for (int i=0; i < 1; ++i) {
      lRoot = lRoot.remove_leaf();
    }

    root_path_ = lRoot.make_preferred().string();
    data_path_ = (path(root_path_) / path(ESERVER_DATA_DIR)).make_preferred().string();
    log_path_ = (path(root_path_) / path(ESERVER_LOG_DIR)).make_preferred().string();

    if (!is_directory(path(log_path_).root_directory()))
      create_directory(path(log_path_).root_directory());
    if (!is_directory(path(data_path_)))
      create_directory(path(data_path_));

    std::cout << "Binary path: " <<  bin_path_ << "\n";
    std::cout << "Root path: " <<  root_path_ << "\n";
    std::cout << "Data path: " <<  data_path_ << "\n";
    std::cout << "Log path: " <<  log_path_ << "\n";

  };

  static void catch_sigusr(int sig) {
    server::singleton().dump_stats(sig);
  }
  void server::run()
  {

    resolve_paths();
    init_logger();

    std::string _ip("0.0.0.0");
    std::string _port(SERVER_PORT);

    std::cout << "-+-+-+-+-+-+ Elementum Server +-+-+-+-+-+-\n";
    std::cout << "Binding at: " << _ip << ":" << _port << "\n";
    std::cout << "-+-+-+-+\n";

    new_connection_.reset(new connection(io_service_));

    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(_ip/*SERVER_ADDRESS*/, _port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    // accept connections
    acceptor_.async_accept(
      new_connection_->socket(),
      boost::bind(&server::handle_accept, this, boost::asio::placeholders::error));

    //signal(SIGUSR1, &server::dump_stats);
    signal(SIGUSR1, &catch_sigusr);
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
      workers.create_thread(boost::bind(&server::work, boost::ref(this)));

    std::cout
      << "Server up, accepting connections on " << thread_pool_size_ << " threads\n";
    std::cout
      << "Messages header length: " << Event::HeaderLength << ", footer length: " << Event::FooterLength << "\n";

    dbmgr_ = new db_manager(io_service_);
    dbmgr_->connect();

    resmgr_ = new sresource_manager();
    resmgr_->dump();

    match_finder_ = new match_finder(io_service_);

    // start our player ping timer
    //refresh_timer();

    // wait for all threads in the pool to exit
    workers.join_all();

    // clean up
    delete match_finder_;
    delete resmgr_;
    delete dbmgr_;

    if (log_) {
      delete log_appender_;
      delete log_category_;
      delete log_layout_;
      delete log_;
    }

    log4cpp::Category::shutdown();

    //~ throw std::runtime_error("shouldn't be here, all workers are dead!");
  }

  void server::work() {
    //~ try {
      io_service_.run();
    //~ } catch (std::exception& e) {
      //~ std::cerr << "an exception caught in a worker, aborting: " << e.what() << "\n";
      //~ throw e;
    //~ }

    std::cout << "worker thread exiting\n";
  }

  void server::stop() {
    std::cout << "Server: Shutting down gracefully, waiting for current jobs to finish. Please wait.\n";
    strand_.post( [&]() -> void {
      for (connection_ptr conn : connections)
        conn->stop();
      std::cout << "Server: Going down.\n";
      connections.clear();

      new_connection_.reset();

      io_service_.stop();
      std::cout << "Server: Down cleanly.\n";
    });
  }
  /* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	monitoring
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
  void server::dump_stats(int sig) {
    std::ofstream out(data_path_ + "/stats.out");
    if (!out.is_open() || !out.good())
      return;

    // 1) uptime
    struct tm * timeinfo = localtime(&uptime);
    std::string uptime_s = asctime(timeinfo);
    time_t nowtime;
    time(&nowtime);
    timeinfo = localtime(&nowtime);
    std::string nowtime_s = asctime(timeinfo);

    out << uptime_s;
    out << nowtime_s;

    // 2) total connections since uptime
    out << nr_connections_ << "\n";
    // 3) total current connections
    out << connections.size() << "\n";
    // 4) total instances since uptime
    out << nr_instances_ << "\n";
    // 5) total current instances
    int nr_current_instances = 0;
    for (auto conn : connections)
      if (conn->in_instance())
        ++nr_current_instances;

    out << nr_current_instances/2 << "\n";

    out.close();
  }

	/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *
	 *	main routines
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
   void server::handle_accept(const boost::system::error_code &e) {
    if (!e)
    {
      connections.push_back(new_connection_);
      ++nr_connections_;

      log_->debugStream() << "a guest has connected";

      new_connection_->start();
      new_connection_.reset(new connection(io_service_));
      acceptor_.async_accept(new_connection_->socket(),
          boost::bind(&server::handle_accept, this,
            boost::asio::placeholders::error));
    } else {
      log_->errorStream() << "couldn't accept connection! " << e << "\n";
      throw std::runtime_error("unable to accept connection!");
    }
  }

  void server::_close(connection_ptr conn) {
    strand_.post( [&, conn]() { connections.remove(conn); });
  }

  void server::ping_clients(const boost::system::error_code& error) {
    if (!connections.empty()) {
      std::cout << "Server: pinging clients\n";
      for (connection_ptr conn : connections) { conn->ping(); }
    }

    strand_.post( boost::bind(&server::refresh_timer, this) );
  }

  void server::refresh_timer() {
    ping_timer_.expires_from_now(boost::posix_time::seconds(ping_interval));
    ping_timer_.async_wait( strand_.wrap(boost::bind(&server::ping_clients, this, boost::asio::placeholders::error)));
  }


  void server::_launch_instance(std::list<player_cptr> players) {
    std::cout << "starting an instance\n";
    strand_.post( [&, players]() -> void {
      new_instance_.reset(new instance(players, io_service_));
      new_instance_->bootstrap();
      instances_.push_back(new_instance_);
      new_instance_.reset();

      ++nr_instances_;
    });
  }
  void server::_shutdown_instance(instance_ptr in_instance) {
    strand_.post( [&, in_instance] { instances_.remove(in_instance); } );
  }

} // namespace Net
} // namespace Pixy
