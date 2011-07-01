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

#ifndef H_PixyNet_Event_H
#define H_PixyNet_Event_H

#include <vector>
#include <exception>
#include <map>
#include <string>
#include <iostream>
#include <cstring>
#include <boost/crc.hpp>
#include <boost/asio.hpp>

namespace Pixy {
namespace Net {

  class request_incomplete : public std::runtime_error {
    public:

    request_incomplete(std::string msg, int in_bytes_left)
    : std::runtime_error(msg),
      bytes_left(in_bytes_left) {
    }
    virtual ~request_incomplete() throw() {
    }

    int bytes_left;
  };

  class bad_request : public std::runtime_error {
    public:

    bad_request(std::string msg)
    : std::runtime_error(msg) {
    }
    virtual ~bad_request() throw() {
    }

  };

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
    Login, // 3
    Logout,
    ValidateClient,
    SyncGameData,
    CreatePuppet,
    UpdatePuppet,

    // lobby events
    ListRooms, // 9
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
    MatchFound,
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
    InvalidCredentials,

    SanityCheck
  };

  class Player;
  typedef boost::shared_ptr<const Player> player_cptr;
  /*! \class Event "Event.h" "include/Event.h"
   *  \brief
   *  Base Event object that is used and handled to represent game events.
   */
  struct Event {

    enum {
      // if Length is small enough it will be cast to 1 byte thus we can't
      // depend on uint16_t being interpreted as 2 bytes long.. so we -1
      HeaderLength = 5, // "UIDLengthFeedback"
      FooterLength = 4, // "\r\n\r\n"
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
    Event(const EventUID, EventFeedback = EventFeedback::Unassigned, unsigned char options=0);
    Event(const Event& src);
    Event& operator=(const Event& rhs);

		//! resets evt state
		~Event();

    bool fromStream(boost::asio::streambuf& in);
    void toStream(boost::asio::streambuf& out) const;

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
    uint32_t            Rawsize;
    static const char   *Footer;
    player_cptr         Sender;

    static int _CRC32(const std::string& my_string);
    static std::string _uid_to_string(EventUID);
		void _clone(const Event& src);
	};

}
} // end of namespace
#endif // H_Event_H
