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

#include "client.hpp"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cout << "Usage: client <nr_clients>\n";
      return 1;
    }

    boost::asio::io_service io_service;
    //boost::asio::io_service::work work(io_service);
    int nr_clients = atoi(argv[1]);

    boost::thread_group threads;
    for (int i=0; i < 3; ++i)
      threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));


    std::vector<Pixy::Net::client*> clients;
    for (int i =0; i < nr_clients; ++i)
      clients.push_back(new Pixy::Net::client(io_service));


    std::cout << "press [return] to quit\n";
    std::getchar();

    io_service.stop();
    threads.join_all();

    while (!clients.empty()) {
      delete clients.back();
      clients.pop_back();
    }
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}
