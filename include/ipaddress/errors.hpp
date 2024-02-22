/**
 * @file      errors.hpp
 * @brief     Declares error codes and exceptions for IP address operations
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file is part of an IP address processing library and is responsible
 * for defining error codes and exceptions that represent various failure states
 * encountered during IP address parsing and processing. It includes an enumeration
 * of error codes that cover a range of issues from syntactical errors in IP address
 * strings to logical inconsistencies in network operations. Additionally, the file
 * declares a base exception class tailored to encapsulate these error codes along
 * with descriptive messages, facilitating precise error handling and reporting.
 * 
 * The error codes are categorized into general parsing errors, IPv4-specific errors,
 * IPv6-specific errors, and logical errors, each with a brief description of the
 * condition they represent. The base exception class, `error`, inherits from the
 * standard `std::runtime_error` and is constructed with an error code and a message,
 * allowing exceptions to carry both a machine-readable identifier and a human-readable
 * explanation.
 * 
 * The file also includes a function template that converts an error code into an
 * exception, streamlining the process of throwing exceptions upon encountering
 * errors during IP address operations. This design ensures that all potential error
 * conditions are accounted for and can be handled gracefully by client code using
 * the library.
 */

#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Error codes.
 * 
 * Enumeration for identifying IP-address parsing errors and other logical errors.
 */
enum class error_code {
    NO_ERROR = 0, /**< No errors */
    EMPTY_ADDRESS, /**< Address cannot be empty */
    EMPTY_NETMASK, /**< Empty mask in address */
    INVALID_NETMASK, /**< Not valid netmask in address */
    NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, /**< Netmask pattern mixes zeroes & ones in address */
    HAS_HOST_BITS_SET, /**< Has host bits set in address */
    ONLY_ONE_SLASH_PERMITTED, /**< Only one '/' permitted in address */

    // ipv4 errors
    EMPTY_OCTET, /**< Empty octet in address */
    EXPECTED_4_OCTETS, /**< Expected 4 octets in address*/
    LEADING_0_ARE_NOT_PERMITTED, /**< Leading zeros are not permitted in octet of address */
    OCTET_MORE_3_CHARACTERS, /**< In octet of address more 3 characters */
    OCTET_HAS_INVALID_SYMBOL, /**< In octet of address has invalid symbol */
    OCTET_EXCEEDED_255, /**< Octet of address exceeded 255 */

    // ipv6 errors
    LEAST_3_PARTS, /**< Least 3 parts in address */
    MOST_8_COLONS_PERMITTED, /**< Most 8 colons permitted in address */
    PART_IS_MORE_4_CHARS, /**< In part of address more 4 characters */
    PART_HAS_INVALID_SYMBOL, /**< In part of address has invalid symbols */
    MOST_ONE_DOUBLE_COLON_PERMITTED, /**< At most one '::' permitted in address */
    LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, /**< At leading ':' only permitted as part of '::' in address */
    TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, /**< At trailing ':' only permitted as part of '::' in address */
    EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, /**< Expected at most 7 other parts with '::' in address */
    EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, /**< Exactly 8 parts expected without '::' in address */
    SCOPE_ID_IS_TOO_LONG, /**< Scope id is too long in address */
    INVALID_SCOPE_ID, /**< Invalid scope id in address */

    // logic errors
    INVALID_VERSION, /**< Versions don't match */
    INVALID_PREFIXLEN_DIFF, /**< Invalid prefixlen diff */
    NEW_PREFIX_MUST_BE_SHORTER, /**< New prefix must be shorter */
    NEW_PREFIX_MUST_BE_LONGER, /**< New prefix must be longer */
    CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX, /**< Cannot set prefixlen diff and new prefix */
    NOT_CONTAINED_NETWORK /**< Network is not a subnet of other */
};

/**
 * Base exception for this library.
 * 
 * Basic exception that can be thrown by this library, for example, 
 * if it is impossible to parse a string with an IP address.
 */
class error : public std::runtime_error {
public:
    template <typename FirstArg, typename... Args>
    explicit error(error_code code, const FirstArg& arg, const Args&... args) : std::runtime_error(concatenate(arg, args...)), _code(code) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit error(error_code code, const std::string& message) : std::runtime_error(message), _code(code) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit error(error_code code, const char* message) : std::runtime_error(message), _code(code) {
    }
    
    /**
     * Get error code.
     * 
     * Constant by which an error can be identified.
     * 
     * @return error code
     */
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

/**
 * Parsing exception.
 * 
 * Thrown when there are errors in parsing an IP address or network from a string.
 */
class parse_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit parse_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit parse_error(error_code code, const std::string& message) : error(code, message) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit parse_error(error_code code, const char* message) : error(code, message) {
    }
};

/**
 * Logic exception.
 * 
 * Thrown in cases such as using an incorrect prefix length, etc.
 */
class logic_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit logic_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit logic_error(error_code code, const std::string& message) : error(code, message) {
    }

    /**
     * Create an instance from an error code and a string describing the error.
     *
     * Will save the error code and a line describing the error
     * 
     * @param[in] code of error
     * @param[in] message of detailed description of the error
     */
    explicit logic_error(error_code code, const char* message) : error(code, message) {
    }
};

template <typename T>
[[noreturn]] IPADDRESS_CONSTEXPR inline void raise_error(error_code code, int index, const T* address, size_t length) {
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
        case error_code::INVALID_VERSION:
            throw logic_error(code, "versions don't match");
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
