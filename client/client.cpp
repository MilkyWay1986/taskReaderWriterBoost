#include "client.h"


//-------------------------------------------------------------------------------------

CS::Client::Client(std::string host, std::string port,
                   boost::asio::io_context &io_context,
                   size_t id, type_client tc)
        : CP(std::make_shared<CP::CommunicationProtocol>()),
          id(id), tc(tc),
          id_w(CP->StringToCharConvert(CP->UintToStringConvert(id))),
          ip(std::move(host)), port(std::move(port)),
          io_context(io_context) {

    DoConnect();
}

//-------------------------------------------------------------------------------------

void CS::Client::DoConnect() {
    boost::asio::async_connect(socket, endpoints,
                               [this](boost::system::error_code ec, const boost::asio::ip::tcp::endpoint &) {
                                   if (!ec) {
                                       DoWriteWhoIAm();
                                   } else {
                                       std::cout << "[-]Failed to connect!" << "\n";
                                       std::cout << ec << "\n";
                                       socket.close();

                                       std::cout << "[+]Timeout 20 sec...\n";
                                       std::cout << "------------------------------- \n\n";
                                       sleep(20);

                                       DoConnect();
                                   }

                               });
}

//-------------------------------------------------------------------------------------

void CS::Client::DoWriteWhoIAm() {

    if (tc == type_client::reader) {
        char s[CP::CommunicationProtocol::max_body_length + 1] = "reader";
        CP->BodyLength(std::strlen(s));
        std::memcpy(CP->Body(), s, CP->BodyLength());
    } else if (tc == type_client::writer) {
        char s[CP::CommunicationProtocol::max_body_length + 1] = "writer";
        CP->BodyLength(std::strlen(s));
        std::memcpy(CP->Body(), s, CP->BodyLength());
    }

    CP->EncodeHeader();

    boost::asio::async_write(socket, boost::asio::buffer(CP->Data(), CP->Length()),
                             [this](boost::system::error_code ec, std::size_t) {
                                 if (!ec) {
                                     if (tc == type_client::reader) {
                                         std::cout << "[+] I Am reader\n";
                                         IAmReaderDoReadHeader();
                                     } else if (tc == type_client::writer) {
                                         std::cout << "[+] I Am writer\n";
                                         IAmWriterDoWrite();
                                     } else { socket.close(); }
                                 } else {
                                     std::cout << "------------------------------- \n";
                                     std::cout << "[-] Failed to write!  " << ec.message() << "\n";
                                     std::cout << "------------------------------- \n\n";
                                     socket.close();
                                     DoConnect();
                                 }
                             });
}

//-------------------------------------------------------------------------------------

void CS::Client::Close() { boost::asio::post(io_context, [this]() { socket.close(); }); }

//-------------------------------------------------------------------------------------
void CS::Client::IAmWriterDoWrite() {

    char s[CP::CommunicationProtocol::max_body_length + 1];
    std::strcpy(s, id_w);
    CP->BodyLength(std::strlen(s));
    std::memcpy(CP->Body(), s, CP->BodyLength());
    CP->EncodeHeader();

    boost::asio::async_write(socket, boost::asio::buffer(CP->Data(), CP->Length()),
                             [this](boost::system::error_code ec, std::size_t) {
                                 if (!ec) {
                                     std::cout << "[+] Data to write is ";
                                     std::cout.write(CP->Data(), CP->Length());
                                     std::cout << "\n";
                                     std::cout << "[+] Do write...\n";
                                     socket.close();
                                     std::cout << "[+] Disconnect...\n\n";
                                 } else {
                                     std::cout << "------------------------------- \n";
                                     std::cout << "[-] Failed to write!  " << ec.message() << "\n";
                                     std::cout << "------------------------------- \n\n";
                                     socket.close();
                                     DoConnect();
                                 }
                             });

}

//-------------------------------------------------------------------------------------

void CS::Client::IAmReaderDoReadHeader() {

    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Data(), CP->header_length),
                            [this](boost::system::error_code ec, std::size_t) {
                                if (!ec && CP->DecodeHeader()) {
                                    IAmReaderDoReadBody();
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read header!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                    socket.close();
                                    DoConnect();
                                }
                            });

}

//-------------------------------------------------------------------------------------

void CS::Client::IAmReaderDoReadBody() {
    std::cout << "[+] Received is ";
    boost::asio::async_read(socket,
                            boost::asio::buffer(CP->Body(), CP->BodyLength()),
                            [this](boost::system::error_code ec, std::size_t) {
                                if (!ec) {
                                    std::cout.write(CP->Body(), CP->BodyLength());
                                    std::cout << "\n";
                                    IAmReaderDoReadHeader();
                                } else {
                                    std::cout << "------------------------------- \n";
                                    std::cout << "[-] Failed to read body!  " << ec.message() << "\n";
                                    std::cout << "------------------------------- \n\n";
                                    socket.close();
                                    DoConnect();
                                }
                            });
}

//-------------------------------------------------------------------------------------
