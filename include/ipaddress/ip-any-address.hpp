/**
 * @file      ip-any-address.hpp
 * @brief     Unified interface for handling both IPv4 and IPv6 addresses
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header defines the ip_address class, which provides a seamless and 
 * efficient way to handle IP addresses, supporting both IPv4 and IPv6 versions.
 * It encapsulates the complexities of dealing with different IP versions and 
 * offers a simple and consistent API to work with. The class internally manages 
 * the storage and representation of the IP addresses, abstracting away the 
 * underlying details from the user.
 * 
 * The ip_address class is designed to be a versatile and robust solution for IP address 
 * manipulation in network-related applications. It integrates the ipv4_address and 
 * ipv6_address classes through a union, ensuring optimal space usage while maintaining 
 * the ability to represent both IP address versions. This file is part of a larger library 
 * that aims to provide comprehensive support for network operations.
 */

#ifndef IPADDRESS_IP_ANY_ADDRESS_HPP
#define IPADDRESS_IP_ANY_ADDRESS_HPP

#include "ipv6-address.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A class that represents an IP address, supporting both IPv4 and IPv6 formats.
 * 
 * The ip_address class is a union-type structure that encapsulates both ipv4_address 
 * and ipv6_address, allowing for the representation and manipulation of both IPv4 
 * and IPv6 addresses. It provides a set of functions and type aliases that facilitate 
 * the handling of IP addresses in a network context, making it a versatile tool for 
 * developers working with IP-based communications.
 *
 * The class ensures that the size of its instances will always be large enough to 
 * store an IPv6 address, which is the larger of the two address types. This design 
 * guarantees that an ip_address object can store any valid IP address, regardless 
 * of its version, without the need for separate storage mechanisms.
 */
class ip_address {
public:
    using base_type_ipv4 = typename ipv4_address::base_type; /**< Base type for IPv4 address storage. */
    using base_type_ipv6 = typename ipv6_address::base_type; /**< Base type for IPv6 address storage. */

    using uint_type_ipv4 = typename ipv4_address::uint_type; /**< Unsigned integer type for IPv4 address representation. */
    using uint_type_ipv6 = typename ipv6_address::uint_type; /**< Unsigned integer type for IPv6 address representation. */

