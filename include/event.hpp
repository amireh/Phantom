#ifndef H_Event_H
#define H_Event_H

#include <vector>
#include <exception>
#include <map>
#include <string>
#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>


using std::ostream;
using std::map;
namespace Pixy {
namespace Net {

  enum class EventUID : unsigned char {
    Unassigned = 0,
    EntityEvent,
    Login,
    Register,
    FetchGameData,
    FetchPuppetData,
    FetchPuppetsData,
    DrawSpells,
    Ping,
    Pong,

    SanityCheck
  };

  enum class EventType : unsigned char {
    Unassigned = 0,
    Request,
    Response
  };

  enum class EventFeedback : unsigned char {
    Unassigned = 0,
    Ok,
    Error,
    InvalidRequest
  };

    /*! \class Event "Event.h" "include/Event.h"
     *  \brief
     *  Base Event object that is used and handled to represent game events.
     */
  struct Event {

    enum {
      HeaderLength = 7, // "UID Length Feedback "
      FooterLength = 4, // "\r\n\r\n"
      MaxLength = 65536, // no single message can be longer than this (stored in uint16_t)
      //gmax_length = 512
    };

		//! default ctor
		//Event(const int inId, const std::string inName);
    Event();
		Event(const EventUID);
    Event(const Event& src);
    Event& operator=(const Event& rhs);

		//! destructor; resets evt state
		~Event();

    //void setUID(const EventUID inUID);
    //EventUID getUID() const;

		/*! \brief
		 *  Sets the name of this Event, used as a convenience for debugging
		 */
		//void setName(const std::string inName);

		/*! \brief
		 *  Retrieves the name of this Event.
		 */
		//std::string const& getName() const;

		//void setType(EventType inType);
		//EventType getType() const;

		//void setFeedback(EventFeedback inMsg);
		//EventFeedback getFeedback() const;

		//void addHandler();
		//void removeHandler();
		//int getNrHandlers();

		std::string const& getProperty(std::string inName) const;
		void setProperty(const std::string inName, const std::string inValue);
		bool propertyExists(const std::string& inName) const;
    bool hasProperty(const std::string& inName) const;

		//friend ostream& operator<<(ostream& outStream, Event* inEvt);

		void dump(std::ostream& inStream = std::cout) const;

		/*bool isLocal();
		void setLocal(bool inLocal);

    virtual void setAny(void* any);
    virtual void* getAny();*/

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & UID;
      //ar & mName;
      //ar & mType;
      ar & Feedback;
      //ar & nrProperties;
      ar & Properties;
    }

    virtual void reset();

	//protected:
		typedef	map< std::string, std::string > property_t;

		EventUID    		UID;
    uint16_t        Length;
		//std::string		  mName;
		//EventType		    Type;
		EventFeedback	  Feedback;
		//bool            IsLocal;
		//int			        NrHandlers;
		//int			        nrProperties;
		property_t		  Properties;
		//void*           mAny;

	//private:
		//Event(const Event& src);
		//Event& operator=(const Event& rhs);
		void _clone(const Event& src);

	};


}
} // end of namespace
#endif // H_Event_H
