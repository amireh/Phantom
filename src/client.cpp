#include "client.hpp"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {

  client::client(boost::asio::io_service& io_service)
    : io_service_(io_service),
      conn_(new client_connection(io_service, "127.0.0.1", "60100")),
      timer_(io_service_)
  {
    conn_->connect();
    try {
      conn_->start();
    } catch (std::exception& e) {
      std::cerr << "couldnt start the conn .. " << e.what() << "\n";
      throw e;
    }
    /*message_handler_.bind(message_id::ping, this, &client::on_ping);
    connect();
    send_req();
    do_read();*/

    timer_.expires_from_now(boost::posix_time::seconds(2));
    timer_.wait();
    //timer_.async_wait(boost::bind(&client::disconnect, this, boost::asio::placeholders::error));
    //send_req();
  }

  client::~client() {
    conn_->stop();
    conn_.reset();
    // stop all asynchronous ops
    //socket_.cancel();
    // initiate graceful connection closure.
    //boost::system::error_code ignored_ec;
    //socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    //socket_.close();
  }


/*  void client::send_req() {
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

    message msg(message_id::foo);
    msg.body = "Foobar";
    msg.length = msg.body.size();

    std::ostream stream(&request_);
    stream << (unsigned char) msg.id << " " << (uint16_t)msg.length << " " << msg.body;
    size_t n = socket_.send(request_.data());
    std::cout << " sent " << n << "bytes of data\n";

    //message_handler_.send(msg, request_);
  }*/


/*
void client::do_read() {
  std::cout << "reading...\n";

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
*/
void client::on_ping(const message &msg) {
  std::cout<<"got PINGED!\n";
  conn_->send(message(message_id::pong));
}

}
}
