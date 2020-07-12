#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>


using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

int main() {
    io_service service;

    try {
        tcp::endpoint ep(address::from_string("173.230.158.61"), 8888); // localhost port 8888
        tcp::socket socket(service);
        
        socket.open(tcp::v4());
        socket.connect(ep);
        std::cout << "Successfully connected to " << ep.address() << " port " << ep.port() << std::endl;


        std::string line;
        while(1) {
            std::getline(std::cin, line);
            line += "\n";
            
            socket.write_some(buffer(line)); 

        }        
    }
    catch (std::exception& e)
    {
        std::cout << "[Exception] " << e.what() << std::endl;
    }
    return 0;
}
