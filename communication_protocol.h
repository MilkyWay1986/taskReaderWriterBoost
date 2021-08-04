#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

namespace CP {

    class CommunicationProtocol {
    public:
        //-----------------------------------

        enum {
            header_length = 4
        };
        enum {
            max_body_length = 512
        };

        //-----------------------------------

        CommunicationProtocol();

        ~CommunicationProtocol() = default;

        //-----------------------------------

        const char *Data() const;

        char *Data();

        //-----------------------------------

        std::size_t Length() const;

        //-----------------------------------

        const char *Body() const;

        char *Body();

        //-----------------------------------

        std::size_t BodyLength() const;

        void BodyLength(std::size_t new_length);

        //-----------------------------------

        bool DecodeHeader();

        void EncodeHeader();

        //-----------------------------------

        std::string UintToStringConvert(std::size_t str);

        char *StringToCharConvert(std::string &&str);

        std::string CharToStringConvert(char *str);

        //-----------------------------------

    private:
        char data_[header_length + max_body_length];
        std::size_t body_length_;
    };

}// namespace CP
