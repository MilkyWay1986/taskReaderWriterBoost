#include "client.h"

#include <iostream>

int main(int argc, char *argv[]) {

    try {

        if (std::string(argv[1]) == "reader") {
            boost::asio::io_context io_context_cl;
            std::shared_ptr<CS::Client> pClient =
                    std::make_shared<CS::Client>(std::string(argv[2]), std::string(argv[3]),
                                                 io_context_cl,
                                                 std::chrono::duration_cast<std::chrono::milliseconds>(
                                                         std::chrono::system_clock::now().time_since_epoch()).count(),
                                                 CS::Client::type_client::reader);
            io_context_cl.run();
        } else if (std::string(argv[1]) == "writer") {
            for (;;) {
                boost::asio::io_context io_context_cl;
                std::shared_ptr<CS::Client> pClient =
                        std::make_shared<CS::Client>(std::string(argv[2]), std::string(argv[3]),
                                                     io_context_cl,
                                                     std::chrono::duration_cast<std::chrono::milliseconds>(
                                                             std::chrono::system_clock::now().time_since_epoch()).count(),
                                                     CS::Client::type_client::writer);
                io_context_cl.run();
                io_context_cl.stop();

                std::cout << "[+] Destroy\n\n";
                std::cout << "Timeout 10 sec...\n\n";
                std::cout << "------------------------------- \n\n";
                sleep(10);

            }

        } else return -1;

    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

