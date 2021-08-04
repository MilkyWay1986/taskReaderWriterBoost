#include "server.h"


int main(int argc, char *argv[]) {

    try {

        boost::thread_group m_thread_pool;

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),
                                                std::stoi(std::string(argv[1])));

        CS::Server server(io_context, endpoint);


        std::cout << "\n------------------------------- \n";
        std::cout << "[+] Create server\n";

        std::size_t count_threads = std::thread::hardware_concurrency() - 4;
        for (auto i = 0; i < count_threads; ++i)
            m_thread_pool.create_thread(std::move([&io_context]() { io_context.run(); }));

        std::cout << "[+] Create " << count_threads << " threads\n";
        std::cout << "------------------------------- \n\n";

        m_thread_pool.join_all();

    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

