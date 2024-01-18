#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

enum class error_code {
    NO_ERROR = 0,
    EMPTY_ADDRESS,
    EMPTY_OCTET,
    EXPECTED_4_OCTETS,
    OCTET_MORE_3_CHARACTERS,
    INVALID_OCTET_SYMBOL,
    OUT_OF_RANGE_OCTET
};

class error : public std::runtime_error {
public:
    template <typename FirstArg, typename... Args>
    explicit error(const FirstArg& arg, const Args&... args) : std::runtime_error(concatenate(arg, args...)) {
    }

    explicit error(const std::string& message) : std::runtime_error(message) {
    }

    explicit error(const char* message) : std::runtime_error(message) {
    }
    
private:
    template <typename... Args>
    static std::string concatenate(const Args&... args) {
        std::ostringstream ss;
        print(ss, args...);
        return ss.str();
    }

    template <typename FirstArg, typename... Args>
    static void print(std::ostringstream& out, const FirstArg& arg, const Args&... args) {
        out << arg << ' ';
        print(out, args...);
    }

    template <typename FirstArg>
    static void print(std::ostringstream& out, const FirstArg& arg) {
        out << arg;
    }
};

class parse_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit parse_error(const FirstArg& arg, const Args&... args) : error(arg, args...) {
    }

    explicit parse_error(const std::string& message) : error(message) {
    }

    explicit parse_error(const char* message) : error(message) {
    }
};

template <typename FixedString>
[[noreturn]] IPADDRESS_CONSTEXPR void raise_error(error_code code, int octet, const FixedString& address) {
    char str[FixedString::length + 1];
    for (size_t i = 0; i < FixedString::length; ++i) {
        str[i] = address[i];
    }
    str[FixedString::length] = '\0';
    switch (code) {
        case error_code::EMPTY_OCTET:
            throw parse_error("empty octet", octet, "in address", str);
        case error_code::EXPECTED_4_OCTETS:
            throw parse_error("expected 4 octets in", str);
        case error_code::OCTET_MORE_3_CHARACTERS:
            throw parse_error("in octet", octet, "of address", str, "more 3 characters");
        case error_code::INVALID_OCTET_SYMBOL:
            throw parse_error("in octet", octet, "of address", str, "has invalid symbol");
        case error_code::OUT_OF_RANGE_OCTET:
            throw parse_error("octet", octet, "of address", str, "exceeded 255");
        default:
            throw error("unknown error");
    }
}

}

#endif
