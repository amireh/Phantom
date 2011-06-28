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
    std::vector<client*> clients;
    for (int i =0; i < nr_clients; ++i)
      clients.push_back(new client(io_service));

    boost::thread_group threads;
    for (int i=0; i < 3; ++i)
      threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));

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
