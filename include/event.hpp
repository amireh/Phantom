#ifndef H_Event_H
#define H_Event_H

#include <vector>
#include <exception>
#include <map>
#include <string>
#include <iostream>
#include <cstring>
#include <boost/crc.hpp>

namespace Pixy {
namespace Net {

  /*enum class EventType : unsigned char {
    Unassigned = 0,
    Request,
    Response
  };*/

  enum class EventUID : unsigned char {
    Unassigned = 0,

    // internal events
    Ping,
    Pong,

    // entry events
    Login,
    Logout,
    ValidateClient,
    SyncGameData,
    CreatePuppet,
    UpdatePuppet,

    // lobby events
    ListRooms,
    JoinRoom,
    LeaveRoom,
    SendMessage,
    SendWhisper,
    AddFriend,
    RemoveFriend,
    Ban,
    Unban,
    ReportAccount,
    OpenTicket,
    CloseTicket,
    JoinQueue,
    LeaveQueue,
    SyncProfileData,
    SyncLadderData,

    // instance events
    SyncPuppetData,
    Ready,
    MatchStarted,
    StartTurn,
    DrawSpells,
    TurnStarted,
    EndTurn,
    CastSpell,
    CreateEntity,
    UpdateEntity,
    Attack,
    Defend,
    MatchFinished,
    SyncScore,

    SanityCheck
  };

  enum class EventFeedback : unsigned char {
    Unassigned = 0,
    Ok,
    Error,
    InvalidRequest,
    InvalidCredentials
  };

  /*! \class Event "Event.h" "include/Event.h"
   *  \brief
   *  Base Event object that is used and handled to represent game events.
   */
  struct Event {

    enum {
      // if Length is small enough it will be cast to 1 byte thus we can't
      // depend on uint16_t being interpreted as 2 bytes long.. so we -1
      HeaderLength = sizeof(unsigned char) * 3 + sizeof(uint16_t) - 1, // "UIDLengthFeedback"
      FooterLength = strlen("\r\n\r\n"), // "\r\n\r\n"
      MaxLength = 65536 // no single message can be longer than this (stored in uint16_t)
    };

    // event options
    enum {
      NoFormat    = 0x01, // events with no format will not be parsed per-property
      Compressed  = 0x02, // whether the content is compressed
      IsLocal     = 0x04, // local events will not be dispatched over the network
      Broadcast   = 0x08
    };

		typedef	std::map< std::string, std::string > property_t;

    Event();
		Event(const EventUID);
    Event(const Event& src);
    Event& operator=(const Event& rhs);

		//! resets evt state
		~Event();

    //! resets event state
    void reset();

		std::string const& getProperty(std::string inName) const;
		void setProperty(const std::string inName, const std::string inValue);
    bool hasProperty(const std::string inName) const;



    /// debug
		void dump(std::ostream& inStream = std::cout) const;

		EventUID    		    UID;
    unsigned char       Options;
    EventFeedback	      Feedback;
    uint16_t            Length;
		int                 Checksum;
    property_t		      Properties;
    static const char   *Footer;

    static int _CRC32(const std::string& my_string);
		void _clone(const Event& src);
	};

}
} // end of namespace
#endif // H_Event_H
