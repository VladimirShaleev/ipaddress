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
 * Enumeration of error codes for IP address parsing and validation.
 * 
 * This enumeration defines a set of error codes used to represent various error conditions
 * that can occur during the parsing and validation of IP addresses. These error codes are
 * used to provide detailed feedback about the nature of the error encountered, allowing
 * for more precise error handling and troubleshooting.
 * 
 * The error codes are divided into several categories:
 * - General errors related to the overall format and structure of the IP address.
 * - IPv4-specific errors that address the unique syntactical requirements of IPv4 addresses.
 * - IPv6-specific errors that address the unique syntactical requirements of IPv6 addresses.
 * - Logical errors that represent inconsistencies or invalid operations in network calculations.
 * 
 * Each error code is accompanied by a brief comment explaining the specific error condition it represents.
 */
enum class error_code {
    no_error = 0, /**< Indicates the absence of any errors. */
    empty_address, /**< The IP address string is empty when it should contain a valid address. */
    empty_netmask, /**< The netmask portion of the address is empty when it should specify a valid netmask. */
    invalid_netmask, /**< The provided netmask is not valid according to standard netmask formatting rules. */
    netmask_pattern_mixes_zeroes_and_ones, /**< The netmask contains an invalid pattern of zeroes and ones. */
    has_host_bits_set, /**< The address has host bits set when they are expected to be clear. */
    only_one_slash_permitted, /**< Only one slash character is permitted, used to separate the address from the netmask. */
    string_is_too_long, /**< Input string is too long. */

    // ipv4 errors
    empty_octet, /**< An octet in the IPv4 address is empty when it should contain a numeric value. */
    expected_4_octets, /**< The IPv4 address does not contain the expected four octets. */
    leading_0_are_not_permitted, /**< Leading zeroes are not permitted in any octet of the IPv4 address. */
    octet_more_3_characters, /**< An octet contains more than three characters, exceeding the maximum allowed. */
    octet_has_invalid_symbol, /**< An octet contains characters other than digits, which are invalid. */
    octet_exceeded_255, /**< An octet's value exceeds the maximum allowed value of 255. */

    // ipv6 errors
    least_3_parts, /**< The IPv6 address contains fewer than the minimum required parts. */
    most_8_colons_permitted, /**< The IPv6 address contains more than the maximum allowed number of colons. */
    part_is_more_4_chars, /**< A part of the IPv6 address contains more than four characters. */
    part_has_invalid_symbol, /**< A part of the IPv6 address contains invalid characters. */
    most_one_double_colon_permitted, /**< More than one double colon is present in the IPv6 address. */
    leading_colon_only_permitted_as_part_of_double_colon, /**< A leading colon is only permitted as part of a double colon. */
    trailing_colon_only_permitted_as_part_of_double_colon, /**< A trailing colon is only permitted as part of a double colon. */
    expected_at_most_7_other_parts_with_double_colon, /**< With a double colon present, at most seven other parts are expected. */
    exactly_8_parts_expected_without_double_colon, /**< Without a double colon, exactly eight parts are expected. */
    scope_id_is_too_long, /**< The scope ID in the IPv6 address exceeds the maximum length. */
    invalid_scope_id, /**< The scope ID in the IPv6 address is invalid. */

    // logic errors
    invalid_version, /**< The IP address version does not match the expected version. */
    invalid_prefixlen_diff, /**< The difference in prefix length is invalid for the operation being performed. */
    new_prefix_must_be_shorter, /**< The new prefix length must be shorter for the operation being performed. */
    new_prefix_must_be_longer, /**< The new prefix length must be longer for the operation being performed. */
    cannot_set_prefixlen_diff_and_new_prefix, /**< Both prefix length difference and new prefix cannot be set simultaneously. */
    not_contained_network, /**< The network is not a subnet of the other network as expected. */
    
    // input string errors
    unexpected_symbol, /**< The input string contains an unexpected character. */
    wrong_encoding_sequence /**< Incorrect byte sequence in Unicode encoding. */
};

/**
 * The primary exception class used by the IP address library.
 * 
 * This class extends the standard `std::runtime_error` and serves as the base exception
 * type for the library. It is designed to be thrown when an error condition is encountered
 * during the processing of IP addresses, such as parsing errors or logical inconsistencies.
 * 
 * The `error` class encapsulates an `error_code` value representing the specific error
 * that occurred, along with a descriptive message that provides context about the error.
 * This design allows for precise and informative error reporting, aiding in debugging
 * and error handling.
 * 
 * Overall, the `error` class is a critical component of the library's error management
 * system, providing a standardized way to handle exceptions throughout the library.
 * 
 * @sa parse_error
 * @sa logic_error
 */
