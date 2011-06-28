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

using boost::asio::ip::tcp;
class message_handler {
  public:
    typedef boost::function<void(boost::shared_ptr<message>)> msg_handler_t;

    message_handler(tcp::socket&, boost::asio::strand&, message_parser&);
    ~message_handler();

    void send(const message &msg, boost::asio::streambuf& out);
    void recv(boost::asio::streambuf& in);

    template <typename T>
    void bind(message_id msg, T* inT, void (T::*handler)(boost::shared_ptr<message>)) {
      // register message if it isn't already
      handlers_t::iterator binder = handlers_.find(msg);
      if (binder == handlers_.end())
      {
        std::vector<msg_handler_t> handlers;
        binder = handlers_.insert(make_pair(msg, handlers)).first;
      }

      binder->second.push_back( boost::bind(handler, inT, _1) );
    }

  private:
    void notify(boost::shared_ptr<message> msg);

    message_parser& parser_;
    tcp::socket* socket_;
    boost::asio::strand& strand_;

    typedef std::map< message_id, std::vector<msg_handler_t> > handlers_t;
    handlers_t handlers_;
};

#endif
