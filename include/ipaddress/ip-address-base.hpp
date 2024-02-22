/**
 * @file      ip-address-base.hpp
 * @brief     Provides basic functionality for IP addresses
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file includes the core functionalities and definitions that are common to
 * both IPv4 and IPv6 address handling. It serves as a foundational component for building
 * specialized IP address classes and utilities.
 */

#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "endian.hpp"
#include "optional.hpp"
#include "byte-array.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Enumerates the IP address versions.
 * 
 * Defines constants representing the two versions of Internet Protocol: IPv4 and IPv6.
 */
enum class ip_version {
    V4 = 4, /**< IPv4 version identifier. */
    V6 = 6 /**< IPv6 version identifier. */
};

/**
 * Enumerates the formatting options for IP address strings.
 * 
 * Defines the formatting styles that can be applied when converting IP addresses to strings,
 * such as full, compact, or compressed representations.
 */
enum class format {
    full = 0, /**< Full format with no compression or omission. */
    compact, /**< Compact format with possible omission of leading zeros. */
    compressed /**< Compressed format with maximal omission of segments or octets. */
};

/**
 * A template base class for IP address representations.
 * 
 * This class template serves as a base for creating IP address objects. It 
 * inherits from a base class that provides the necessary functionality, and 
 * it is extended by more specific IP address classes. 
 * 
 * @tparam Base the base class providing storage and low-level IP address functionalities
 */
template <typename Base>
class ip_address_base : public Base {
public:
    template <typename>
    friend class ip_network_base;

    using base_type = typename Base::base_type; /**< Type alias for the base storage type. */
    using uint_type = typename Base::uint_type; /**< Type alias for the underlying unsigned integer type. */

