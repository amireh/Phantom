#include "message_handler.hpp"
message_handler::message_handler(tcp::socket& in_socket, boost::asio::strand& in_strand, message_parser& in_parser)
: socket_(&in_socket),
  parser_(in_parser),
  strand_(in_strand) {

}

message_handler::~message_handler() {
  socket_ = 0;
}

void message_handler::send(const message &msg, boost::asio::streambuf& out) {
  // calculate expected number of bytes to be sent
  // ...

  //boost::asio::mutable_buffer buf;
  boost::asio::streambuf buf(message::max_length);
  parser_.dump(msg, buf);
  size_t n = socket_->send(buf.data());

  // validate the number of bytes sent
  // ...

  // clear streambuf
  /*std::cout
    << "sent a message with id: " << (int)msg.id
    << ", length:" << msg.length;
  if (msg.body)
    std::cout << " and content: " << strlen(msg.body);

  std::cout << "(" << n << "b actually sent out of " << out.size() << ")\n";*/

  //out.consume(n);
}

void message_handler::recv(boost::asio::streambuf& in) {
  //std::cout << "bytes transferred: " << bytes_transferred << "b\n";
  boost::shared_ptr<message> msg( new message() );
  bool trying_again = false;

  try {
    //boost::asio::streambuf buf(message::max_length);
    parser_.parse(*msg, in);
    //parser_.parse(*msg, buf);
    /*std::cout
      << "parsed msg id: " << (unsigned char)msg->id
      << ", length is : "<< msg->length
      << ", and content is (" << strlen(msg->body) << "b long)";
      //if (msg->body)
      //  std::cout << " => " << msg->body;
      std::cout << "\n";*/

    notify(msg);

  } catch (request_incomplete &e) {
    std::cout << "missing " << e.bytes_left << " bytes\n";
    /*boost::asio::streambuf::mutable_buffers_type bufs = request_.prepare(e.bytes_left);

    size_t n = socket_.receive(bufs);

    request_.commit(n);

    trying_again = true;*/

    /*tmp.resize(bytes_left);
    boost::system::error_code ec;
    std::size_t n = boost::asio::read(
        socket_,
        boost::asio::buffer(tmp),
        boost::asio::transfer_all(), ec);

    if (ec) {
      std::cerr << "couldn't receive the rest of the data ! aborting\n";
      return;
    } else {
      std::cout << "got the remaining data (" << tmp.size() << "b)\n";
    }*/
  } catch (bad_request& e) {
    throw e;
  }

  /*if (trying_again) {
    socket_.get_io_service().post(
      boost::bind(&connection::handle_read, shared_from_this(),
      e, bytes_transferred));

    return;
  }*/

  msg.reset();
}

void message_handler::notify(boost::shared_ptr<message> msg) {
  handlers_t::const_iterator itr = handlers_.find(msg->id);
  if (itr == handlers_.end())
    return;

  for (msg_handler_t handler : itr->second)
    //socket_->get_io_service().post( boost::bind(handler, msg) );
    strand_.post( boost::bind(handler, msg) );
    //socket_->get_io_service().post( strand_.wrap( boost::bind(handler, msg) ) );
}
