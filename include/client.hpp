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

#ifndef H_PixyNet_Client_H
#define H_PixyNet_Client_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "client_connection.hpp"
#include "cresource_manager.hpp"

using boost::asio::ip::tcp;
namespace Pixy {
namespace Net {

  class client
  {
    public:
      typedef std::list<Puppet*> puppets_t;

      client(boost::asio::io_service& io_service);
      virtual ~client();

      void run();

    private:
      connection_ptr conn_;

      boost::asio::io_service &io_service_;
      boost::asio::deadline_timer timer_;

      void on_login(const Event& evt);
      void on_sync_game_data(const Event& evt);
      void on_join_queue(const Event& evt);
      void on_sync_puppet_data(const Event& evt);
      void on_start_turn(const Event&);
      void on_turn_started(const Event&);
      void on_draw_spells(const Event&);

      puppets_t const& get_puppets();
      void register_puppet(Puppet* inPuppet);
      void assign_puppet(Puppet* inPuppet);
      Puppet* get_puppet();
      Puppet* get_puppet(int inUID);

      CResourceManager rmgr_;
      puppets_t		puppets_;
      std::string puppet_name_;
      Puppet *puppet_, *active_puppet_;
  };

}
}
#endif
