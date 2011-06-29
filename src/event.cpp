#include "event.hpp"

namespace Pixy {
namespace Net {

  const char* Event::Footer = "\r\n\r\n";
	Event::Event() {
    reset();
  }

  Event::Event(const EventUID inUID) {
		reset();
		UID = inUID;
	}

	Event::~Event()
  {
		//std::cout << "Event " << mName << " destroyed\n";
		reset();
    //mAny = 0;
  }

	void Event::reset() {
		//mName.clear();
    UID = EventUID::Unassigned;
		//Type = EventType::Unassigned;
		Feedback = EventFeedback::Unassigned;
    Properties.clear();
    Length = 0;
		//nrHandlers = 0;
		//nrProperties = 0;
		//fLocal = false;
		//mAny = 0;
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
		//this->Name = src.mName;
		//this->Type = src.mType;
		this->Feedback = src.Feedback;
    this->Length = src.Length;
		//this->NrProperties = src.nrProperties;
    //this->mAny = src.mAny;

    if (!src.Properties.empty())
      for (auto property : src.Properties)
        setProperty(property.first, property.second);

	}

	/*ostream& operator<<(ostream& outStream, Event* inEvt) {
		std::string _type = (inEvt->getType() == EventType::Request) ? "REQUEST" : "RESPONS";

		outStream << _type << "> " << inEvt->getName();

		_type.clear();
		return outStream;
	}


	void Event::setUID(const EventUID inUID) {
		mUID = inUID;
	}

	EventUID Event::getUID() const {
		return mUID;
  }

  void Event::setName(const std::string inName) {
      mName = inName;
  }

  std::string const& Event::getName() const {
    return mName;
  }*/

	/*
    bool Event::operator==(const Event& rhs) {
        if (mId == rhs.mId)
            return true;
        else
            return false;
    }
	 */

	/*void Event::setFeedback(EventFeedback inMsg) {
		mFeedback = inMsg;
	}

	EventFeedback Event::getFeedback() const {
		return mFeedback;
	}

	void Event::setType(EventType inType) {
		mType = inType;
	}

	EventType Event::getType() const {
		return mType;
	}

	void Event::addHandler() {
		nrHandlers++;
	}

	void Event::removeHandler() {
		nrHandlers--;
		//if (nrHandlers <= 0)
			//EventManager::getSingletonPtr()->releaseEvt(this);
	}

	int Event::getNrHandlers() { return nrHandlers; };*/

	void Event::setProperty(const std::string inName, const std::string inValue) {
		if ( propertyExists(inName) ) {
			Properties.find(inName)->second = inValue;
			return;
		}

		Properties.insert( std::make_pair(inName, inValue) );
		//nrProperties++;
	}

	std::string const& Event::getProperty(const std::string inName) const {
		return Properties.find(inName)->second;
	}

	bool Event::propertyExists(const std::string& inName) const {
		return (Properties.find(inName) != Properties.end());
	}
	bool Event::hasProperty(const std::string& inName) const {
		return propertyExists(inName);
	}
	void Event::dump(std::ostream& inStream) const {
		inStream
      << "uid: " << (int)UID << "\n"
      << "length: " << Length << "\n"
      //<< "type: " << (mType == EventType::Request ? "request" : "response") << "\n"
      << "feedback: " << (Feedback == EventFeedback::Error ? "error" : "ok") << "\n"
      //<< "current handlers count: " << nrHandlers << "\n"
      << "properties count: " << Properties.size() << "\n";

		inStream << "properties: \n";
		for (auto property : Properties) {
			inStream << "\t" << property.first << " : " << property.second << "\n";
		}
	}

	/*bool Event::isLocal() { return fLocal; };
	void Event::setLocal(bool inLocal) { fLocal = inLocal; };

	void Event::setAny(void* inAny) {
	  mAny = inAny;
	}

	void* Event::getAny() { return mAny; }*/

}
}
