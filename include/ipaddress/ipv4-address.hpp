/**
 * @file      ipv4-address.hpp
 * @brief     Provides a set of functions and classes for handling IPv4 addresses
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * Includes functionalities to convert IPv4 addresses to and from various formats,
 * perform comparisons, and query specific properties of the addresses.
 * It serves as a foundational component for network applications that require manipulation
 * and analysis of IPv4 address data.
 */

#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "base-v4.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Represents the base class for IPv4 address manipulation.
 * 
 * This class provides the basic functionalities required for handling IPv4 addresses,
 * including conversion to and from numeric representations, access to the underlying bytes,
 * and utility functions that are common across different representations of IPv4 addresses.
 */
class ipv4_address_base : public base_v4<ipv4_address_base> {
public:
    using base_type = typename base_v4<ipv4_address_base>::base_type; /**< The base type for the IPv4 address. */
    using uint_type = typename base_v4<ipv4_address_base>::uint_type; /**< The unsigned integer type for the IPv4 address. */

    /**
     * Creates an IPv4 address from an unsigned integer using a template parameter.
     * 
     * @tparam Ip The unsigned integer representing the IPv4 address.
     * @return An instance of ip address representing the IPv4 address.
     */
    template <uint_type Ip>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<ipv4_address_base> from_uint() IPADDRESS_NOEXCEPT {
        return from_uint(Ip);
    }

    /**
     * Creates an IPv4 address from an unsigned integer.
     * 
     * @param[in] ip The unsigned integer representing the IPv4 address.
     * @return An instance of ip address representing the IPv4 address.
     * @remark Bytes in integer \a ip must be presented in **host byte order**.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<ipv4_address_base> from_uint(uint_type ip) IPADDRESS_NOEXCEPT {
        return ip_from_uint32(ip);
    }

    /**
     * Converts the IPv4 address to an unsigned integer.
     * 
     * @return The unsigned integer representation of the IPv4 address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type to_uint() const IPADDRESS_NOEXCEPT {
        return ip_to_uint32(_bytes);
    }

    /**
     * Provides access to the underlying bytes of the IPv4 address.
     * 
     * @return A reference to the base type containing the bytes of the IPv4 address.
     * @remark Retrieves the data representing the IP address in **network byte order** (big-endian).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _bytes;
    }

protected:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address_base() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ipv4_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _bytes(bytes) {
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(ip_address_base<ipv4_address_base>& lhs, ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        lhs._bytes.swap(rhs._bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t hash(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return calc_hash(0, size_t(bytes[0]), size_t(bytes[1]), size_t(bytes[2]), size_t(bytes[3]));
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool equals(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes == rhs._bytes;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool less(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes < rhs._bytes;
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering compare(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes <=> rhs._bytes;
    }

#endif // IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    base_type _bytes{};
}; // ipv4_address_base

/**
 * Alias for the base class specialized for IPv4 address manipulation.
 * 
 * This alias provides a convenient shorthand for the specialized `ip_address_base` class
 * tailored for IPv4 address handling. It inherits all functionalities from the `ipv4_address_base`
 * class, allowing for operations such as conversion, comparison, and property querying
 * specific to IPv4 addresses.
 */
using ipv4_address = ip_address_base<ipv4_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ipv4_address from a fixed string at compile time.
     * 
     * @tparam FixedString A compile-time fixed string representing the IPv4 address.
     * @return An ipv4_address object parsed from the fixed string.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4() IPADDRESS_NOEXCEPT {
        return ipv4_address::parse<FixedString>();
    }

    /**
     * User-defined literal for creating an ipv4_address from an unsigned long long integer at compile time.
     * 
     * @param[in] value An unsigned long long integer representing the IPv4 address in *host byte order*.
     * @return An ipv4_address object created from the integer.
     */
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT {
        assert(value <= ipv4_address::base_all_ones && "literal integer is too long");
        return ipv4_address::from_uint(uint32_t(value));
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ipv4_address from a string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_address::base_max_string_len && "literal string is too long");
        char str[ipv4_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_address::parse(str);
    }

    /**
     * User-defined literal for creating an ipv4_address from a wide string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(const wchar_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_address::base_max_string_len && "literal string is too long");
        wchar_t str[ipv4_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_address::parse(str);
    }

    /**
     * User-defined literal for creating an ipv4_address from a UTF-16 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(const char16_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_address::base_max_string_len && "literal string is too long");
        char16_t str[ipv4_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_address::parse(str);
    }

    /**
     * User-defined literal for creating an ipv4_address from a UTF-32 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(const char32_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_address::base_max_string_len && "literal string is too long");
        char32_t str[ipv4_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_address::parse(str);
    }

    /**
     * User-defined literal for creating an ipv4_address from an unsigned long long integer.
     * 
     * @param[in] value An unsigned long long integer representing the IPv4 address in host byte order.
     * @return An ipv4_address object created from the integer.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_address operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT {
        assert(value <= ipv4_address::base_all_ones && "literal integer is too long");
        return ipv4_address::from_uint(uint32_t(value));
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_ADDRESS_HPP
