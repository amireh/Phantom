#include "event.hpp"
#include "PixyUtility.h"

namespace Pixy {
namespace Net {

  const char* Event::Footer = "\r\n\r\n";
	Event::Event() {
    reset();
  }

  Event::Event(const EventUID inUID)
  : UID(inUID),
    Options(0),
    Feedback(EventFeedback::Unassigned),
    Length(0),
    Checksum(0)
  {
		Properties.clear();
	}

	Event::~Event()
  {
		//std::cout << "Event " << mName << " destroyed\n";
		reset();
  }

	void Event::reset() {
    UID = EventUID::Unassigned;
    Options = 0;
		Feedback = EventFeedback::Unassigned;
    Length = 0;
    Checksum = 0;
    Rawsize = 0;
    Properties.clear();
	}

	Event::Event(const Event& src) {
		_clone(src);
	}

	Event& Event::operator=(const Event& rhs) {
		if (this != &rhs)
			_clone(rhs);

		return *this;
	}

	void Event::_clone(const Event& src) {
		reset();

		this->UID = src.UID;
    this->Options = src.Options;
		this->Feedback = src.Feedback;
    this->Length = src.Length;
    this->Rawsize = src.Rawsize;
    this->Checksum = src.Checksum;

    if (!src.Properties.empty())
      for (auto property : src.Properties)
        setProperty(property.first, property.second);
	}

	void Event::setProperty(const std::string inName, const std::string inValue) {
		if ( hasProperty(inName) ) {
			Properties.find(inName)->second = inValue;
			return;
		}

		Properties.insert( std::make_pair(inName, inValue) );
	}

	std::string const& Event::getProperty(const std::string inName) const {
		return Properties.find(inName)->second;
	}

	bool Event::hasProperty(const std::string inName) const {
		return (Properties.find(inName) != Properties.end());
	}

	void Event::dump(std::ostream& inStream) const {
		inStream
      << "uid: " << (int)UID << "\n"
      << "unformatted? " << ((Options & Event::NoFormat) == Event::NoFormat ? "yes" : "no")
      << ", compressed? " << ((Options & Event::Compressed) == Event::Compressed ? "yes" : "no")
      << ", broadcast? " << ((Options & Event::Broadcast) == Event::Broadcast ? "yes" : "no")
      << ", local? " << ((Options & Event::IsLocal) == Event::IsLocal ? "yes" : "no")
      << "\n"
      << "feedback: " << (Feedback == EventFeedback::Error ? "error" : "ok")
        << "(" << (int)Feedback << ")" << "\n"
      << "length: " << Length << "\n"
      << "CRC: " << Checksum << "\n"
      << "properties count: " << Properties.size() << "\n";

		inStream << "properties: \n";
		for (auto property : Properties) {
			inStream << "\t" << property.first << " : " << property.second << "\n";
		}
	}

  int Event::_CRC32(const std::string& my_string) {
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.size());
    return result.checksum();
  }


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

  bool Event::fromStream(boost::asio::streambuf& in) {

    int bytes_received = in.size();
    if (bytes_received < Event::HeaderLength + Event::FooterLength) {
      std::cerr
        << "message is too short (" << bytes_received
        << " out of " << Event::HeaderLength + Event::FooterLength << "!!\n";
      return false;
    }

    std::cout << "received " << bytes_received << "bytes of data\n";

    char sp1;
    unsigned char uid, feedback, flags;
    uint16_t length;
    //char clength[2] = {0};
    //char header[3] = {0};

    std::istream is(&in);
    is.unsetf(std::ios_base::skipws);
    is >> uid >> flags >> feedback >> length;
    //is.rdbuf()->sgetn(header,3);
    //is.rdbuf()->sgetn(clength, 2);
    //clength[2] = '\0';
    is.get();

    this->UID = (EventUID)uid;
    this->Options = flags;
    this->Feedback = (EventFeedback)feedback;
    this->Length = length;

    // check header's sanity
    if ((this->UID < EventUID::Unassigned || this->UID > EventUID::SanityCheck)
        || (this->Length > Event::MaxLength)
        || (this->Feedback < EventFeedback::Unassigned || this->Feedback > EventFeedback::SanityCheck)) {
      std::cerr << "request failed header sanity check\n";
      return false;
    }

    // there must be N+sizeof(int) bytes of properties where N > in_bytes - signature
    if (this->Length > 0 && (bytes_received - Event::HeaderLength - Event::FooterLength < this->Length+sizeof(int))) {
      std::cerr << "invalid properties length: " << this->Length << "\n";
      return false;
    }

    std::cout << "event uid : " << (int)this->UID << " and length: " << this->Length << "\n";

    // parse properties
    if (this->Length > 0) {
      if ((this->Options & Event::Compressed) == Event::Compressed) {
        is >> this->Rawsize;
        is.get();

        std::cout << "a compressed event, raw size: " << this->Rawsize << "\n";
      }

      int checksum;
      is >> checksum;
      is.get();

      std::string props;
      for (int i=0; i < this->Length; ++i)
        props.push_back(is.get());

      // verify CRC checksum
      this->Checksum = Event::_CRC32(props);
      if (this->Checksum != checksum) {
        std::cerr << "CRC mismatch, aborting: " << this->Checksum << " vs " << checksum << " for " << props << " => " << props.size() << "\n";
        //delete props;
        return false;
      }

      if ((this->Options & Event::NoFormat) == Event::NoFormat) {
        this->setProperty("Data", std::string(props));

        //std::cout << "oh loook its an unformatted msg: " << std::string(props) << "\n";
      } else {
        std::vector<std::string> elems = split(props, ',');
        assert(elems.size() > 0 && elems.size() % 2 == 0);

        for (int i=0; i < elems.size(); i+=2)
          this->setProperty(elems[i], elems[i+1]);
      }

      //delete props;
    }

    // skip the footer
    std::cout << in.size() << " bytes left\n";
    assert(in.size() == Event::FooterLength);
    in.consume(Event::FooterLength);

    //this->dump();

    return true;
  }

  void Event::toStream(boost::asio::streambuf& out) const {

    //std::cout << "pre-message dump: buffer has " << out.size() << "(expected 0), ";
    std::ostream stream(&out);

    std::string props = "";
    if (!this->Properties.empty()) {
      // if the event is raw, we've to dump only one property
      if ((this->Options & Event::NoFormat) == Event::NoFormat) {
        // must have this, otherwise discard
        assert(this->hasProperty("Data"));
        if (this->hasProperty("Data"))
          props = this->getProperty("Data");
      } else {
        // flatten properties
        for (auto property : this->Properties)
          props += property.first + "," + property.second + ",";

        props.erase(props.end()-1);
      }
    }

    stream << (unsigned char)this->UID;
    stream << (unsigned char)this->Options;
    stream << (unsigned char)this->Feedback;
    //stream.rdbuf()->sputn(stringify((uint16_t)props.size()).c_str(), 2);
    stream << (uint16_t)props.size() << " ";
    if (!props.empty()) {
      if ((this->Options & Event::Compressed) == Event::Compressed) {
        stream << (uint32_t)this->Rawsize << " ";
        std::cout << "dumping raw size of compressed event: " << this->Rawsize << "\n";
      }

      stream << Event::_CRC32(props) << " ";
      stream << props;
    }

    //std::cout << "outbound -> " << props << "\n";

    //~ std::cout << "Event properties: "
      //~ << props << " (size: " << (uint16_t)props.size()
      //~ << "->" << stringify((uint16_t)props.size()) << ")\n";

    stream << Event::Footer;
    //std::cout << "post-dump: " << out.size() << "bytes \n";
  }

}
}