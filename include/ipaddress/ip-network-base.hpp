/**
 * @file      ip-network-base.hpp
 * @brief     Defines the base class for IP network representation.
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file contains the declaration of the ip_network_base class, which serves as
 * a foundation for creating representations of IP networks. The class employs a template
 * approach, allowing it to work with various types of IP addresses, such as IPv4 and IPv6.
 */

#ifndef IPADDRESS_IP_NETWORK_BASE_HPP
#define IPADDRESS_IP_NETWORK_BASE_HPP

#include "ip-address-iterator.hpp"
#include "ip-network-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Template base class for representing a network of IP addresses.
 *
 * This class provides the foundational elements for representing a network of IP addresses,
 * offering methods to iterate over them and perform common network-related operations.
 * It is designed to be flexible and work with any IP address type that conforms to a
 * specific interface, making it suitable for both IPv4 and IPv6 networks.
 * 
 * @tparam Base The base class from which ip_network_base inherits. Should be a type that
 *              represents an individual IP address.
 */
template <typename Base>
class ip_network_base : public Base {
public:
    using ip_address_type = typename Base::ip_address_type; /**< The IP address type used by the network. */
    using uint_type = typename ip_address_type::uint_type; /**< Unsigned integer type used for the underlying IP address representation. */

    /**
     * Constructs a new IP network base object.
     * 
     * Initializes a new instance of the ip_network_base class with the maximum prefix length
     * for the IP address type, effectively setting the network address to the default address
     * and the netmask to the maximum value.
     */
    IPADDRESS_CONSTEXPR ip_network_base() IPADDRESS_NOEXCEPT 
        : 
        _network_address(),
        _netmask(ip_address_type::ip_from_prefix(Base::base_max_prefixlen)),
        _prefixlen(Base::base_max_prefixlen) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * Parses a network address and prefix from a fixed string at compile time.
     * 
     * This template function allows for the compile-time parsing of an IP network address
     * and prefix length from a fixed string. It is enabled only when non-type template parameters
     * are supported by the compiler.
     * 
     * @tparam FixedString A string literal representing the IP network in "address/prefix" format.
     * @tparam Strict A boolean flag indicating whether to perform strict validation of the address.
     * @return A consteval ip network object representing the parsed network.
     */
    template <fixed_string FixedString, bool Strict = true>
    IPADDRESS_NODISCARD static consteval ip_network_base parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = parse_address_with_prefix(str, Strict, code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return result;
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    /**
     * Parses a network address and prefix from a string view.
     * 
     * This function parses an IP network address and prefix length from a string view.
     * It is available when the compiler supports C++17 or later.
     * 
     * @param[in] address A string view representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a wide string view.
     * 
     * This function parses an IP network address and prefix length from a wide string view.
     * It is available when the compiler supports C++17 or later.
     * 
     * @param[in] address The wide string view representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::wstring_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses a network address and prefix from UTF-8 string view.
     * 
     * This function parses an IP network address and prefix length from UTF-8 string view. 
     * It leverages the `char8_t` type introduced in C++20 to handle UTF-8 strings natively.
     * 
     * @param[in] address A UTF-8 string view representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::u8string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

#endif // __cpp_char8_t

    /**
     * Parses a network address and prefix from UTF-16 string view.
     * 
     * This function parses an IP network address and prefix length from UTF-16 string view.
     * It is available when the compiler supports C++17 or later.
     * 
     * @param[in] address The UTF-16 string view representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::u16string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from UTF-32 string view.
     * 
     * This function parses an IP network address and prefix length from UTF-32 string view.
     * It is available when the compiler supports C++17 or later.
     * 
     * @param[in] address The UTF-32 string view representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::u32string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a string view with error handling.
     * 
     * This function parses an IP network address and prefix length from a string view,
     * providing an error code for exception-free error handling.
     * 
     * @param[in] address The string view representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR ip_network_base parse(std::string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

    /**
     * Parses a network address and prefix from a wide string view with error handling.
     * 
     * This function parses an IP network address and prefix length from a wide string view,
     * providing an error code for exception-free error handling.
     * 
     * @param[in] address The wide string view representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::wstring` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR ip_network_base parse(std::wstring_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#if __cpp_char8_t >= 201811L

    /**
     * Parses a network address and prefix from UTF-8 string view with error handling.
     * 
     * This function parses an IP network address and prefix length from UTF-8 string view,
     * providing an error code for exception-free error handling.
     * 
     * @param[in] address The UTF-8 string view representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @note This method is available for C++20 and later versions where `char8_t` is supported.
     */
    static IPADDRESS_CONSTEXPR ip_network_base parse(std::u8string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#endif // __cpp_char8_t

    /**
     * Parses a network address and prefix from UTF-16 string view with error handling.
     * 
     * This function parses an IP network address and prefix length from UTF-16 string view,
     * providing an error code for exception-free error handling.
     * 
     * @param[in] address The UTF-16 string view representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u16string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR ip_network_base parse(std::u16string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

    /**
     * Parses a network address and prefix from UTF-32 string view with error handling.
     * 
     * This function parses an IP network address and prefix length from UTF-32 string view,
     * providing an error code for exception-free error handling.
     * 
     * @param[in] address The UTF-32 string view representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @note This method is available for C++17 and later versions.
     * @remark For C++ versions prior to C++17, member functions with `std::u32string` and C-strings will be used instead.
     */
    static IPADDRESS_CONSTEXPR ip_network_base parse(std::u32string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#else // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses a network address and prefix from a `std::string`.
     * 
     * @param[in] address The `std::string` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_network_base parse(const std::string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::wstring`.
     * 
     * @param[in] address The `std::wstring` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_network_base parse(const std::wstring& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u16string`.
     * 
     * @param[in] address The `std::u16string` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_network_base parse(const std::u16string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u32string`.
     * 
     * @param[in] address The `std::u32string` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_network_base parse(const std::u32string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::string` and reports errors through an error code.
     * 
     * @param[in] address The `std::string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static ip_network_base parse(const std::string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

    /**
     * Parses a network address and prefix from a `std::wstring` and reports errors through an error code.
     * 
     * @param[in] address The `std::wstring` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static ip_network_base parse(const std::wstring& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

    /**
     * Parses a network address and prefix from a `std::u16string` and reports errors through an error code.
     * 
     * @param[in] address The `std::u16string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static ip_network_base parse(const std::u16string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

    /**
     * Parses a network address and prefix from a `std::u32string` and reports errors through an error code.
     * 
     * @param[in] address The `std::u32string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static ip_network_base parse(const std::u32string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base parse(const T(&address)[N], bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        return parse_address_with_prefix(str, strict);
    }

    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base parse(const T(&address)[N], error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        auto index = 0;
        return parse_address_with_prefix(str, strict, code, index);
    }

    /**
     * Creates an ip network object from a given IP address and prefix length.
     * 
     * This static method constructs an ip network object representing the network
     * that includes the given IP address, with the specified prefix length. If 'strict' is true,
     * the address is validated against the netmask.
     * 
     * @param[in] address The IP address to use for creating the network.
     * @param[in] prefixlen The prefix length for the network's netmask. *Defaults to the maximum prefix length*.
     * @param[in] strict Whether to validate the address against the netmask.
     * @return An ip network object representing the network.
     * @throw parse_error Exception caused by invalid input string.
     * @remark May throw an exception if the address does not conform to the netmask when \a strict is `true`.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base from_address(
        const ip_address_type& address, 
        size_t prefixlen = ip_address_type::max_prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto result = from_address(address, code, prefixlen, strict);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                char str[ip_address_type::base_max_string_len + 1]{};
                const auto len = address.ip_to_chars(address.bytes(), format::compressed, str);
                raise_error(code, 0, str, len);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            char str[ip_address_type::base_max_string_len + 1]{};
            const auto len = address.ip_to_chars(address.bytes(), format::compressed, str);
            raise_error(code, 0, str, len);
        #endif
        }
        return result;
    }

    /**
     * Creates an ip network object from a given IP address and prefix length, with error handling.
     * 
     * Similar to the from_address method above, but this version allows for error handling without exceptions.
     * It populates the provided error_code object instead of throwing.
     * 
     * @param[in] address The IP address to use for creating the network.
     * @param[in] code An error_code object that will be set if an error occurs.
     * @param[in] prefixlen The prefix length for the network's netmask. Defaults to the maximum prefix length.
     * @param[in] strict Whether to validate the address against the netmask.
     * @return An ip network object representing the network, or an object in an error state if an error occurs.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base from_address(
        const ip_address_type& address, 
        error_code& code, 
        size_t prefixlen = ip_address_type::max_prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;

        const auto netmask = ip_address_type::ip_from_prefix(prefixlen); 

        ip_network_base result;
        result._network_address = ip_address_type::strict_netmask(address, netmask, strict, code);

        if (code != error_code::NO_ERROR) {
            return result;
        }

        result._netmask = netmask;
        result._prefixlen = prefixlen;
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool contains(const ip_address_type& address) const IPADDRESS_NOEXCEPT {
        const auto& ip_bytes = address.bytes();
        const auto& address_bytes = network_address().bytes();
        const auto& netmask_bytes = netmask().bytes();

        for (size_t i = 0; i < ip_address_type::base_size; ++i) {
            if ((ip_bytes[i] & netmask_bytes[i]) != address_bytes[i]) {
                return false;
            }
        }
        return true;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool overlaps(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return other.contains(network_address()) || other.contains(broadcast_address()) || contains(other.network_address()) || contains(other.broadcast_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool subnet_of(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return is_subnet_of(*this, other);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool supernet_of(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return is_subnet_of(other, *this);
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return _network_address.to_string(fmt) + '/' + std::to_string(_prefixlen);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(ip_network_base& network) IPADDRESS_NOEXCEPT {
        _network_address.swap(network._network_address);
        _netmask.swap(network._netmask);
        auto tmp = _prefixlen;
        _prefixlen = network._prefixlen;
        network._prefixlen = tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return calc_hash(_network_address.hash(), _netmask.hash());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const ip_address_type& network_address() const IPADDRESS_NOEXCEPT {
        return _network_address;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_type broadcast_address() const IPADDRESS_NOEXCEPT {
        typename ip_address_type::base_type bytes {};
        const auto& network_address_bytes = network_address().bytes();
        const auto hostmask_bytes = hostmask().bytes();
        for (size_t i = 0; i < ip_address_type::base_size; ++i) {
            bytes[i] = uint8_t(network_address_bytes[i] | hostmask_bytes[i]);
        }
        return ip_address_type(bytes);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const ip_address_type& netmask() const IPADDRESS_NOEXCEPT {
        return _netmask;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_type hostmask() const IPADDRESS_NOEXCEPT {
        const auto& netmask_bytes = netmask().bytes();
        typename ip_address_type::base_type bytes {};
        for (size_t i = 0; i < ip_address_type::base_size; ++i) {
            bytes[i] = uint8_t(netmask_bytes[i] ^ 0xFF);
        }
        return ip_address_type(bytes);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return _prefixlen;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return network_address().is_multicast() && broadcast_address().is_multicast();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return network_address().is_reserved() && broadcast_address().is_reserved();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return network_address().is_loopback() && broadcast_address().is_loopback();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return network_address().is_link_local() && broadcast_address().is_link_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return network_address().is_unspecified() && broadcast_address().is_unspecified();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type addresses_count() const IPADDRESS_NOEXCEPT {
        return broadcast_address().to_uint() - network_address().to_uint() + 1;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence<ip_address_type> hosts() const IPADDRESS_NOEXCEPT {
        return hosts_sequence<ip_address_type>(network_address(), broadcast_address(), prefixlen(), ip_address_type::base_max_prefixlen);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence<ip_network_base<Base>> address_exclude(const ip_network_base& other) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code code = error_code::NO_ERROR;
        const auto result = address_exclude(other, code);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, 0, "", 0);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, 0, "", 0);
        #endif
        }
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence<ip_network_base<Base>> address_exclude(const ip_network_base& other, error_code& code) const IPADDRESS_NOEXCEPT {
        auto lhs = Base::remove_scope_id(*this);
        auto rhs = Base::remove_scope_id(other);

        if (!rhs.subnet_of(lhs)) {
            code = error_code::NOT_CONTAINED_NETWORK;
            return exclude_network_sequence<ip_network_base<Base>>();
        }

        if (lhs == rhs) {
            return exclude_network_sequence<ip_network_base<Base>>();
        }

        return exclude_network_sequence<ip_network_base<Base>>(lhs, rhs);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence<ip_network_base<Base>> subnets(size_t prefixlen_diff = 1, optional<size_t> new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code code = error_code::NO_ERROR;
        const auto result = subnets(code, prefixlen_diff, new_prefixlen);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, 0, "", 0);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, 0, "", 0);
        #endif
        }
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_sequence<ip_network_base<Base>> subnets(error_code& code, size_t prefixlen_diff = 1, optional<size_t> new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT {
        auto address = Base::remove_scope_id(network_address());

        if (prefixlen() == ip_address_type::base_max_prefixlen) {
            return subnets_sequence<ip_network_base<Base>>(address, prefixlen());
        }

        if (new_prefixlen) {
            if (new_prefixlen.value() < prefixlen()) {
                code = error_code::NEW_PREFIX_MUST_BE_LONGER;
                return subnets_sequence<ip_network_base<Base>>();
            }
            if (prefixlen_diff != 1) {
                code = error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX;
                return subnets_sequence<ip_network_base<Base>>();
            }
            prefixlen_diff = new_prefixlen.value() - prefixlen();
        }

        auto new_prefix = prefixlen() + prefixlen_diff;

        if (new_prefix > ip_address_type::base_max_prefixlen) {
            code = error_code::INVALID_PREFIXLEN_DIFF;
            return subnets_sequence<ip_network_base<Base>>();
        }

        return subnets_sequence<ip_network_base<Base>>(address, broadcast_address(), hostmask(), prefixlen_diff, new_prefix);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<Base> supernet(size_t prefixlen_diff = 1, optional<size_t> new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code code = error_code::NO_ERROR;
        const auto result = supernet(code, prefixlen_diff, new_prefixlen);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, 0, "", 0);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, 0, "", 0);
        #endif
        }
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<Base> supernet(error_code& code, size_t prefixlen_diff = 1, optional<size_t> new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;

        if (prefixlen() == 0) {
            return *this;
        }

        if (new_prefixlen) {
            if (new_prefixlen.value() > prefixlen()) {
                code = error_code::NEW_PREFIX_MUST_BE_SHORTER;
                return ip_network_base<Base>();
            }
            if (prefixlen_diff != 1) {
                code = error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX;
                return ip_network_base<Base>();
            }
            prefixlen_diff = prefixlen() - new_prefixlen.value();
        }

        if (prefixlen_diff > prefixlen()) {
            code = error_code::INVALID_PREFIXLEN_DIFF;
            return ip_network_base<Base>();
        }

        const auto new_prefix = prefixlen() - prefixlen_diff;
        const auto address = ip_address_type::from_uint(network_address().to_uint() & (netmask().to_uint() << prefixlen_diff));
        return ip_network_base<Base>::from_address(address, new_prefix);
    }

    IPADDRESS_NODISCARD explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return _network_address == rhs._network_address && _netmask == rhs._netmask;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (auto result = _network_address <=> rhs._network_address; result != std::strong_ordering::equivalent) {
            return result;
        }
        return _netmask <=> rhs._netmask;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (_network_address != rhs._network_address) {
            return _network_address < rhs._network_address;
        }
        if (_netmask != rhs._netmask) {
            return _netmask < rhs._netmask;
        }
        return false;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base parse_address_with_prefix(const Str& str, bool strict) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = parse_address_with_prefix(str, strict, code, index);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, index, str.data(), str.size());
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, str.data(), str.size());
        #endif
        }
        return result;
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base parse_address_with_prefix(const Str& str, bool strict, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        
        auto has_slash = false;
        auto netmask = str.end();
        auto symbol = 0;
        char address[ip_address_type::base_max_string_len + 1] = {};
        for (auto it = str.begin(); it != str.end(); ++it) {
            const auto c = char(*it);
            if (c == '/') {
                if (has_slash) {
                    code = error_code::ONLY_ONE_SLASH_PERMITTED;
                    return ip_network_base<Base>();
                }
                if (it + 1 == str.end()) {
                    code = error_code::EMPTY_NETMASK;
                    return ip_network_base<Base>();
                }
                netmask = it + 1;
                has_slash = true;
            }
            if (!has_slash) {
                address[symbol++] = c;
            }
            if (symbol > ip_address_type::base_max_string_len) {
                break;
            }
        }

        auto netmask_result = ip_address_type::parse_netmask(netmask, str.end(), code, index);
        
        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        ip_network_base<Base> result;
        result._network_address = ip_address_type::parse(address, code);
        result._netmask = std::get<0>(netmask_result);
        result._prefixlen = std::get<1>(netmask_result);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        result._network_address = ip_address_type::strict_netmask(result._network_address, result._netmask, strict, code);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        return result;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_subnet_of(const ip_network_base& lhs, const ip_network_base& rhs) IPADDRESS_NOEXCEPT {
        return rhs.network_address() <= lhs.network_address() && rhs.broadcast_address() >= lhs.broadcast_address();
    }

    ip_address_type _network_address;
    ip_address_type _netmask;
    size_t _prefixlen;
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

inline int network_strict_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

inline std::istream& strict(std::istream& stream) {
    stream.iword(network_strict_index()) = 0;
    return stream;
}

inline std::istream& non_strict(std::istream& stream) {
    stream.iword(network_strict_index()) = 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <typename Base>
struct hash<IPADDRESS_NAMESPACE::ip_network_base<Base>> {
    IPADDRESS_CONSTEXPR std::size_t operator()(const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) const IPADDRESS_NOEXCEPT {
        return network.hash();
    }
};

template <typename Base>
inline IPADDRESS_CONSTEXPR void swap(IPADDRESS_NAMESPACE::ip_network_base<Base>& net1, IPADDRESS_NAMESPACE::ip_network_base<Base>& net2) IPADDRESS_NOEXCEPT {
    return net1.swap(net2);
}

template <typename Base>
inline std::string to_string(const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    return network.to_string();
}

template <typename Base>
inline std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    return stream << network.to_string(fmt);
}

template <typename Base>
inline std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_strict_index());
    auto strict = iword == 0;
    iword = 0;

    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    network = IPADDRESS_NAMESPACE::ip_network_base<Base>::parse(str, err, strict);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_NETWORK_BASE_HPP
