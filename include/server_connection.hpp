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
#ifndef H_PixyNet_Connection_H
#define H_PixyNet_Connection_H

#include "base_connection.hpp"
#include "db_manager.hpp"
#include "md5/md5.hh"

namespace Pixy {
namespace Net {

/// Represents a single connection from a client.
class connection : public base_connection
{
  public:

  /// Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service&);
  virtual ~connection();

  virtual boost::shared_ptr<connection> shared_from_this() {
    return boost::static_pointer_cast<connection>(base_connection::shared_from_this());
  }

  virtual void stop();
  void ping();

  bool is_authentic() const;
  bool in_instance() const;
  bool in_lobby() const;

  protected:

  virtual void handle_inbound();

  void reject(Event&);

  void on_pong(const Event& evt);
  void on_login(const Event& evt);
  void on_login_feedback(db_result rc, std::string username);
  void on_logout(const Event& msg);
  void on_sync_game_data(const Event& evt);
  void on_sync_puppets(const Event& evt);
  void on_create_puppet(const Event& evt);
  void on_remove_puppet(const Event& evt);
  void on_update_deck(const Event& evt);
  void on_remove_deck(const Event& evt);
  void on_join_queue(const Event& evt);
  void on_leave_queue(const Event& evt);
  void on_join_lobby(const Event& evt);
  void on_load_puppet(db_result rc);


  Event pingevt_;
  int ping_timeouts_;

  void promote(std::string& username);

  boost::shared_ptr<Player> player_;
};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace Net
} // namespace Pixy

#endif // H_PixyNet_Connection_H
