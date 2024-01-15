#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#ifndef IPADDRESS_NO_EXCEPTIONS

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

class error : public std::runtime_error {
public:
    explicit error(const std::string& message) : std::runtime_error(message) {
    }

    explicit error(const char* message) : std::runtime_error(message) {
    }
};

class parse_error : public error {
public:
    explicit parse_error(const std::string& message) : error(message) {
    }

    explicit parse_error(const char* message) : error(message) {
    }
};

}

#endif

#endif
