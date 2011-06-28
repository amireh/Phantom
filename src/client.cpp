#include "client.hpp"

using boost::asio::ip::tcp;

  client::client(boost::asio::io_service& io_service)
    : connected_(false),
      io_service_(io_service),
      resolver_(io_service),
      socket_(io_service),
      server_("127.0.0.1"),
      port_("60100"),
      strand_(io_service),
      message_parser_(),
      message_handler_(socket_, strand_, message_parser_),
      request_(message::max_length),
      response_(message::max_length),
      timer_(io_service_)
  {
    message_handler_.bind(message_id::ping, this, &client::on_ping);
    connect();
    send_req();
    do_read();

    //timer_.expires_from_now(boost::posix_time::seconds(5));
    //timer_.async_wait(boost::bind(&client::disconnect, this, boost::asio::placeholders::error));
    //send_req();
  }

  client::~client() {
    // stop all asynchronous ops
    //socket_.cancel();
    // initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    socket_.close();
  }

  void client::connect() {
    if (!connected_) {

      tcp::resolver::query query(server_, "60100");

      // Start a synchronous resolve
      boost::system::error_code ec;
      tcp::resolver::iterator endpoint = resolver_.resolve(query);
      socket_.connect(*endpoint, ec);

      if (ec) {
        std::cerr << "couldn't connect to server, aborting\n";
        return;
      }

      // set KEEPALIVE to true
      socket_.set_option(boost::asio::socket_base::keep_alive(true));

      std::cout << "connected!\n";
      connected_ = true;

      // Start an asynchronous resolve to translate the server and service names
      // into a list of endpoints.
      /*resolver_.async_resolve(query,
          boost::bind(&client::handle_resolve, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::iterator));*/
    } else {
      std::cerr << "already connected!\n";
    }
  }

  void client::send_req() {
    if (!connected_)
      return;

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::cout << "sending a request\n";

    //request_.consume(request_.size());
    //response_.consume(response_.size());

    std::string body = "";
    for (int i=0; i < 10; ++i)
      body += "Foobar";

    message msg;
    msg.id = message_id::foo;
    msg.body = "Foobar";
    msg.length = msg.body.size();

    std::ostream stream(&request_);
    stream << (unsigned char) msg.id << " " << msg.length;
    socket_.send(request_.data());

    //message_handler_.send(msg, request_);
  }

  void client::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // Attempt a connection to the first endpoint in the list. Each endpoint
      // will be tried until we successfully establish a connection.
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
          boost::bind(&client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error (in handle_resolve): " << err.message() << "\n";
    }
  }

  void client::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      connected_ = true;
      std::cout << "connected to server\n";

      // The connection was successful. Send the request.
      /*boost::asio::async_write(socket_, request_,
          boost::bind(&client::handle_write_request, this,
            boost::asio::placeholders::error));*/
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      std::cout << "connection to that host failed, trying a different endpoint\n";
      // The connection failed. Try the next endpoint in the list.
      socket_.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
          boost::bind(&client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error (in handle_connect): " << err.message() << "\n";
    }
  }


void client::do_read() {
  std::cout << "reading...\n";
  async_read_until( socket_, response_, message::footer,
    strand_.wrap(
      boost::bind(&client::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred)));
}

void client::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e && bytes_transferred != 0) {
    message_handler_.recv(response_);
    do_read();

  } else if (e != boost::asio::error::eof) {
    std::cerr << "an error occured: " << e.message() << "\n";
  }

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}

void client::on_ping(boost::shared_ptr<message> msg) {
  std::cout<<"got PINGED!\n";
  message pong(message_id::pong);
  message_handler_.send(pong, request_);
}

void client::disconnect(const boost::system::error_code& error) {
  message_handler_.send(message(message_id::disconnect), request_);
}
