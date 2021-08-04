#include "communication_protocol.h"


//-------------------------------------------------------------------------------------

CP::CommunicationProtocol::CommunicationProtocol()
        : body_length_(0) {}

//-------------------------------------------------------------------------------------

const char *CP::CommunicationProtocol::Data() const { return data_; }

//-------------------------------------------------------------------------------------

char *CP::CommunicationProtocol::Data() { return data_; }

//-------------------------------------------------------------------------------------

std::size_t CP::CommunicationProtocol::Length() const { return header_length + body_length_; }

//-------------------------------------------------------------------------------------

const char *CP::CommunicationProtocol::Body() const { return data_ + header_length; }

//-------------------------------------------------------------------------------------

char *CP::CommunicationProtocol::Body() { return data_ + header_length; }

//-------------------------------------------------------------------------------------

std::size_t CP::CommunicationProtocol::BodyLength() const { return body_length_; }

//-------------------------------------------------------------------------------------

void CP::CommunicationProtocol::BodyLength(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

//-------------------------------------------------------------------------------------

bool CP::CommunicationProtocol::DecodeHeader() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------

void CP::CommunicationProtocol::EncodeHeader() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}

//-------------------------------------------------------------------------------------

std::string CP::CommunicationProtocol::UintToStringConvert(std::size_t str) {
    std::stringstream ss;
    ss << str;
    return ss.str();
}

//-------------------------------------------------------------------------------------

char *CP::CommunicationProtocol::StringToCharConvert(std::string &&str) { return strdup(str.c_str()); }

//-------------------------------------------------------------------------------------

std::string CP::CommunicationProtocol::CharToStringConvert(char *str) { return std::string(str); }

//-------------------------------------------------------------------------------------