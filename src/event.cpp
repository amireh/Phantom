#include "event.hpp"

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

}
}