    /**
     * Retrieves the version of the IP address.
     * 
     * @return The version of the IP address, either IPv4 or IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return _version;
    }

    /**
     * Checks if the IP address is a multicast address.
     * 
     * @return `true` if the IP address is reserved for multicast use, `false` otherwise.
     * @see [RFC 3171](https://datatracker.ietf.org/doc/html/rfc3171.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_multicast() : _ipv.ipv6.is_multicast();
    }

    /**
     * Checks if the IP address is a private address.
     * 
     * @return `true` if the IP address is allocated for private networks, `false` otherwise.
     * @see [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_private() : _ipv.ipv6.is_private();
    }

    /**
     * Checks if the IP address is a global address.
     * 
     * @return `true` if the IP address is allocated for public networks, `false` otherwise.
     * @see [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_global() : _ipv.ipv6.is_global();
    }

    /**
     * Checks if the IP address is a reserved address.
     * 
     * @return `true` if the IP address is otherwise IETF reserved, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_reserved() : _ipv.ipv6.is_reserved();
    }

    /**
     * Checks if the IP address is a loopback address.
     * 
     * @return `true` if the IP address is a loopback address, `false` otherwise.
     * @see [RFC 3330](https://datatracker.ietf.org/doc/html/rfc3330.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_loopback() : _ipv.ipv6.is_loopback();
    }

    /**
     * Checks if the IP address is link-local.
     * 
     * @return True if the IP address is link-local, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_link_local() : _ipv.ipv6.is_link_local();
    }

    /**
     * Checks if the IP address is unspecified.
     * 
     * An unspecified IP address is an address with all bits set to zero.
     * In IPv4, this is represented as 0.0.0.0, and in IPv6, as ::.
     * This type of address is used to indicate the absence of an address.
     * 
     * @return `true` if the IP address is unspecified (all bits are zero), `false` otherwise.
     * @see [RFC 5735](https://datatracker.ietf.org/doc/html/rfc5735.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_unspecified() : _ipv.ipv6.is_unspecified();
    }

    /**
     * Checks if the IPv6 address is a site-local address.
     * 
     * @return `true` if the address is site-local, `false` otherwise.
     * @note Site-local addresses are equivalent to private addresses in IPv4 and are not routable on the global internet.
     * @remark These attribute is true for the network as a whole if it is true for both the network address and the broadcast address.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? false : _ipv.ipv6.is_site_local();
    }

    /**
     * Checks if the IP address is an IPv4 address.
     * 
     * @return `true` if the IP address is an IPv4 address, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    /**
     * Checks if the IP address is an IPv6 address.
     * 
     * @return `true` if the IP address is an IPv6 address, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    /**
     * Retrieves the size of the IPv4 address.
     * 
     * Depending on the IP version, this function returns the size of the IPv4 or IPv6 address.
     * 
     * @return The size of the IP address in bytes.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.size() : _ipv.ipv6.size();
    }

    /**
     * Computes a hash value for the IP address.
     * 
     * This function generates a hash value that can be used to uniquely identify the IP address.
     * It can be useful when IP addresses are used as keys in hash tables.
     * 
     * @return A `size_t` hash value of the IP address.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.hash() : _ipv.ipv6.hash();
    }

    /**
     * Determines if the IPv6 address is an IPv4-mapped address.
     * 
     * If the IP address is an IPv6 address, this function returns the IPv4-mapped IPv6 address if available.
     * 
     * @return An `optional` containing the mapped IPv4 address if the IPv6 address is IPv4-mapped, or an empty `optional` otherwise.
     * @remark An IPv4-mapped IPv6 address has its first 80 bits set to zero and the next 16 bits set to one (starting with `::FFFF/96`).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> ipv4_mapped() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<ipv4_address>() : _ipv.ipv6.ipv4_mapped();
    }

    /**
     * Determines if the IPv6 address is a 6to4 address.
     * 
     * If the IP address is an IPv6 address, this function returns the 6to4 address if available.
     * 
     * @return An optional containing the encapsulated IPv4 address if the IPv6 address is a 6to4 address, or an empty optional otherwise.
     * @remark A 6to4 address uses a `2002::/16` prefix and embeds an IPv4 address in the next 32 bits.
     * @see [RFC 3056](https://datatracker.ietf.org/doc/html/rfc3056.html).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> sixtofour() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<ipv4_address>() : _ipv.ipv6.sixtofour();
    }

    /**
     * Determines if the IPv6 address is a Teredo address.
     * 
     * If the IP address is an IPv6 address, this function returns the Teredo address, which includes
     * both the Teredo server and client IPv4 addresses.
     * 
     * @return An optional containing a pair of IPv4 addresses representing the Teredo server and client if the IPv6 address is a Teredo address, or an empty optional otherwise.
     * @retval std::pair::first The Teredo server IPv4 address
     * @retval std::pair::second The Teredo client IPv4 address
     * @remark A Teredo address begins with the `2001::/32` prefix and is used for NAT traversal for IPv6.
     * @see [RFC 4380](https://datatracker.ietf.org/doc/html/rfc4380.html).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<std::pair<ipv4_address, ipv4_address>> teredo() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<std::pair<ipv4_address, ipv4_address>>() : _ipv.ipv6.teredo();
    }

    /**
     * Retrieves the IPv4 address.
     * 
     * If the IP address is an IPv4 address, this function returns the IPv4 address.
     * 
     * @return An optional containing the IPv4 address, or an empty optional if the IP address is not IPv4.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> v4() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            return optional<ipv4_address>();
        }
        auto ip = _ipv.ipv4;
        return optional<ipv4_address>(std::move(ip));
    }

    /**
     * Retrieves the IPv6 address.
     * 
     * If the IP address is an IPv6 address, this function returns the IPv6 address.
     * 
     * @return An optional containing the IPv6 address, or an empty optional if the IP address is not IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv6_address> v6() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            return optional<ipv6_address>();
        }
        auto ip = _ipv.ipv6;
        return optional<ipv6_address>(std::move(ip));
    }

    /**
     * Retrieves the raw data of the IP address in **network byte order** (big-endian).
     * 
     * This function returns a pointer to the raw data representing the IP address. The format of the data
     * depends on whether the address is IPv4 or IPv6.
     * 
     * @return A pointer to the raw data of the IP address.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const uint8_t* data() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.data() : _ipv.ipv6.data();
    }

    /**
     * Default constructor.
     * 
     * Constructs an ip_address object with an unspecified ip address with version IPv4.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address() IPADDRESS_NOEXCEPT {
    }

    /**
     * Constructor from an ipv4_address.
     * 
     * Constructs an ip_address object with the specified IPv4 address.
     * 
     * @param[in] ipv4 An ipv4_address object to initialize the ip_address.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address(const ipv4_address& ipv4) IPADDRESS_NOEXCEPT : _ipv(ipv4), _version(ip_version::V4) {
    }

    /**
     * Constructor from an ipv6_address.
     * 
     * Constructs an ip_address object with the specified IPv6 address.
     * 
     * @param[in] ipv6 An ipv6_address object to initialize the ip_address.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address(const ipv6_address& ipv6) IPADDRESS_NOEXCEPT : _ipv(ipv6), _version(ip_version::V6) {
    }

    /**
     * Constructor from byte array for IPv4.
     * 
     * Constructs an ip_address object with the specified byte array for an IPv4 address.
     * 
     * @param[in] bytes A base_type_ipv4 object representing the byte array of the IPv4 address.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT : _ipv(bytes), _version(ip_version::V4) {
    }

    /**
     * Constructor from byte array for IPv6.
     * 
     * Constructs an ip_address object with the specified byte array for an IPv6 address.
     * 
     * @param[in] bytes A base_type_ipv6 object representing the byte array of the IPv6 address.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT : _ipv(bytes), _version(ip_version::V6) {
    }

    /**
     * Static factory method to create an ip_address from byte array for IPv4.
     * 
     * @param[in] bytes A base_type_ipv4 object representing the byte array of the IPv4 address.
     * @return An ip_address object initialized with the provided IPv4 bytes.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT {
        return ip_address(bytes);
    }

    /**
     * Static factory method to create an ip_address from byte array for IPv6.
     * 
     * @param[in] bytes A base_type_ipv6 object representing the byte array of the IPv6 address.
     * @return An ip_address object initialized with the provided IPv6 bytes.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT {
        return ip_address(bytes);
    }

    /**
     * Static factory method to create an ip_address from raw bytes.
     * 
     * This method creates an ip_address object from raw bytes, with the specified IP version.
     * 
     * @param[in] bytes A pointer to the raw bytes representing the IP address.
     * @param[in] byte_count The number of bytes to use from the pointer.
     * @param[in] version The version of the IP address to create (IPv4 or IPv6).
     * @return An ip_address object initialized with the provided bytes and version.
     * @parblock
     * @remark If the number of bytes \a byte_count is less than the target number .
     *         of bytes to represent the IP address, the missing bytes will be 
     *         filled with zeros.
     * @endparblock
     * @parblock
     * @remark If the number of bytes \a byte_count is greater than the target .
     *         number of bytes represented by the IP address, then the extra 
     *         bytes will be ignored
     * @endparblock
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const uint8_t* bytes, size_t byte_count, ip_version version) IPADDRESS_NOEXCEPT {
        return version == ip_version::V4 ? ip_address(ipv4_address::from_bytes(bytes, byte_count)) : ip_address(ipv6_address::from_bytes(bytes, byte_count));
    }

    /**
     * Static factory method to create an ip_address from a uint_type_ipv4.
     * 
     * @param[in] ip A uint_type_ipv4 (uint32_t) representing the IPv4 address.
     * @return An ip_address object initialized with the provided IPv4 address.
     * @remark Bytes in integer \a ip must be presented in **host byte order**.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_uint(uint_type_ipv4 ip) IPADDRESS_NOEXCEPT {
        return ip_address(ipv4_address::from_uint(ip));
    }

    /**
     * Static factory method to create an ip_address from a uint_type_ipv6.
     * 
     * @param[in] ip A uint_type_ipv6 (uint128_t) representing the IPv6 address.
     * @return An ip_address object initialized with the provided IPv6 address.
     * @remark Bytes in integer \a ip must be presented in **host byte order**.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_uint(const uint_type_ipv6& ip) IPADDRESS_NOEXCEPT {
        return ip_address(ipv6_address::from_uint(ip));
    }
    /**
     * Converts the IP address to a uint128_t.
     * 
     * This function returns the numeric representation of the IP address as a uint128_t. For IPv4 addresses,
     * the function converts the address to a uint128_t format.
     * 
     * @return A uint128_t representing the numeric value of the IP address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t to_uint() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? uint128_t(_ipv.ipv4.to_uint()) : _ipv.ipv6.to_uint();
    }

    /**
     * Converts the IP address to a string.
     * 
     * This function returns a string representation of the IP address. The format can be specified by the \a fmt parameter.
     * 
     * @param[in] fmt The format to use for the string representation, defaults to compressed format.
     * @return A `std::string` representing the IP address in the specified format.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return _version == ip_version::V4 ? _ipv.ipv4.to_string(fmt) : _ipv.ipv6.to_string(fmt);
    }

    /**
     * Generates a reverse DNS lookup pointer for the IP address.
     * 
     * This function creates a string that is the reverse DNS lookup pointer of the IP address.
     * It is commonly used in reverse DNS lookups, where the IP address is reversed and appended with `.in-addr.arpa` for IPv4,
     * or `.ip6.arpa` for IPv6, to form a domain name that can be looked up in the DNS system.
     * 
     * The name of the reverse DNS PTR record for the IP address, e.g.:
     * @code{.cpp}
     *   std::cout << ip_address::parse("127.0.0.1").reverse_pointer() << std::endl;
     *   std::cout << ip_address::parse("2001:db8::1").reverse_pointer() << std::endl;
     * 
     *   // out:
     *   // 1.0.0.127.in-addr.arpa
     *   // 1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa
     * @endcode
     * @return A `std::string` that is the reverse DNS lookup pointer of the IP address.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string reverse_pointer() const {
       return _version == ip_version::V4 ? _ipv.ipv4.reverse_pointer() : _ipv.ipv6.reverse_pointer();
    }

    /**
     * Swaps the contents of this ip_address with another ip_address.
     * 
     * This function exchanges the contents of the ip_address with those of the ip parameter.
     * 
     * @param[in] ip The other ip_address object to swap with.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_address& ip) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = ip;
        ip = tmp;
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * Static method template to parse an IP address from a fixed string at compile time.
     * 
     * @tparam FixedString A non-type template parameter that holds the string representation of the IP address.
     * @return An instance of ip address parsed from the fixed string.
     * @note This method is only available when non-type template parameters for strings are supported.
     * @remark If parsing fails, an error will be raised at compile time.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval IPADDRESS_FORCE_INLINE ip_address parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;

        char ip[str.size() + 1]{};
        for (size_t i = 0; i < str.size(); ++i) {
            ip[i] = str[i];
        }

        const auto ipv4 = ipv4_address::parse(ip, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        return ip_address(ipv6_address::parse<FixedString>());
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    /**
     * Parses an IP address from a string view.
     * 
     * This method provides a way to parse an IP address from a string view, 
     * which is a non-owning reference to a sequence of characters.
     * 
     * @param[in] address The string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a wide string view.
     * 
     * This method allows for parsing an IP address from a wide string view, 
     * which is typically used for wide character strings.
     * 
     * @param[in] address The wide string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::wstring_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses an IP address from a UTF-8 string view.
     * 
     * This method parses an IP address from a given UTF-8 string view. It leverages the `char8_t`
     * type introduced in C++20 to handle UTF-8 strings natively.
     * 
     * @param[in] address A UTF-8 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u8string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

#endif // __cpp_char8_t

    /**
     * Parses an IP address from a UTF-16 string view.
     * 
     * This method is designed to parse an IP address from a UTF-16 string view, 
     * which is a sequence of 16-bit characters.
     * 
     * @param[in] address The UTF-16 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u16string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-32 string view.
     * 
     * This method allows for parsing an IP address from a UTF-32 string view, 
     * which is a sequence of 32-bit characters.
     * 
     * @param[in] address The UTF-32 string view containing the IP address to parse.
     * @return An instance of ip address parsed from the string view.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u32string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a string view and reports errors through an error code.
     * 
     * This method parses an IP address from a string view and provides an error code if the parsing fails.
     * 
     * @param[in] address The string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the string view. If parsing fails, the returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a wide string view and reports errors through an error code.
     * 
     * This method parses an IP address from a wide string view and provides an error code if the parsing fails.
     * 
     * @param[in] address The wide string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the wide string view. If parsing fails, the returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::wstring_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses an IP address from a UTF-8 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-8 string view and provides an error code if the parsing fails.
     * 
     * @param[in] address The UTF-8 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the UTF-8 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u8string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // __cpp_char8_t

    /**
     * Parses an IP address from a UTF-16 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-16 string view and provides an error code if the parsing fails.
     * 
     * @param[in] address The UTF-16 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the UTF-16 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u16string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-32 string view and reports errors through an error code.
     * 
     * This method parses an IP address from a UTF-32 string view and provides an error code if the parsing fails.
     * 
     * @param[in] address The UTF-32 string view containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the UTF-32 string view. If parsing fails, the returned object will be in an unspecified state.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u32string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Sets the scope identifier of the IPv6 address using a string view.
     * 
     * This function sets the scope identifier using a string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a wide string view.
     * 
     * This function sets the scope identifier using a wide string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The wide string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

#if __cpp_char8_t >= 201811L

    /**
     * Sets the scope identifier of the IPv6 address using UTF-8 string view.
     * 
     * This function sets the scope identifier using UTF-8 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-8 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

#endif // __cpp_char8_t

    /**
     * Sets the scope identifier of the IPv6 address using UTF-16 string view.
     * 
     * This function sets the scope identifier using UTF-16 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-16 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using UTF-32 string view.
     * 
     * This function sets the scope identifier using UTF-32 string view, which allows for a more efficient
     * way to handle strings without copying them.
     * 
     * @param[in] scope_id The UTF-32 string view representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a wide string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a wide string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The wide string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#if __cpp_char8_t >= 201811L

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-8 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-8 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-8 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++20 and later versions.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#endif // __cpp_char8_t

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-16 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-16 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-16 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a UTF-32 string view and reports any errors encountered.
     * 
     * This function sets the scope identifier using a UTF-32 string view and provides an error code parameter to report any issues that occur during the operation.
     * 
     * @param[in] scope_id The UTF-32 string view representing the scope identifier.
     * @param[out] code An error_code object that will be set to the error that occurred, if any.
     * @note This method is available for C++17 and later versions.
     * @parblock
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     * @endparblock
     * @parblock
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     * @endparblock
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#else // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses an IP address from a `std::string`.
     * 
     * @param[in] address The `std::string` containing the IP address to parse.
     * @return An instance of ip address parsed from the string.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a wide `std::wstring`.
     * 
     * @param[in] address The wide `std::wstring` containing the IP address to parse.
     * @return An instance of ip address parsed from the string.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::wstring& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-16 `std::u16string`.
     * 
     * @param[in] address The UTF-16 `std::u16string` containing the IP address to parse.
     * @return An instance of ip address parsed from the string.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::u16string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a UTF-32 `std::u32string`.
     * 
     * @param[in] address The UTF-32 `std::u32string` containing the IP address to parse.
     * @return An instance of ip address parsed from the string.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::u32string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    /**
     * Parses an IP address from a `std::string` and reports errors through an error code.
     * 
     * @param[in] address The `std::string` containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a wide `std::wstring` and reports errors through an error code.
     * 
     * @param[in] address The wide `std::wstring` containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the wide string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address parse(const std::wstring& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-16 `std::u16string` and reports errors through an error code.
     * 
     * @param[in] address The UTF-16 `std::u16string` containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the UTF-16 string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address parse(const std::u16string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Parses an IP address from a UTF-32 `std::u32string` and reports errors through an error code.
     * 
     * @param[in] address The UTF-32 `std::u32string` containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the UTF-32 string. If parsing fails, the returned object will be in an unspecified state.
     */
    static IPADDRESS_FORCE_INLINE ip_address parse(const std::u32string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::string`.
     * 
     * @param[in] scope_id The string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::wstring`.
     * 
     * @param[in] scope_id The wide string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input wide string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u16string`.
     * 
     * @param[in] scope_id The UTF-16 string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input UTF-16 string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u32string`.
     * 
     * @param[in] scope_id The UTF-32 string representing the scope identifier.
     * @throw parse_error Exception caused by invalid input UTF-32 string.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::string` and reports any errors.
     * 
     * @param[in] scope_id The string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::wstring` and reports any errors.
     * 
     * @param[in] scope_id The wide string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u16string` and reports any errors.
     * 
     * @param[in] scope_id The UTF-16 string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    /**
     * Sets the scope identifier of the IPv6 address using a `std::u32string` and reports any errors.
     * 
     * @param[in] scope_id The UTF-32 string representing the scope identifier.
     * @param[out] code An error_code object that will store the result of the operation.
     */
    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#endif // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses an IP address from a character array.
     * 
     * This method template parses an IP address from a character array of a 
     * specified size. Can check and get the result at compile time.
     * 
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array containing the IP address to parse.
     * @return An instance of ip address parsed from the character array.
     */
    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(const T(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        internal::is_char_type<T>();
        auto code = error_code::NO_ERROR;
        auto result = parse_string(address, code);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, 0, address, N);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, 0, address, N);
        #endif
        }
        return result;
    }

    /**
     * Parses an IP address from a character array and reports errors through an error code.
     * 
     * This method template parses an IP address from a character array of a specified size 
     * and provides an error code if the parsing fails.
     * 
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array containing the IP address to parse.
     * @param[out] code A reference to an `error_code` object that will be set if an error occurs during parsing.
     * @return An instance of ip address parsed from the character array. If parsing fails, the returned object will be in an unspecified state.
     */
    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(const T(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        code = error_code::NO_ERROR;
        
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        const auto ipv6 = ipv6_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv6);
        }
        
        return ip_address();
    }

    /**
     * Sets the scope identifier of the IPv6 address.
     * 
     * This function sets the scope identifier using a character array. The length of the array
     * should not exceed `IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1`.
     * 
     * @tparam T The character type of the scope identifier.
     * @tparam N The size of the scope identifier array.
     * @param[in] scope_id The character array representing the scope identifier.
     * @remark If scope is disabled in settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`) then this call will have no effect.
     */
    template <typename T, size_t N>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void set_scope_id(const T(&scope_id)[N]) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    /**
     * Retrieves the scope identifier of the IPv6 address.
     * 
     * The scope identifier is used to determine the context in which the address is valid.
     * It is particularly relevant for link-local and site-local addresses.
     * 
     * @return A `scope` object representing the scope identifier of the IPv6 address.
     * @remark If the scope is disabled in the settings (`IPADDRESS_IPV6_SCOPE_MAX_LENGTH == 0`), then an empty scope will be returned.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope get_scope_id() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            return fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>();
        }
        return _ipv.ipv6.get_scope_id();
    }
    
    /**
     * Converts the IP address to a uint32_t.
     * 
     * This function casts the IP address to a uint32_t. For IPv4 addresses, it returns the direct numeric representation.
     * For IPv6 addresses, it returns the least significant 32 bits of the address.
     * 
     * @return A uint32_t representing the IPv4 address or the least significant 32 bits of the IPv6 address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator uint32_t() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.to_uint() : uint32_t(_ipv.ipv6.to_uint());
    }

    /**
     * Converts the IP address to a uint128_t.
     * 
     * This function casts the IP address to a uint128_t. It is used to obtain the full numeric representation of the IP address,
     * especially for IPv6 addresses.
     * 
     * @return A uint128_t representing the numeric value of the IP address.
     * @remark Bytes in integer are presented in **host byte order**.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator uint128_t() const IPADDRESS_NOEXCEPT {
        return to_uint();
    }

    /**
     * Converts the IP address to a string.
     * 
     * This operator allows the IP address to be converted to a string.
     * 
     * @return A `std::string` representation of the IP address.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return _version == ip_version::V4 ? _ipv.ipv4.to_string() : _ipv.ipv6.to_string();
    }

    /**
     * Equality comparison operator.
     * 
     * Compares this IP address with another IP address for equality.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if both IP addresses are equal, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (_version != rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv.ipv4 == rhs._ipv.ipv4) : (_ipv.ipv6 == rhs._ipv.ipv6);
    }

    /**
     * Inequality comparison operator.
     * 
     * Compares this IP address with another IP address for inequality.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if both IP addresses are not equal, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares this IP address with another IP address to determine the ordering.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return `std::strong_ordering` indicating less than, equivalent to, or greater than.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> rhs._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? (_ipv.ipv4 <=> rhs._ipv.ipv4) : (_ipv.ipv6 <=> rhs._ipv.ipv6);
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less than comparison operator.
     * 
     * Compares this IP address with another IP address to determine if it is less than the other.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if this IP address is less than the other, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (_version < rhs._version) {
            return true;
        }
        if (_version > rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv.ipv4 < rhs._ipv.ipv4) : (_ipv.ipv6 < rhs._ipv.ipv6);
    }
    
    /**
     * Greater than comparison operator.
     * 
     * Compares this IP address with another IP address to determine if it is greater than the other.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if this IP address is greater than the other, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    /**
     * Less than or equal to comparison operator.
     * 
     * Compares this IP address with another IP address to determine if it is less than or equal to the other.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if this IP address is less than or equal to the other, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    /**
     * Greater than or equal to comparison operator.
     * 
     * Compares this IP address with another IP address to determine if it is greater than or equal to the other.
     * 
     * @param[in] rhs The right-hand side ip_address object for comparison.
     * @return True if this IP address is greater than or equal to the other, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        return ip_address(ipv6_address::parse(address));
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        const auto ipv6 = ipv6_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv6);
        }
        
        return ip_address();
    }

    union ip_any_address {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address() IPADDRESS_NOEXCEPT : ipv4() {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address(const ipv4_address& ip) IPADDRESS_NOEXCEPT : ipv4(ip) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address(const ipv6_address& ip) IPADDRESS_NOEXCEPT : ipv6(ip) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_any_address(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT : ipv4(bytes) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_any_address(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT : ipv6(bytes) {
        }

        ipv4_address ipv4;
        ipv6_address ipv6;
    } _ipv {};
    ip_version _version = ip_version::V4;
};

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ip_address from a fixed string at compile time.
     * 
     * @tparam FixedString A compile-time fixed string representing the IPv6 address.
     * @return An ip_address object parsed from the fixed string.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ip_address operator""_ip() IPADDRESS_NOEXCEPT {
        return ip_address::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal for creating an ip_address from a string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ip_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address operator""_ip(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::base_max_string_len && "literal string is too long");
        char str[ipv6_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_address::parse(str);
    }

    /**
     * User-defined literal for creating an ip_address from a wide string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ip_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address operator""_ip(const wchar_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::base_max_string_len && "literal string is too long");
        wchar_t str[ipv6_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_address::parse(str);
    }

    /**
     * User-defined literal for creating an ip_address from a UTF-16 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ip_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address operator""_ip(const char16_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::base_max_string_len && "literal string is too long");
        char16_t str[ipv6_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_address::parse(str);
    }

    /**
     * User-defined literal for creating an ip_address from a UTF-32 string literal.
     * 
     * @param[in] address A pointer to a character array representing the IPv6 address.
     * @param[in] size The size of the character array.
     * @return An ip_address object parsed from the string literal.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address operator""_ip(const char32_t* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::base_max_string_len && "literal string is too long");
        char32_t str[ipv6_address::base_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_address::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <>
struct hash<IPADDRESS_NAMESPACE::ip_address> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::ip_address& ip) const IPADDRESS_NOEXCEPT {
        return ip.hash();
    }
};

IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_address& ip1, IPADDRESS_NAMESPACE::ip_address& ip2) IPADDRESS_NOEXCEPT {
    ip1.swap(ip2);
}

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_address& ip) {
    return ip.to_string();
}

IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_address& ip) {
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

IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_address& ip) {
    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    ip = IPADDRESS_NAMESPACE::ip_address::parse(str, err);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ANY_ADDRESS_HPP
