#include "message_parser.hpp"
#include "message_handler.hpp"

message_parser::message_parser() {
}

void message_parser::parse(message& msg,  boost::asio::streambuf& in) {
  //std::cout << "reading a request, got " << in.size() << " bytes \n";
  std::istream is(&in);
  std::string line;
  int received_bytes = in.size() - message::header_length - message::footer_length;;

  // we might have already parsed the header, in the case where content was missing
  //if (msg.id != message_id::Unassigned) {
    // parse the message id
    //char tmp_id;
    msg.id = (message_id)is.get();
    //std::cout << "msg id : " << (int)msg.id << "\n";
    is.get();
    //in.consume(1);
    //getline(is,line);
    //msg.id = (message_id)line[0];

    //in.consume(line.size());

    // parse the length
    //getline(is,msg.length);
    //char tmp_length[sizeof(uint16_t)];
    //is.get(tmp_length, sizeof(uint16_t));
    //msg.length = atoi(tmp_length);
    //in.consume(sizeof(uint16_t));
    is >> msg.length;
    //msg.length = atoi(line.c_str());
    //in.consume(line.size());

    //std::cout << "size of body: " << msg.length << "b\n";

    //received_bytes = in.size() - message::header_length - message::footer_length;
  /*} else
    received_bytes = in.size();*/

  // if content length is < what we received, we need to pull more
  if (received_bytes < msg.length) {
    int bytes_left = msg.length - received_bytes + 1;
    throw request_incomplete("need more data", bytes_left);
  }

  //is.seekg(1, std::ios::cur);

  // always create a null-terminated body even if the message has no body
  msg.body = new char[msg.length+1];
  is.read(msg.body, msg.length);
  /*if (!tmp.empty()) {
    int offset = received_bytes-1;
    for (int i=0; i <= tmp.size(); ++i)
      data[offset+i] = tmp[i];

    tmp.clear();
  }*/
  msg.body[msg.length] = '\0';


  //in.consume(msg.length);
  in.consume(message::footer_length); // remove the footer (\r\n\r\n)

  //std::cout << "stream has " << in.size() << "b left\n";
  if (in.size() != 0)
    throw bad_request("");
  //assert(in.size() == 0);
}

void message_parser::dump(message const& msg, boost::asio::streambuf& out) {
  //out.consume(out.size());

  std::ostream stream(&out);
  //boost::asio::mutable_buffer stream(&out);
  stream << (unsigned char)msg.id << "\n";
  stream << (uint16_t)msg.length;
  if (msg.body) // a message might not have a body
    stream << msg.body;
  stream << message::footer;

}


void message_parser::dump(message const& msg, boost::asio::mutable_buffer& out) {
  //out.consume(out.size());

  //std::ostream stream(&out);
  //boost::asio::mutable_buffer stream(&out);
  //out << (unsigned char)msg.id << "\n";
  /*stream << (uint16_t)msg.length;
  if (msg.body) // a message might not have a body
    stream << msg.body;
  stream << message::footer;*/

}
