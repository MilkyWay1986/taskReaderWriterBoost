#pragma once


#include "../communication_protocol.h"


#include <boost/asio.hpp>
#include <boost/move/utility.hpp>


#include <memory>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <unistd.h>


namespace CS {

    class Client {

    public:
        //-----------------------------------
        enum type_client {
            reader, writer
        };
        //-----------------------------------

    private:
        std::shared_ptr<CP::CommunicationProtocol> CP;
        const std::size_t id;
        const type_client tc;
        const char *id_w;

        std::string ip;
        std::string port;


        boost::asio::io_context &io_context;

        boost::asio::ip::tcp::resolver resolver{io_context};

        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(
                boost::asio::ip::tcp::resolver::query{ip, port});

        boost::asio::ip::tcp::socket socket{io_context};

    public:
        //-----------------------------------

        Client(std::string, std::string, boost::asio::io_context &, std::size_t, type_client);

        ~Client() = default;

        //-----------------------------------

        void DoConnect();

        //-----------------------------------

        void DoWriteWhoIAm();

        //-----------------------------------

        void Close();

        //-----------------------------------

        void IAmWriterDoWrite();

        //-----------------------------------

        void IAmReaderDoReadHeader();

        void IAmReaderDoReadBody();

        //-----------------------------------

    };
}//namespace CS

