/**
 * @file      ipv4-network.hpp
 * @brief     Provides a set of functions and classes for handling IPv4 networks
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file defines the ipv4_network_base class and ipv4_network type, which are
 * part of a library for working with IPv4 network addresses. The ipv4_network_base class
 * is derived from the base_v4 class and includes methods for manipulating network addresses.
 * The ipv4_network is a typedef for the ip_network_base class specialized for IPv4, 
 * providing a convenient alias for users of the library.
 */

#ifndef IPADDRESS_IPV4_NETWORK_HPP
#define IPADDRESS_IPV4_NETWORK_HPP

#include "ipv4-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Base class for IPv4 network address manipulation.
 * 
 * The ipv4_network_base class provides foundational functionality for IPv4 network
 * address manipulation by extending the base_v4 class.
 */
class ipv4_network_base : public base_v4<ipv4_network_base> {
public:
    using ip_address_type = ipv4_address; /**< Alias for the IPv4 address type used within the class. */

protected:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_type remove_scope_id(const ip_address_type& address) IPADDRESS_NOEXCEPT {
        return address;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<ipv4_network_base> remove_scope_id(const ip_network_base<ipv4_network_base>& network) IPADDRESS_NOEXCEPT {
        return network;
    }
}; // ipv4_network_base

/**
 * Alias for the specialized ip_network_base class for IPv4.
 * 
 * The ipv4_network is a convenient alias for the ip_network_base class
 * specialized with ipv4_network_base. It allows users to work with IPv4
 * network addresses using a type that is specifically designed for IPv4,
 * simplifying the interface and usage in code that deals with IPv4 networks.
 */
using ipv4_network = ip_network_base<ipv4_network_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE ipv4_network operator""_ipv4_net() IPADDRESS_NOEXCEPT {
        return ipv4_network::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_network operator""_ipv4_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_network::base_max_string_len * 2 + 1 && "literal string is too long");
        char str[ipv4_network::base_max_string_len * 2 + 2] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_network::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_NETWORK_HPP
