#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include<memory>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include<boost/asio/ssl.hpp>
#include<sstream>
#include<argon2.h>
#include"Globals.h"


using namespace boost::asio;
using tcp = ip::tcp;
using namespace std;

class AuthService;


class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{

public:

    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context);

    tcp::socket& socket();

    void start();


    void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

    void send_message(string& data);

private:
    tcp_connection(boost::asio::io_context& io_context);

    void do_read();

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);


  
   
    std::shared_ptr<AuthService> auth_service_;
   

    ssl::stream<tcp::socket> socket_;
    std::array<char, 1024> buffer_{};  // Используем std::array для буфера
   

    uint32_t t_cost = 4;
    uint32_t m_cost = 65536;
    uint32_t parallelism = 2;

    uint8_t hash[32];
    uint32_t hashlen = 32;
    uint8_t salt[16];

    string responseChoice;
    string username;
    string password;
    string email;


};