    /**
     * Default constructor.
     * 
     * Constructs an empty IP address object.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base() IPADDRESS_NOEXCEPT : Base() {
    }

    /**
     * Constructs an IP address object from a byte array.
     * 
     * @param[in] bytes the byte array representing an IP address
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : Base(bytes) {
    }

    /**
     * Static factory method to create an IP address object from a byte array.
     * 
     * @param[in] bytes the byte array representing an IP address
     * @return    An instance of ip address constructed from the given byte array.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base from_bytes(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return ip_address_base(bytes);
    }

    /**
     * Static factory method to create an IP address object from a raw byte buffer.
     * 
     * @param[in]   bytes      pointer to the raw byte buffer representing an IP address
     * @param[in]   byte_count the number of bytes in the buffer to use
     * @return      An instance of ip address constructed from the given byte buffer.
     * @parblock
     * @remark      If the number of bytes \a byte_count is less than the target number 
     *              of bytes to represent the IP address, the missing bytes will be 
     *              filled with zeros.
     * @endparblock
     * @parblock
     * @remark      If the number of bytes \a byte_count is greater than the target 
     *              number of bytes represented by the IP address, then the extra 
     *              bytes will be ignored
     * @endparblock
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base from_bytes(const uint8_t* bytes, size_t byte_count) IPADDRESS_NOEXCEPT {
        base_type data = {};
        for (size_t i = 0; i < Base::base_size && i < byte_count; ++i) {
            data[i] = bytes[i];
        }
        return ip_address_base(data);
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * Static method template to parse an IP address from a fixed string at compile time.
     * 
     * @tparam FixedString a non-type template parameter that holds the string representation of the IP address.
     * @return An instance of ip address parsed from the fixed string.
     * @note   This method is only available when non-type template parameters for strings are supported.
     * @remark If parsing fails, an error will be raised at compile time.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval IPADDRESS_FORCE_INLINE ip_address_base<Base> parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(str.begin(), str.end(), code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return ip_address_base(result);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    /**
     * Parses an IP address from a string view.
     * 
     * This method provides a way to parse an IP address from a string view, 
     * which is a non-owning reference to a sequence of characters.
     * 
     * @param[in] address the string view containing the IP address to parse
     * @return    An instance of ip address parsed from the string view.
     * @throw     parse_error exception caused by invalid input string
     * @note      This method is available for C++17 and later versions.
     * @remark    For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a wide string view.
     * 
     * This method allows for parsing an IP address from a wide string view, 
     * which is typically used for wide character strings.
     * 
     * @param[in] address the wide string view containing the IP address to parse
     * @return    An instance of ip address parsed from the string view.
     * @throw     parse_error exception caused by invalid input string
     * @note      This method is available for C++17 and later versions.
     * @remark    For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::wstring_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses an IP address from a UTF-8 string view.
     * 
     * This method parses an IP address from a given UTF-8 string view. It leverages the `char8_t`
     * type introduced in C++20 to handle UTF-8 strings natively.
     * 
     * @param[in] address a UTF-8 string view containing the IP address to parse
     * @return    An instance of ip address parsed from the string view.
     * @throw     parse_error exception caused by invalid input string
     * @note      This method is available for C++20 and later versions where `char8_t` is supported.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u8string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

#endif // __cpp_char8_t

    /**
     * Parses an IP address from a UTF-16 string view.
     * 
     * This method is designed to parse an IP address from a UTF-16 string view, 
     * which is a sequence of 16-bit characters.
     * 
     * @param[in] address the UTF-16 string view containing the IP address to parse
     * @return    An instance of ip address parsed from the string view.
     * @throw     parse_error exception caused by invalid input string
     * @note      This method is available for C++17 and later versions.
     * @remark    For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u16string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-32 string view.
     * 
     * This method allows for parsing an IP address from a UTF-32 string view, 
     * which is a sequence of 32-bit characters.
     * 
     * @param[in] address the UTF-32 string view containing the IP address to parse
     * @return    An instance of ip address parsed from the string view.
     * @throw     parse_error exception caused by invalid input string
     * @note      This method is available for C++17 and later versions.
     * @remark    For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u32string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a string view and reports errors through an error code.
     * 
     * This method parses an IP address from a string view and provides an error code if the parsing fails.
     * 
     * @param[in]  address the string view containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the string view. If parsing fails, the returned object will be in an unspecified state.
     * @note       This method is available for C++17 and later versions.
     * @remark     For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a wide string view and reports errors through an error code.
     * 
     * This method parses an IP address from a wide string view and provides an error code if the parsing fails.
     * 
     * @param[in]  address the wide string view containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the wide string view. If parsing fails, the returned object will be in an unspecified state.
     * @note       This method is available for C++17 and later versions.
     * @remark     For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::wstring_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses an IP address from a UTF-8 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-8 string view and provides an error code if the parsing fails.
     * 
     * @param[in]  address the UTF-8 string view containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the UTF-8 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note       This method is available for C++20 and later versions where `char8_t` is supported.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u8string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // __cpp_char8_t

    /**
     * Parses an IP address from a UTF-16 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-16 string view and provides an error code if the parsing fails.
     * 
     * @param[in]  address the UTF-16 string view containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the UTF-16 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note       This method is available for C++17 and later versions.
     * @remark     For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u16string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-32 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-32 string view and provides an error code if the parsing fails.
     * 
     * @param[in]  address the UTF-32 string view containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the UTF-32 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note       This method is available for C++17 and later versions.
     * @remark     For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u32string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#else // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses an IP address from a `std::string`.
     * 
     * @param[in] address the `std::string` containing the IP address to parse
     * @return    An instance of ip address parsed from the string.
     * @throw     parse_error exception caused by invalid input string
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a wide `std::wstring`.
     * 
     * @param[in] address the wide `std::wstring` containing the IP address to parse
     * @return    An instance of ip address parsed from the string.
     * @throw     parse_error exception caused by invalid input string
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::wstring& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-16 `std::u16string`.
     * 
     * @param[in] address the UTF-16 `std::u16string` containing the IP address to parse
     * @return    An instance of ip address parsed from the string.
     * @throw     parse_error exception caused by invalid input string
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u16string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-32 `std::u32string`.
     * 
     * @param[in] address the UTF-32 `std::u32string` containing the IP address to parse
     * @return    An instance of ip address parsed from the string.
     * @throw     parse_error exception caused by invalid input string
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u32string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a `std::string` and reports errors through an error code.
     * 
     * @param[in]  address the `std::string` containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a wide `std::wstring` and reports errors through an error code.
     * 
     * @param[in]  address the wide `std::wstring` containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the wide string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::wstring& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-16 `std::u16string` and reports errors through an error code.
     * 
     * @param[in]  address the UTF-16 `std::u16string` containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the UTF-16 string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u16string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-32 `std::u32string` and reports errors through an error code.
     * 
     * @param[in]  address the UTF-32 `std::u32string` containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the UTF-32 string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u32string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses an IP address from a character array.
     * 
     * This method template parses an IP address from a character array of a 
     * specified size. Can check and get the result at compile time.
     * 
     * @tparam    T       the character type of the array
     * @tparam    N       the size of the character array
     * @param[in] address the character array containing the IP address to parse
     * @return    An instance of ip address parsed from the character array.
     */
    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(const T(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        return parse_string(str);
    }

    /**
     * Parses an IP address from a character array and reports errors through an error code.
     * 
     * This method template parses an IP address from a character array of a specified size 
     * and provides an error code if the parsing fails.
     * 
     * @tparam     T       the character type of the array
     * @tparam     N       the size of the character array
     * @param[in]  address the character array containing the IP address to parse
     * @param[out] code    a reference to an `error_code` object that will be set if an error occurs during parsing
     * @return     An instance of ip address parsed from the character array. If parsing fails, the returned object will be in an unspecified state.
     */
    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(const T(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        return parse_string(str, code);
    }

    /**
     * Retrieves the raw data representing the IP address in **network byte order** (big-endian).
     * 
     * This method returns a pointer to the underlying byte array that stores the IP address.
     * 
     * @return A pointer to the constant byte array containing the raw IP address data.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const uint8_t* data() const IPADDRESS_NOEXCEPT {
        return Base::bytes().data();
    }

    /**
     * Checks if the IP address is a multicast address.
     * 
     * @return `true` if the IP address is reserved for multicast use, `false` otherwise.
     * @see    [RFC 3171 for IPv4](https://datatracker.ietf.org/doc/html/rfc3171.html)
     * @see    [RFC 2373 for IPv6](https://datatracker.ietf.org/doc/html/rfc2373.html)
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT;

    /**
     * Checks if the IP address is a private address.
     * 
     * @return `true` if the IP address is allocated for private networks, `false` otherwise.
     * @see    [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml)
     * @see    [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml)
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        const auto& b = Base::bytes();
        for (size_t i = 0; i < Base::base_size; ++i) {
            if (b[i] != 0) {
                return false;
            }
        }
        return true;
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        char res[Base::base_max_string_len + 1]{};
        const auto len = Base::ip_to_chars(Base::bytes(), fmt, res);
        return std::string(res, len);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_address_base& ip) IPADDRESS_NOEXCEPT {
        Base::swap(*this, ip);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return Base::hash(Base::bytes());
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string reverse_pointer() const {
       return Base::ip_reverse_pointer(Base::bytes());
    }

    IPADDRESS_NODISCARD explicit IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE operator uint_type() const IPADDRESS_NOEXCEPT {
        return Base::to_uint();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::equals(*this, rhs);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::compare(*this, rhs);
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::less(*this, rhs);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(address.begin(), address.end(), code, index);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, index, address.data(), address.size());
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, address.data(), address.size());
        #endif
        }
        return result;
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return Base::ip_from_string(address.begin(), address.end(), code, index);
    }
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE int stream_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

IPADDRESS_FORCE_INLINE std::ostream& full(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::full) + 1;
    return stream;
}

IPADDRESS_FORCE_INLINE std::ostream& compact(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compact) + 1;
    return stream;
}

IPADDRESS_FORCE_INLINE std::ostream& compressed(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compressed) + 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <typename Base>
struct hash<IPADDRESS_NAMESPACE::ip_address_base<Base>> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) const IPADDRESS_NOEXCEPT {
        return ip.hash();
    }
};

template <typename Base>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_address_base<Base>& ip1, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip2) IPADDRESS_NOEXCEPT {
    ip1.swap(ip2);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    return ip.to_string();
}

template <typename Base>
IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    auto str = ip.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        auto end = std::find(str.cbegin(), str.cend(), '%');
        std::transform(str.cbegin(), end, str.begin(), [](auto c){
            return std::toupper(c);
        });
    }
    return stream << str;
}

template <typename Base>
IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    ip = IPADDRESS_NAMESPACE::ip_address_base<Base>::parse(str, err);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ADDRESS_BASE_HPP
