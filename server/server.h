#pragma once

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <string>
#include <cstring>
#include <mutex>
#include <iostream>
#include <memory>


#include "../communication_protocol.h"
#include "../shared_memory.h"


namespace CS {

    static std::shared_ptr<SM::SharedMemory> SM = std::make_shared<SM::SharedMemory>();
    static std::mutex sm_m;

//***********************************************************************************************************//
    struct ConnectionSession : public std::enable_shared_from_this<ConnectionSession> {

    public:
        //-----------------------------------

        explicit ConnectionSession(boost::asio::ip::tcp::socket socket);

        ~ConnectionSession() = default;

        //-----------------------------------

        void DoReadHeader();

        void DoReadBody();

        //-----------------------------------

        void DoReadHeaderMessageWriter();

        void DoReadBodyMessageWriter();

        //-----------------------------------

        void DoWriteForReader(boost::asio::ip::tcp::socket &&socket);

        //-----------------------------------

    private:
        boost::asio::ip::tcp::socket socket;
        std::shared_ptr<CP::CommunicationProtocol> CP;
        std::shared_ptr<SM::SharedMemory>& SM_server;

    };

//***********************************************************************************************************//
    class Server {

    public:
        Server(boost::asio::io_context &io_context, const boost::asio::ip::tcp::endpoint &endpoint);

        ~Server() = default;

    private:
        void DoAccept();

    private:
        boost::asio::ip::tcp::acceptor acceptor;

    };
//***********************************************************************************************************//

}//namespace CS

