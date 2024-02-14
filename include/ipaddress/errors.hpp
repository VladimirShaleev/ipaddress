#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

enum class error_code {
    NO_ERROR = 0,
    EMPTY_ADDRESS,
    EMPTY_NETMASK,
    INVALID_NETMASK,
    NETMASK_PATTERN_MIXES_ZEROES_AND_ONES,
    HAS_HOST_BITS_SET,
    ONLY_ONE_SLASH_PERMITTED,

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
    INVALID_SCOPE_ID,

    // logic errors
    INVALID_PREFIXLEN_DIFF,
    NEW_PREFIX_MUST_BE_SHORTER,
    NEW_PREFIX_MUST_BE_LONGER,
    CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX,
    NOT_CONTAINED_NETWORK
};

class error : public std::runtime_error {
public:
    template <typename FirstArg, typename... Args>
    explicit error(error_code code, const FirstArg& arg, const Args&... args) : std::runtime_error(concatenate(arg, args...)), _code(code) {
    }

    explicit error(error_code code, const std::string& message) : std::runtime_error(message), _code(code) {
    }

    explicit error(error_code code, const char* message) : std::runtime_error(message), _code(code) {
    }
    
    IPADDRESS_NODISCARD error_code code() const IPADDRESS_NOEXCEPT {
        return _code;
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

    error_code _code;
};

class parse_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit parse_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    explicit parse_error(error_code code, const std::string& message) : error(code, message) {
    }

    explicit parse_error(error_code code, const char* message) : error(code, message) {
    }
};

class logic_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit logic_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    explicit logic_error(error_code code, const std::string& message) : error(code, message) {
    }

    explicit logic_error(error_code code, const char* message) : error(code, message) {
    }
};

template <typename T>
[[noreturn]] IPADDRESS_CONSTEXPR void raise_error(error_code code, int index, const T* address, size_t length) {
    char str[101] = {};
    size_t max_len = length;
    if (length > 100) {
        max_len = 100;
    }
    for (size_t i = 0; i < max_len; ++i) {
        str[i] = char(address[i]);
    }
    switch (code) {
        case error_code::EMPTY_ADDRESS:
            throw parse_error(code, "address cannot be empty");
        case error_code::EMPTY_NETMASK:
            throw parse_error(code, "empty mask in address", str);
        case error_code::INVALID_NETMASK:
            throw parse_error(code, "is not a valid netmask in address", str);
        case error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES:
            throw parse_error(code, "netmask pattern mixes zeroes & ones in address", str);
        case error_code::HAS_HOST_BITS_SET:
            throw parse_error(code, "has host bits set in address", str);
        case error_code::ONLY_ONE_SLASH_PERMITTED:
            throw parse_error(code, "only one '/' permitted in address", str);
        case error_code::EMPTY_OCTET:
            throw parse_error(code, "empty octet", index, "in address", str);
        case error_code::EXPECTED_4_OCTETS:
            throw parse_error(code, "expected 4 octets in", str);
        case error_code::LEADING_0_ARE_NOT_PERMITTED:
            throw parse_error(code, "leading zeros are not permitted in octet", index, "of address", str);
        case error_code::OCTET_MORE_3_CHARACTERS:
            throw parse_error(code, "in octet", index, "of address", str, "more 3 characters");
        case error_code::OCTET_HAS_INVALID_SYMBOL:
            throw parse_error(code, "in octet", index, "of address", str, "has invalid symbol");
        case error_code::OCTET_EXCEEDED_255:
            throw parse_error(code, "octet", index, "of address", str, "exceeded 255");
        case error_code::LEAST_3_PARTS:
            throw parse_error(code, "least 3 parts in address", str);
        case error_code::MOST_8_COLONS_PERMITTED:
            throw parse_error(code, "most 8 colons permitted in address", str);
        case error_code::PART_IS_MORE_4_CHARS:
            throw parse_error(code, "in part", index, "of address", str, "more 4 characters");
        case error_code::PART_HAS_INVALID_SYMBOL:
            throw parse_error(code, "in part", index, "of address", str, "has invalid symbols");
        case error_code::MOST_ONE_DOUBLE_COLON_PERMITTED:
            throw parse_error(code, "at most one '::' permitted in address", str);
        case error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON:
            throw parse_error(code, "at leading ':' only permitted as part of '::' in address", str);
        case error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON:
            throw parse_error(code, "at trailing ':' only permitted as part of '::' in address", str);
        case error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON:
            throw parse_error(code, "expected at most 7 other parts with '::' in address", str);
        case error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON:
            throw parse_error(code, "exactly 8 parts expected without '::' in address", str);
        case error_code::SCOPE_ID_IS_TOO_LONG:
            throw parse_error(code, "scope id is too long in address", str);
        case error_code::INVALID_SCOPE_ID:
            throw parse_error(code, "invalid scope id in address", str);
        case error_code::INVALID_PREFIXLEN_DIFF:
            throw logic_error(code, "invalid prefixlen_diff");
        case error_code::NEW_PREFIX_MUST_BE_SHORTER:
            throw logic_error(code, "new prefix must be shorter");
        case error_code::NEW_PREFIX_MUST_BE_LONGER:
            throw logic_error(code, "new prefix must be longer");
        case error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX:
            throw logic_error(code, "cannot set prefixlen_diff and new_prefix");
        case error_code::NOT_CONTAINED_NETWORK:
            throw logic_error(code, "network is not a subnet of other");
        default:
            throw error(code, "unknown error");
    }
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_ERRORS_HPP
