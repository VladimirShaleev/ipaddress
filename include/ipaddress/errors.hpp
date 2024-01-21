#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

enum class error_code {
    NO_ERROR = 0,
    EMPTY_ADDRESS,

    // ipv4 errors
    EMPTY_OCTET,
    EXPECTED_4_OCTETS,
    LEADING_0_ARE_NOT_PERMITTED,
    OCTET_MORE_3_CHARACTERS,
    OCTET_HAS_INVALID_SYMBOL,
    OCTET_EXCEEDED_255,

    // ipv6 errors
    LEAST_3_PARTS,
    MOST_8_COLONS_PERMITTED,
    PART_IS_MORE_4_CHARS,
    PART_HAS_INVALID_SYMBOL,
    MOST_ONE_DOUBLE_COLON_PERMITTED,
    LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON,
    TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON,
    EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON,
    EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON,
    SCOPE_ID_IS_TOO_LONG,
    INVALID_SCOPE_ID
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

[[noreturn]] IPADDRESS_CONSTEXPR void raise_error(error_code code, int index, const char* address, size_t length) {
    char str[101] = {};
    size_t max_len = length;
    if (length > 100) {
        max_len = 100;
    }
    for (size_t i = 0; i < max_len; ++i) {
        str[i] = address[i];
    }
    switch (code) {
        case error_code::EMPTY_ADDRESS:
            throw parse_error("address cannot be empty");
        case error_code::EMPTY_OCTET:
            throw parse_error("empty octet", index, "in address", str);
        case error_code::EXPECTED_4_OCTETS:
            throw parse_error("expected 4 octets in", str);
        case error_code::LEADING_0_ARE_NOT_PERMITTED:
            throw parse_error("leading zeros are not permitted in octet", index, "of address", str);
        case error_code::OCTET_MORE_3_CHARACTERS:
            throw parse_error("in octet", index, "of address", str, "more 3 characters");
        case error_code::OCTET_HAS_INVALID_SYMBOL:
            throw parse_error("in octet", index, "of address", str, "has invalid symbol");
        case error_code::OCTET_EXCEEDED_255:
            throw parse_error("octet", index, "of address", str, "exceeded 255");
        case error_code::LEAST_3_PARTS:
            throw parse_error("least 3 parts in address", str);
        case error_code::MOST_8_COLONS_PERMITTED:
            throw parse_error("most 8 colons permitted in address", str);
        case error_code::PART_IS_MORE_4_CHARS:
            throw parse_error("in part", index, "of address", str, "more 4 characters");
        case error_code::PART_HAS_INVALID_SYMBOL:
            throw parse_error("in part", index, "of address", str, "has invalid symbols");
        case error_code::MOST_ONE_DOUBLE_COLON_PERMITTED:
            throw parse_error("address cannot be empty");
        case error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON:
            throw parse_error("at leading ':' only permitted as part of '::' in address", str);
        case error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON:
            throw parse_error("at trailing ':' only permitted as part of '::' in address", str);
        case error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON:
            throw parse_error("expected at most 7 other parts with '::' in address", str);
        case error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON:
            throw parse_error("exactly 8 parts expected without '::' in address", str);
        case error_code::SCOPE_ID_IS_TOO_LONG:
            throw parse_error("scope id is too long in address", str);
        case error_code::INVALID_SCOPE_ID:
            throw parse_error("invalid scope id in address", str);
        default:
            throw error("unknown error");
    }
}

}

#endif
