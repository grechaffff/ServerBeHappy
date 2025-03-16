#pragma once
#include"tcp_connection.h"

class tcp_server {


public:


	tcp_server(boost::asio::io_context& io_context);

private:

	void start_accept();


	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error);

	int counter = 0;

	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;

};