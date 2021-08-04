#include "server.h"

//-------------------------------------------------------------------------------------

CS::Server::Server(boost::asio::io_context &io_context, const boost::asio::ip::tcp::endpoint &endpoint)
        : acceptor(io_context, endpoint) {
    DoAccept();
}

//-------------------------------------------------------------------------------------

void CS::Server::DoAccept() {
    acceptor.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "[+] New Connect... \n";
                    std::make_shared<ConnectionSession>(std::move(socket))->DoReadHeader();
                } else {
                    std::cout << ec.message() << "\n";
                }
                DoAccept();
            });
}

//-------------------------------------------------------------------------------------

CS::ConnectionSession::ConnectionSession(boost::asio::ip::tcp::socket socket)
        : socket(std::move(socket)),
          CP(std::make_shared<CP::CommunicationProtocol>()),
          SM_server(SM) {}

//-------------------------------------------------------------------------------------

void CS::ConnectionSession::DoReadHeader() {
    auto self(shared_from_this());
    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Data(), CP->header_length),
                            [this, self](boost::system::error_code ec, std::size_t) {
                                if (!ec && CP->DecodeHeader()) {
                                    DoReadBody();
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read header!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                }
                            });
}

//-------------------------------------------------------------------------------------

void CS::ConnectionSession::DoReadBody() {
    auto self(shared_from_this());

    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Body(), CP->BodyLength()),
                            [this, self](boost::system::error_code ec, std::size_t) {
                                if (!ec) {
                                    std::string res{CP->Body(), CP->BodyLength()};
                                    std::cout << "[+] " << res << " is connected...\n";
                                    std::cout << "------------------------------- \n\n";
                                    if (res == "reader") {
                                        DoWriteForReader(std::move(socket));
                                    } else if (res == "writer") {
                                        DoReadHeaderMessageWriter();
                                    }
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read body!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                }
                            });
}

//--------------------------------------WRITER-----------------------------------------------------------------------//
//-------------------------------------------------------------------------------------

void CS::ConnectionSession::DoReadHeaderMessageWriter() {
    auto self(shared_from_this());
    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Data(), CP->header_length),
                            [this, self](boost::system::error_code ec, std::size_t) {
                                if (!ec && CP->DecodeHeader()) {
                                    DoReadBodyMessageWriter();
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read header!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                }
                            });
}

//-------------------------------------------------------------------------------------

void CS::ConnectionSession::DoReadBodyMessageWriter() {
    auto self(shared_from_this());
    SM->PrintSharedMemory();
    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Body(), CP->BodyLength()),
                            [this, self](boost::system::error_code ec, std::size_t) {
                                if (!ec) {

                                    {
                                        std::unique_lock<std::mutex> lk(sm_m);
                                        std::cout << "[+] Set SM... \n";
                                        SM->SetSharedMemory(CP->Body(), CP->BodyLength());
                                        lk.unlock();
                                    }
                                    SM->PrintSharedMemory();

                                    socket.close();
                                    std::cout << "[+] Disconnection... \n\n";
                                    std::cout << "------------------------------- \n\n";
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read body!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                }
                            });
}

//-------------------------------------------------------------------------------------
//--------------------------------------WRITER-----------------------------------------------------------------------//



//--------------------------------------READER-----------------------------------------------------------------------//
//-------------------------------------------------------------------------------------

void CS::ConnectionSession::DoWriteForReader(boost::asio::ip::tcp::socket &&socket_) {
    auto self(shared_from_this());

    char s[CP::CommunicationProtocol::max_body_length + 1];
    std::strcpy(s, SM->GetSharedMemory().c_str());

    CP->BodyLength(std::strlen(s));
    std::memcpy(CP->Body(), s, CP->BodyLength());
    CP->EncodeHeader();


    boost::asio::async_write(socket,
                             boost::asio::buffer(CP->Data(), CP->Length()),
                             [self, &socket_](boost::system::error_code ec, std::size_t) {
                                 if (!ec) {
                                     self->DoWriteForReader(std::move(socket_));
                                 } else {
                                     std::cout << "------------------------------- \n";
                                     std::cout << "[-] Reader unavailable! "
                                                  "And it will be removed from the mailing queue!  "
                                               << ec.message() << "\n";
                                     std::cout << "------------------------------- \n\n";
                                 }
                             });
}

//-------------------------------------------------------------------------------------
//--------------------------------------READER-----------------------------------------------------------------------//
