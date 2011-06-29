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

#include "message_parser.hpp"
#include "message_handler.hpp"

namespace Pixy {
namespace Net {

message_parser::message_parser() {
}

bool message_parser::parse_header(message& msg,  boost::asio::streambuf& in) {

  if (in.size() < message::header_length) {
    std::cerr << "message size is smaller than header!!\n";
    //in.consume(in.size());
    return false;
  }

  std::cout << "received " << in.size() << "bytes of data\n";

  char sp1;
  unsigned char msg_id;
  char msg_length[sizeof(uint16_t)];
  std::istream is(&in);
  is.unsetf(std::ios_base::skipws);
  is >> msg_id >> sp1 >> msg_length >> sp1;

  msg.id = (message_id)msg_id;
  msg.length = atoi(msg_length);

  std::cout<< "msg id=" << (unsigned char)msg.id << ", length=" << msg.length << "(" << msg_length << ")\n";

  //assert(msg.id > message_id::unassigned && msg.id < message_id::placeholder);
  //assert(atoi(msg_length) <= message::max_length);

  if (msg.id > message_id::unassigned && msg.id < message_id::placeholder && msg.length <= message::max_length)
    return true;

  return false;
}

bool message_parser::parse_body(message& msg, boost::asio::streambuf& in) {
  int bytes_received = in.size();
  int bytes_left = msg.length - msg.body.size() - bytes_received;
  int bytes_to_process = (bytes_received > msg.length) ? msg.length : bytes_received;

  std::cout << "processing " << bytes_to_process <<"b from " << bytes_received << " and there is " << bytes_left << "missing\n";
  // read until either the message length is reached or the buffer is empty
  std::istream is(&in);
  while (msg.body.size() != msg.length && in.size() > 0) {
    msg.body.push_back(is.get());
  }


  //std::cout << "body now is " << msg.body.size() << "bytes long and has " << msg.body << "\n";
  return bytes_left <= 0;
}


bool message_parser::parse_all(message& msg, boost::asio::streambuf& in) {

  bool success = false;
  if (parse_header(msg, in)) {
    success = parse_body(msg, in);
  }

  return success;
}

void message_parser::dump(message const& msg, boost::asio::streambuf& out) {

  std::cout << "pre-message dump: buffer has " << out.size() << "(expected 0), ";
  std::ostream stream(&out);
  stream << (unsigned char)msg.id << " ";
  stream << (uint16_t)msg.body.size() << " ";
  if (!msg.body.empty()) // a message might not have a body
    stream << msg.body;

  stream << message::footer;
  std::cout << "post-dump: " << out.size() << "\n";

  //out.prepare(sizeof(unsigned char) + 2 + sizeof(uint16_t) + msg.body.size());
}




/* ----- event handling ----- */

bool message_parser::parse_header(Event& evt,  boost::asio::streambuf& in) {

  if (in.size() < message::header_length) {
    std::cerr << "message size is smaller than header!!\n";
    return false;
  }

  std::cout << "received " << in.size() << "bytes of data\n";

  char sp1;
  unsigned char uid, feedback;
  char length[sizeof(uint16_t)];

  std::istream is(&in);
  is.unsetf(std::ios_base::skipws);
  is >> uid >> sp1 >> length >> sp1 >> feedback >> sp1;

  evt.UID = (EventUID)uid;
  evt.Length = atoi(length);
  evt.Feedback = (EventFeedback)feedback;

  std::string pn, pv;
  bool delim_found = false;
  char c;
  while (is.good()) {
    //if (pn.empty()) { // parse the property name
      while (true) {
        is.get(c);
        if (c == ' ')
          break;

        pn.push_back(c);
      }
    //} else { // parse the property value
      while (true) {
        is.get(c);
        if (c == ' ')
          break;

        pv.push_back(c);
      }
      evt.setProperty(pn, pv);
      pn.clear();
      pv.clear();
    //}
  }
  //std::cout<< "msg id=" << (unsigned char)msg.id << ", length=" << msg.length << "(" << msg_length << ")\n";

  //assert(msg.id > message_id::unassigned && msg.id < message_id::placeholder);
  //assert(atoi(msg_length) <= message::max_length);

  if (evt.UID > EventUID::Unassigned && evt.UID < EventUID::SanityCheck && evt.Length <= Event::MaxLength)
  //  return true;

  return false;
}
}
}