class error : public std::runtime_error {
public:
    /**
     * Constructs an error with a code and a concatenated message from multiple arguments.
     * 
     * @tparam FirstArg The type of the first argument passed to the constructor.
     * @tparam Args The types of the additional arguments passed to the constructor.
     * @param[in] code The error code associated with the exception.
     * @param[in] arg The first argument describing the error.
     * @param[in] args Additional arguments describing the error.
     */
    template <typename FirstArg, typename... Args>
    explicit error(error_code code, const FirstArg& arg, const Args&... args) : std::runtime_error(concatenate(arg, args...)), _code(code) {
    }

    /**
     * Constructs an error with a code and a message string.
     * 
     * @param[in] code The error code associated with the exception.
     * @param[in] message A detailed description of the error.
     */
    explicit error(error_code code, const std::string& message) : std::runtime_error(message), _code(code) {
    }

    /**
     * Constructs an error with a code and a message C-string.
     * 
     * @param[in] code The error code associated with the exception.
     * @param[in] message A detailed description of the error.
     */
    explicit error(error_code code, const char* message) : std::runtime_error(message), _code(code) {
    }
    
    /**
     * Returns the error code associated with this error.
     * 
     * @return The error code that represents the specific error condition.
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
 * Exception for errors encountered during IP address parsing.
 * 
 * This exception is thrown when the library encounters an error while
 * attempting to parse an IP address or network from a string. It is a
 * specialized form of the `error` class that provides context specific
 * to parsing operations.
 * 
 * @sa error
 */
class parse_error : public error {
public:    
    /**
     * Constructs a parsing error with a code and a concatenated message from multiple arguments.
     * 
     * @tparam FirstArg The type of the first argument passed to the constructor.
     * @tparam Args The types of the additional arguments passed to the constructor.
     * @param[in] code The error code associated with the parsing exception.
     * @param[in] arg The first argument describing the error.
     * @param[in] args Additional arguments describing the error.
     */
    template <typename FirstArg, typename... Args>
    explicit parse_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    /**
     * Constructs a parsing error with a code and a message string.
     * 
     * @param[in] code The error code associated with the parsing exception.
     * @param[in] message A detailed description of the parsing error.
     */
    explicit parse_error(error_code code, const std::string& message) : error(code, message) {
    }

    /**
     * Constructs a parsing error with a code and a message C-string.
     * 
     * @param[in] code The error code associated with the parsing exception.
     * @param[in] message A detailed description of the parsing error.
     */
    explicit parse_error(error_code code, const char* message) : error(code, message) {
    }
};

/**
 * Exception for logical errors in IP address operations.
 * 
 * This exception is thrown for logical errors that do not pertain directly
 * to parsing, such as using an incorrect prefix length or other operations
 * that violate the logic of IP address manipulation. It extends the `error`
 * class to provide additional context for these types of logical issues.
 * 
 * @sa error
 */
class logic_error : public error {
public:
    /**
     * Constructs a logic error with a code and a concatenated message from multiple arguments.
     * 
     * @tparam FirstArg The type of the first argument passed to the constructor.
     * @tparam Args The types of the additional arguments passed to the constructor.
     * @param[in] code The error code associated with the logical exception.
     * @param[in] arg The first argument describing the error.
     * @param[in] args Additional arguments describing the error.
     */
    template <typename FirstArg, typename... Args>
    explicit logic_error(error_code code, const FirstArg& arg, const Args&... args) : error(code, arg, args...) {
    }

    /**
     * Constructs a logic error with a code and a message string.
     * 
     * @param[in] code The error code associated with the logical exception.
     * @param[in] message A detailed description of the logical error.
     */
    explicit logic_error(error_code code, const std::string& message) : error(code, message) {
    }

