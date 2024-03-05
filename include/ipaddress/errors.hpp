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
    NoError = 0, /**< Indicates the absence of any errors. */
    EmptyAddress, /**< The IP address string is empty when it should contain a valid address. */
    EmptyNetmask, /**< The netmask portion of the address is empty when it should specify a valid netmask. */
    InvalidNetmask, /**< The provided netmask is not valid according to standard netmask formatting rules. */
    NetmaskPatternMixesZeroesAndOnes, /**< The netmask contains an invalid pattern of zeroes and ones. */
    HasHostBitsSet, /**< The address has host bits set when they are expected to be clear. */
    OnlyOneSlashPermitted, /**< Only one slash character is permitted, used to separate the address from the netmask. */
    StringIsTooLong, /**< Input string is too long. */

    // ipv4 errors
    EmptyCctet, /**< An octet in the IPv4 address is empty when it should contain a numeric value. */
    Expected4Octets, /**< The IPv4 address does not contain the expected four octets. */
    Leading0AreNotPermitted, /**< Leading zeroes are not permitted in any octet of the IPv4 address. */
    OctetMore3Characters, /**< An octet contains more than three characters, exceeding the maximum allowed. */
    OctetHasInvalidSymbol, /**< An octet contains characters other than digits, which are invalid. */
    OctetExceeded255, /**< An octet's value exceeds the maximum allowed value of 255. */

    // ipv6 errors
    Least3Parts, /**< The IPv6 address contains fewer than the minimum required parts. */
    Most8ColonsPermitted, /**< The IPv6 address contains more than the maximum allowed number of colons. */
    PartIsMore4Chars, /**< A part of the IPv6 address contains more than four characters. */
    PartHasInvalidSymbol, /**< A part of the IPv6 address contains invalid characters. */
    MostOneDoubleColonPermitted, /**< More than one double colon is present in the IPv6 address. */
    LeadingColonOnlyPermittedAsPartOfDoubleColon, /**< A leading colon is only permitted as part of a double colon. */
    TrailingColonOnlyPermittedAsPartOfDoubleColon, /**< A trailing colon is only permitted as part of a double colon. */
    ExpectedAtMost7OtherPartsWithDoubleColon, /**< With a double colon present, at most seven other parts are expected. */
    Exactly8PartsExpectedWithoutDoubleColon, /**< Without a double colon, exactly eight parts are expected. */
    ScopeIdIsTooLong, /**< The scope ID in the IPv6 address exceeds the maximum length. */
    InvalidScopeId, /**< The scope ID in the IPv6 address is invalid. */

    // logic errors
    InvalidVersion, /**< The IP address version does not match the expected version. */
    InvalidPrefixlenDiff, /**< The difference in prefix length is invalid for the operation being performed. */
    NewPrefixMustBeShorter, /**< The new prefix length must be shorter for the operation being performed. */
    NewPrefixMustBeLonger, /**< The new prefix length must be longer for the operation being performed. */
    CannotSetPrefixlenDiffAndNewPrefix, /**< Both prefix length difference and new prefix cannot be set simultaneously. */
    NotContainedNetwork /**< The network is not a subnet of the other network as expected. */
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
 * index, and address, then throws a parse_error or a logic_error exception with the constructed message.
 * 
 * @tparam T The character type of the address string.
 * @param[in] code The error code indicating the type of error encountered.
 * @param[in] index The index at which the error occurred, if applicable.
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
IPADDRESS_CONSTEXPR inline void raise_error(error_code code, int index, const T* address, size_t length) {
#ifndef IPADDRESS_NO_EXCEPTIONS
    char str[101] = {};
    size_t max_len = length;
    if (length > 100) {
        max_len = 100;
    }
    for (size_t i = 0; i < max_len; ++i) {
        str[i] = char(address[i]);
    }
    switch (code) {
        case error_code::EmptyAddress:
            throw parse_error(code, "address cannot be empty");
        case error_code::EmptyNetmask:
            throw parse_error(code, "empty mask in address", str);
        case error_code::InvalidNetmask:
            throw parse_error(code, "is not a valid netmask in address", str);
        case error_code::NetmaskPatternMixesZeroesAndOnes:
            throw parse_error(code, "netmask pattern mixes zeroes & ones in address", str);
        case error_code::HasHostBitsSet:
            throw parse_error(code, "has host bits set in address", str);
        case error_code::OnlyOneSlashPermitted:
            throw parse_error(code, "only one '/' permitted in address", str);
        case error_code::StringIsTooLong:
            throw parse_error(code, "input string is too long", str);
        case error_code::EmptyCctet:
            throw parse_error(code, "empty octet", index, "in address", str);
        case error_code::Expected4Octets:
            throw parse_error(code, "expected 4 octets in", str);
        case error_code::Leading0AreNotPermitted:
            throw parse_error(code, "leading zeros are not permitted in octet", index, "of address", str);
        case error_code::OctetMore3Characters:
            throw parse_error(code, "in octet", index, "of address", str, "more 3 characters");
        case error_code::OctetHasInvalidSymbol:
            throw parse_error(code, "in octet", index, "of address", str, "has invalid symbol");
        case error_code::OctetExceeded255:
            throw parse_error(code, "octet", index, "of address", str, "exceeded 255");
        case error_code::Least3Parts:
            throw parse_error(code, "least 3 parts in address", str);
        case error_code::Most8ColonsPermitted:
            throw parse_error(code, "most 8 colons permitted in address", str);
        case error_code::PartIsMore4Chars:
            throw parse_error(code, "in part", index, "of address", str, "more 4 characters");
        case error_code::PartHasInvalidSymbol:
            throw parse_error(code, "in part", index, "of address", str, "has invalid symbols");
        case error_code::MostOneDoubleColonPermitted:
            throw parse_error(code, "at most one '::' permitted in address", str);
        case error_code::LeadingColonOnlyPermittedAsPartOfDoubleColon:
            throw parse_error(code, "at leading ':' only permitted as part of '::' in address", str);
        case error_code::TrailingColonOnlyPermittedAsPartOfDoubleColon:
            throw parse_error(code, "at trailing ':' only permitted as part of '::' in address", str);
        case error_code::ExpectedAtMost7OtherPartsWithDoubleColon:
            throw parse_error(code, "expected at most 7 other parts with '::' in address", str);
        case error_code::Exactly8PartsExpectedWithoutDoubleColon:
            throw parse_error(code, "exactly 8 parts expected without '::' in address", str);
        case error_code::ScopeIdIsTooLong:
            throw parse_error(code, "scope id is too long in address", str);
        case error_code::InvalidScopeId:
            throw parse_error(code, "invalid scope id in address", str);
        case error_code::InvalidVersion:
            throw logic_error(code, "versions don't match");
        case error_code::InvalidPrefixlenDiff:
            throw logic_error(code, "invalid prefixlen_diff");
        case error_code::NewPrefixMustBeShorter:
            throw logic_error(code, "new prefix must be shorter");
        case error_code::NewPrefixMustBeLonger:
            throw logic_error(code, "new prefix must be longer");
        case error_code::CannotSetPrefixlenDiffAndNewPrefix:
            throw logic_error(code, "cannot set prefixlen_diff and new_prefix");
        case error_code::NotContainedNetwork:
            throw logic_error(code, "network is not a subnet of other");
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
