/**
 * @file      ipv6-network.hpp
 * @brief     Provides a set of functions and classes for handling IPv6 networks
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file defines the ipv6_network_base class and ipv6_network type, which are
 * part of a library for working with IPv6 network addresses. The ipv6_network_base class
 * is derived from the base_v6 class and includes methods for manipulating network addresses.
 * The ipv6_network is a typedef for the ip_network_base class specialized for IPv6, 
 * providing a convenient alias for users of the library.
 */

#ifndef IPADDRESS_IPV6_NETWORK_HPP
#define IPADDRESS_IPV6_NETWORK_HPP

#include "ipv6-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Base class for IPv6 network address manipulation.
 * 
 * The ipv6_network_base class provides foundational functionality for IPv6 network
 * address manipulation by extending the base_v6 class.
 */
class ipv6_network_base : public base_v6<ipv6_network_base> {
public:
    using ip_address_type = ipv6_address; /**< Alias for the IPv6 address type used within the class. */

    /**
     * Checks if the IPv6 network is site-local.
     * 
     * This method determines if both the network address and the broadcast address
     * of the IPv6 network are site-local, which are addresses used within a particular
     * organization's intranet and are not routable on the global internet.
     * 
     * @return A boolean value indicating whether the network is site-local.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT {
        const auto& network = *static_cast<const ip_network_base<ipv6_network_base>*>(this);
        return network.network_address().is_site_local() && network.broadcast_address().is_site_local();
    }

protected:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_type remove_scope_id(const ip_address_type& address) IPADDRESS_NOEXCEPT {
        auto result = address;
        result.set_scope_id("");
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<ipv6_network_base> remove_scope_id(const ip_network_base<ipv6_network_base>& network) IPADDRESS_NOEXCEPT {
        return ip_network_base<ipv6_network_base>::from_address(remove_scope_id(network.network_address()), network.prefixlen());
    }
}; // ipv6_network_base

/**
 * Alias for the specialized ip_network_base class for IPv6.
 * 
 * The ipv6_network is a convenient alias for the ip_network_base class
 * specialized with ipv6_network_base. It allows users to work with IPv6
 * network addresses using a type that is specifically designed for IPv6,
 * simplifying the interface and usage in code that deals with IPv6 networks.
 */
using ipv6_network = ip_network_base<ipv6_network_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal operator for creating an ipv6_network object from a string literal.
     * 
     * This operator allows the creation of ipv6_network objects using a string literal with the
     * `_ipv6_net` suffix.
     * 
     * @tparam FixedString A string literal representing the IPv6 network.
     * @return An ipv6_network object representing the network specified by the string literal.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE ipv6_network operator""_ipv6_net() IPADDRESS_NOEXCEPT {
        return ipv6_network::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal operator for creating an ipv6_network object from a string literal.
     * 
     * This operator allows the creation of ipv6_network objects using a string literal with the
     * `_ipv6_net` suffix.
     * 
     * @param[in] address The string literal representing the IPv6 network.
     * @param[in] size The size of the string literal.
     * @return An ipv6_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_network operator""_ipv6_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_net_from_literal<ipv6_network_base, char, ipv6_network::base_max_string_len + 4>(address, size);
    }

    /**
     * User-defined literal operator for creating an ipv6_network object from a wide string literal.
     * 
     * This operator allows the creation of ipv6_network objects using a string literal with the
     * `_ipv6_net` suffix.
     * 
     * @param[in] address The string literal representing the IPv6 network.
     * @param[in] size The size of the string literal.
     * @return An ipv6_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_network operator""_ipv6_net(const wchar_t* address, std::size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_net_from_literal<ipv6_network_base, wchar_t, ipv6_network::base_max_string_len + 4>(address, size);
    }

    /**
     * User-defined literal operator for creating an ipv6_network object from UTF-16 string literal.
     * 
     * This operator allows the creation of ipv6_network objects using a string literal with the
     * `_ipv6_net` suffix.
     * 
     * @param[in] address The string literal representing the IPv6 network.
     * @param[in] size The size of the string literal.
     * @return An ipv6_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_network operator""_ipv6_net(const char16_t* address, std::size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_net_from_literal<ipv6_network_base, char16_t, ipv6_network::base_max_string_len + 4>(address, size);
    }

    /**
     * User-defined literal operator for creating an ipv6_network object from UTF-32 string literal.
     * 
     * This operator allows the creation of ipv6_network objects using a string literal with the
     * `_ipv6_net` suffix.
     * 
     * @param[in] address The string literal representing the IPv6 network.
     * @param[in] size The size of the string literal.
     * @return An ipv6_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_network operator""_ipv6_net(const char32_t* address, std::size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::parse_net_from_literal<ipv6_network_base, char32_t, ipv6_network::base_max_string_len + 4>(address, size);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif
