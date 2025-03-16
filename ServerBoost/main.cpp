#define _CRT_SECURE_NO_WARNINGS

#include"tcp_server.h"



using boost::asio::ip::tcp;
using namespace std;

using namespace boost::asio;
using tcp = ip::tcp;



 /*
class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket()
    {
        return socket_.next_layer();
    }

    void start()
    {
        do_read();
    }

private:
    tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context , ctx)
    {
    }

    void do_read()
    {
        socket_.handshake(ssl::stream_base::server);

        socket_.async_read_some(boost::asio::buffer(buffer_),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
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
            }
            return;
        }

       

        // Преобразуем принятые данные в строку
        std::string data(buffer_.data(), bytes_transferred);
        //std::cout << "Received: " << data << std::endl;

        stringstream ss(data);
       

        getline(ss, responseChoice, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, email, '|');

        if (strcmp(responseChoice.c_str(), "Register") == 0) {

            Registrattion();
        }

        if (strcmp(responseChoice.c_str(), "Login") == 0) {

            Login();

        }


       
       
       
      
    }

    void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            std::cout << "Response sent to client.\n";

            std::cout << "--------------------------" << std::endl;
        }
        else
        {
            std::cerr << "Write error: " << error.message() << std::endl;
        }
    }

    void generate_salt(uint8_t* salt, size_t lenght) {

        if (!RAND_bytes(salt, lenght)) {


        }

    }

    void Registrattion() {


        std::cout << "Response Choice: " << responseChoice << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Email: " << email << std::endl;
      
        uint8_t hash[32];  
        uint32_t hashlen = 32;
        uint8_t salt[16];

        generate_salt(salt, sizeof(salt));


        


        int result = argon2id_hash_raw(t_cost , m_cost , parallelism, 
            password.c_str(), strlen(password.c_str()),
            salt, sizeof(salt),
            hash, hashlen);

        if (result == ARGON2_OK) {

            cout << "Argon2: ";
            for (uint8_t i : hash) {
                printf("%02x", i);
              
            }
            cout << endl;
        }

        else {
            cerr << "Error Argon2" << argon2_error_message(result) << endl;
        }

        std::string response = "Registration succesfull\n";
        boost::asio::async_write(socket_, boost::asio::buffer(response),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));


        
    }




    void Login() {
        std::cout << "Response Choice: " << responseChoice << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Password: " << password << std::endl;
       

        uint8_t hash[32];
        uint32_t hashlen = 32;
        uint8_t salt[16];

        generate_salt(salt, sizeof(salt));





        int result = argon2id_hash_raw(t_cost, m_cost, parallelism,
            password.c_str(), strlen(password.c_str()),
            salt, sizeof(salt),
            hash, hashlen);

        if (result == ARGON2_OK) {

            cout << "Argon2: ";
            for (uint8_t i : hash) {
                printf("%02x", i);

            }
            cout << endl;
        }

        else {
            cerr << "Error Argon2" << argon2_error_message(result) << endl;
        }


        std::string response = "Login succesfull\n";
        boost::asio::async_write(socket_, boost::asio::buffer(response),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

      
    }

   ssl::stream<tcp::socket> socket_;
    std::array<char, 1024> buffer_{};  // Используем std::array для буфера
    string responseChoice;
    string username;
    string password;
    string email;
   
    uint32_t t_cost = 4;
    uint32_t m_cost = 65536;
    uint32_t parallelism = 2;
};


class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context)
        : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 6969))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
            tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->socket().lowest_layer(),
            boost::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};
*/
int main()
{

    std::locale::global(std::locale("ru_RU.UTF-8"));

    try
    {
        std::cout << "Waiting" << std::endl;
        boost::asio::io_context io_context;

        ctx.set_options(ssl::context::default_workarounds |
            ssl::context::no_sslv2 |
            ssl::context::no_sslv3 |
            ssl::context::no_tlsv1 |
            ssl::context::no_tlsv1_1 |
            ssl::context::single_dh_use);
        boost::system::error_code error;

        ctx.use_certificate_chain_file("C:\\Users\\Rocket\\cert\\selfsigned.crt", error);
        if (!error) {
            ctx.use_private_key_file("C:\\Users\\Rocket\\cert\\private.key", ssl::context::pem);
            // ctx.set_verify_mode(ssl::verify_none);  // Только для тестов!
            tcp_server server(io_context);
            io_context.run();
          
        }
        else {

            cout << error.message() << endl;
        }
        
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
