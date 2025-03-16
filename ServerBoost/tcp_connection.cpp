#include"tcp_connection.h"
#include"DataBase.h"
#include"AuthService.h"


typedef boost::shared_ptr<tcp_connection> pointer;

pointer tcp_connection::create(boost::asio::io_context& io_context)
{
	return pointer(new tcp_connection(io_context));
}

tcp::socket& tcp_connection::socket()
{


	return socket_.next_layer();
}

void tcp_connection::start() {

	auth_service_ = std::make_shared<AuthService>(shared_from_this());

	


	do_read();

}




tcp_connection::tcp_connection(boost::asio::io_context& io_context)
	: socket_(io_context, ctx) {
}



void tcp_connection::do_read()
{

	socket_.handshake(ssl::stream_base::server);

	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&tcp_connection::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

}

void tcp_connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{

	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "Client disconnected (EOF received)\n";
		}
		else
		{
			std::cerr << "Read error: " << error.message() << std::endl;
			std::cout << "Client connection closed: " << endl;
			std::cout << "---------------------------------------------- " << endl;
		}
		return;
	}

	std::string data(buffer_.data(), bytes_transferred);

	auth_service_->recvData(data, socket_);

}

void tcp_connection::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::cout << "Response sent to client.\n";

		std::cout << "-----------------------------------------------" << std::endl;
	}
	else
	{
		std::cerr << "Write error: " << error.message() << std::endl;
	}

}

void tcp_connection::send_message(string& dataResponse)
{

	
	boost::asio::async_write(socket_, boost::asio::buffer(dataResponse),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	
}


