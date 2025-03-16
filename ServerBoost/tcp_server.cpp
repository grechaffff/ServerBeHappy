#include"tcp_server.h"


tcp_server::tcp_server(boost::asio::io_context& io_context)
	: io_context_(io_context) , acceptor_(io_context , tcp::endpoint(tcp::v4() , 7171)) { 

	start_accept();
}

void tcp_server::start_accept()
{

    tcp_connection::pointer new_connection =
        tcp_connection::create(io_context_);
    counter++;
    std::cout << "CounterUserConn: " << counter << endl;
    std::cout << "" << std::endl;
    acceptor_.async_accept(new_connection->socket().lowest_layer(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));


}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
    const boost::system::error_code& error)

{
    if (!error)
    {
        new_connection->start();
    }

    start_accept();

}



