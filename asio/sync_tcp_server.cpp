#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>


using namespace std;
using namespace boost;
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr; // smart pointer(no need to mannually deallocate, kinda like java references)
boost::mutex mu;

void client_session(socket_ptr socket) {
    char buf[256];
    try {
        while(1) {
            socket->read_some(boost::asio::buffer(buf));
            std::cout << std::string(buf);
            memset(buf, 0, strlen(buf));
            
            // socket->write_some(boost::asio::buffer("received", 8)); // oop style
            /* boost::asio:write/read vs socket.write_some/read_some : 
            * write can write all in one call, write_some doesn't guarenteed 
            * all bytes got written, which might need a loop to do so
            */
            boost::asio::write(*socket, boost::asio::buffer("received!\n", 10)); // c style
        }
    }
    catch (std::exception& e)
    {
        std::cout << "[Exception] " << e.what() << std::endl;
    }

}

int main() {
    io_service service;

    try {
        ip::tcp::endpoint endpoint_to_listen(ip::tcp::v4(), 8888); // listen on port 8888
        ip::tcp::acceptor acceptor(service, endpoint_to_listen); 
            /* boost acceptor constructor automatically take cares of all three 
            * steps: "bind() ---> listen() ---> accept()" of a unix socket  
            */

        std::cout << "Server started, open another terminal and run 'nc localhost 8888' to test this server" << std::endl; 

        while(1) {
            socket_ptr socket(new ip::tcp::socket(service)); // new socket object and let the smart pointer to point to it
            acceptor.accept(*socket); // waiting for new connection (blocking)
            std::cout << "Accepted socket request from: " << socket->remote_endpoint().address().to_string() 
                                << std::endl;
            boost::thread(boost::bind(client_session, socket)); // process client request in a new thread
        }
    }
    catch (std::exception& e)
    {
        std::cout << "[Exception] " << e.what() << std::endl;
    }
    return 0;
}

// test this program using the following cmd
// nc localhost 8888
// then type any message u want to send to the server