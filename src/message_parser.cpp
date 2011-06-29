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
/*
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

*/

  /* splits a string s using the delimiter delim */

  std::vector<std::string>
  split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
  }
  bool message_parser::parse(Event& evt,  boost::asio::streambuf& in) {

    int bytes_received = in.size();
    if (bytes_received < Event::HeaderLength + Event::FooterLength) {
      std::cerr << "message is too short!!\n";
      return false;
    }

    std::cout << "received " << bytes_received << "bytes of data\n";

    char sp1;
    unsigned char uid, feedback, flags;
    uint16_t length;

    std::istream is(&in);
    is.unsetf(std::ios_base::skipws);
    is >> uid >> flags >> feedback >> length;

    evt.UID = (EventUID)uid;
    evt.Options = flags;
    evt.Feedback = (EventFeedback)feedback;
    evt.Length = length;

    // check header's sanity
    if ((evt.UID < EventUID::Unassigned || evt.UID > EventUID::SanityCheck)
        || (evt.Length > Event::MaxLength)
        || (evt.Feedback < EventFeedback::Unassigned || evt.Feedback > EventFeedback::InvalidRequest)) {
      std::cerr << "request failed header sanity check\n";
      return false;
    }

    // there must be N+sizeof(int) bytes of properties where N > in_bytes - signature
    if (evt.Length > 0 && (bytes_received - Event::HeaderLength - Event::FooterLength < evt.Length+sizeof(int))) {
      std::cerr << "invalid properties length: " << evt.Length << "\n";
      return false;
    }

    std::cout << "event uid : " << (int)evt.UID << " and length: " << evt.Length << "\n";

    // parse properties
    if (evt.Length > 0) {
      int checksum;
      is >> checksum;

      char* props = new char[evt.Length+1];
      is.rdbuf()->sgetn(props, evt.Length);
      props[evt.Length] = '\0';

      // verify CRC checksum
      evt.Checksum = Event::_CRC32(std::string(props));
      if (evt.Checksum != checksum) {
        std::cerr << "CRC mismatch, aborting: " << evt.Checksum << " vs " << checksum << " for " << props << "\n";
        delete props;
        return false;
      }

      if ((evt.Options & Event::NoFormat) == Event::NoFormat) {
        evt.setProperty("Data", props);

        std::cout << "oh loook its an unformatted msg: " << props << "\n";
      } else {
        std::vector<std::string> elems = split(props, ',');
        assert(elems.size() > 0 && elems.size() % 2 == 0);

        for (int i=0; i < elems.size(); i+=2)
          evt.setProperty(elems[i], elems[i+1]);
      }

      delete props;
    }

    // skip the footer
    std::cout << in.size() << " bytes left\n";
    assert(in.size() == Event::FooterLength);
    in.consume(Event::FooterLength);

    evt.dump();

    return true;
  }

  void message_parser::dump(Event const& evt, boost::asio::streambuf& out) {

    //std::cout << "pre-message dump: buffer has " << out.size() << "(expected 0), ";
    std::ostream stream(&out);

    std::string props = "";
    if (!evt.Properties.empty()) {
      // if the event is raw, we've to dump only one property
      if ((evt.Options & Event::NoFormat) == Event::NoFormat) {
        // must have this, otherwise discard
        //assert(evt.hasProperty("Data"));
        if (evt.hasProperty("Data"))
          props = evt.getProperty("Data");
      } else {
        // flatten properties
        for (auto property : evt.Properties)
          props += property.first + "," + property.second + ",";

        props.erase(props.end()-1);
      }
    }

    stream << (unsigned char)evt.UID;
    stream << evt.Options;
    stream << (unsigned char)evt.Feedback;
    stream << (uint16_t)props.size();
    if (!props.empty()) {
      stream << Event::_CRC32(props);
      stream << props;
    }
    stream << Event::Footer;

    //std::cout << "post-dump: " << out.size() << "\n";
  }
}
}
