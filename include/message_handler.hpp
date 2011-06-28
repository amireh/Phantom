#ifndef H_PixyNet_MessageHandler_H
#define H_PixyNet_MessageHandler_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <exception>
#include <stdexcept>
#include "message_parser.hpp"
#include "handler.hpp"
#include <map>
#include <deque>

using boost::asio::ip::tcp;
class message_handler {
  public:
    typedef boost::function<void(const message&)> msg_handler_t;

    message_handler(boost::asio::io_service&, tcp::socket&);
    ~message_handler();

    void send(const message &msg, boost::asio::streambuf& out);
    void recv(boost::asio::streambuf& in);

    template <typename T>
    void bind(message_id msg, T* inT, void (T::*handler)(const message &)) {
      // register message if it isn't already
      handlers_t::iterator binder = handlers_.find(msg);
      if (binder == handlers_.end())
      {
        std::vector<msg_handler_t> handlers;
        binder = handlers_.insert(make_pair(msg, handlers)).first;
      }

      binder->second.push_back( boost::bind(handler, inT, _1) );
    }

    void notify(const message&);

  private:

    void dispatch();

    //message_parser& parser_;
    tcp::socket* socket_;
    boost::asio::strand strand_;

    typedef std::map< message_id, std::vector<msg_handler_t> > handlers_t;
    handlers_t handlers_;

    std::deque<message> messages;
};

#endif