    /**
     * Constructs a logic error with a code and a message C-string.
     * 
     * @param[in] code The error code associated with the logical exception.
     * @param[in] message A detailed description of the logical error.
     */
    explicit logic_error(error_code code, const char* message) : error(code, message) {
    }
};

/**
 * Raises an error with a specific error code and additional context.
 * 
 * This function constructs an error message based on the provided error code,
 * value, and address, then throws a parse_error or a logic_error exception with the constructed message.
 * 
 * @tparam T The character type of the address string.
 * @param[in] code The error code indicating the type of error encountered.
 * @param[in] value The value at which the error occurred, if applicable.
 * @param[in] address A pointer to the beginning of the address string.
 * @param[in] length The length of the address string.
 * @throw parse_error Thrown with a message corresponding to the error code.
 * @throw logic_error Thrown with a message corresponding to the error code.
 * @note This function is marked [[noreturn]] as it always throws an exception.
 */
template <typename T>
#ifndef IPADDRESS_NO_EXCEPTIONS 
[[noreturn]] 
#endif
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void raise_error(error_code code, uint32_t value, const T* address, size_t length) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS;

template <>
#ifndef IPADDRESS_NO_EXCEPTIONS 
[[noreturn]] 
#endif
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void raise_error<char>(error_code code, uint32_t value, const char* address, size_t length) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
#ifndef IPADDRESS_NO_EXCEPTIONS
    switch (code) {
        case error_code::empty_address:
            throw parse_error(code, "address cannot be empty");
        case error_code::empty_netmask:
            throw parse_error(code, "empty mask in address", std::string(address, length));
        case error_code::invalid_netmask:
            throw parse_error(code, "is not a valid netmask in address", std::string(address, length));
        case error_code::netmask_pattern_mixes_zeroes_and_ones:
            throw parse_error(code, "netmask pattern mixes zeroes & ones in address", std::string(address, length));
        case error_code::has_host_bits_set:
            throw parse_error(code, "has host bits set in address", std::string(address, length));
        case error_code::only_one_slash_permitted:
            throw parse_error(code, "only one '/' permitted in address", std::string(address, length));
        case error_code::string_is_too_long:
            throw parse_error(code, "input string is too long", std::string(address, length));
        case error_code::empty_octet:
            throw parse_error(code, "empty octet", value, "in address", std::string(address, length));
        case error_code::expected_4_octets:
            throw parse_error(code, "expected 4 octets in", std::string(address, length));
        case error_code::leading_0_are_not_permitted:
            throw parse_error(code, "leading zeros are not permitted in octet", value, "of address", std::string(address, length));
        case error_code::octet_more_3_characters:
            throw parse_error(code, "in octet", value, "of address", std::string(address, length), "more 3 characters");
        case error_code::octet_has_invalid_symbol:
            throw parse_error(code, "in octet", value, "of address", std::string(address, length), "has invalid symbol");
        case error_code::octet_exceeded_255:
            throw parse_error(code, "octet", value, "of address", std::string(address, length), "exceeded 255");
        case error_code::least_3_parts:
            throw parse_error(code, "least 3 parts in address", std::string(address, length));
        case error_code::most_8_colons_permitted:
            throw parse_error(code, "most 8 colons permitted in address", std::string(address, length));
        case error_code::part_is_more_4_chars:
            throw parse_error(code, "in part", value, "of address", std::string(address, length), "more 4 characters");
        case error_code::part_has_invalid_symbol:
            throw parse_error(code, "in part", value, "of address", std::string(address, length), "has invalid symbols");
        case error_code::most_one_double_colon_permitted:
            throw parse_error(code, "at most one '::' permitted in address", std::string(address, length));
        case error_code::leading_colon_only_permitted_as_part_of_double_colon:
            throw parse_error(code, "at leading ':' only permitted as part of '::' in address", std::string(address, length));
        case error_code::trailing_colon_only_permitted_as_part_of_double_colon:
            throw parse_error(code, "at trailing ':' only permitted as part of '::' in address", std::string(address, length));
        case error_code::expected_at_most_7_other_parts_with_double_colon:
            throw parse_error(code, "expected at most 7 other parts with '::' in address", std::string(address, length));
        case error_code::exactly_8_parts_expected_without_double_colon:
            throw parse_error(code, "exactly 8 parts expected without '::' in address", std::string(address, length));
        case error_code::scope_id_is_too_long:
            throw parse_error(code, "scope id is too long in address", std::string(address, length));
        case error_code::invalid_scope_id:
            throw parse_error(code, "invalid scope id in address", std::string(address, length));
        case error_code::invalid_version:
            throw logic_error(code, "versions don't match");
        case error_code::invalid_prefixlen_diff:
            throw logic_error(code, "invalid prefixlen_diff");
        case error_code::new_prefix_must_be_shorter:
            throw logic_error(code, "new prefix must be shorter");
        case error_code::new_prefix_must_be_longer:
            throw logic_error(code, "new prefix must be longer");
        case error_code::cannot_set_prefixlen_diff_and_new_prefix:
            throw logic_error(code, "cannot set prefixlen_diff and new_prefix");
        case error_code::not_contained_network:
            throw logic_error(code, "network is not a subnet of other");
        case error_code::unexpected_symbol: {
            std::ostringstream ss;
            ss << "{U+" << std::setw(4) << std::setfill('0') << std::hex << value << '}';
            throw parse_error(code, "unexpected next unicode symbol", ss.str(), "in string", std::string(address, length));
        }
        case error_code::wrong_encoding_sequence:
            throw parse_error(code, "incorrect sequence of bytes in unicode encoding for string", std::string(address, length));
        default:
            throw error(code, "unknown error");
    }
#else
    if (IPADDRESS_IS_CONST_EVALUATED(length)) {
        const auto _err = int(code) / (int(code) - int(code)); // invalid input string
    }
#endif
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_ERRORS_HPP
