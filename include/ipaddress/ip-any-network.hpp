/**
 * @file      ip-any-network.hpp
 * @brief     Defines the ip_network class for representing both IPv4 and IPv6 networks
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file includes the definition of the ip_network class, which is a versatile
 * representation of an IP network that can handle both IPv4 and IPv6 addresses.
 * It provides methods to retrieve network properties such as the network and broadcast
 * addresses, netmask, and hostmask, as well as to check for various types of networks
 * like multicast, private, global, reserved, loopback, link-local, unspecified,
 * site-local and etc.
 * 
 * It integrates the ipv4_network and 
 * ipv6_network classes through a union, ensuring optimal space usage while maintaining 
 * the ability to represent both IP networks versions. This file is part of a larger library 
 * that aims to provide comprehensive support for network operations.
 */

#ifndef IPADDRESS_IP_ANY_NETWORK_HPP
#define IPADDRESS_IP_ANY_NETWORK_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"
#include "ip-any-address.hpp"
#include "ip-any-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename T>
struct net_any_parser {
    template <typename Str>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T parse(const Str& address, bool strict) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::no_error;
        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::no_error) {
            return T(net4);
        }
        return T(ipv6_network::parse(address, strict));
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE T parse(const Str& address, error_code& code, bool strict) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::no_error) {
            return T(net4);
        }
        const auto net6 = ipv6_network::parse(address, code, strict);
        if (code == error_code::no_error) {
            return T(net6);
        }
        return T();
    }
};

} // namespace IPADDRESS_NAMESPACE::internal

/**
 * A class that encapsulates both IPv4 and IPv6 network functionalities.
 * 
 * The ip_network class serves as a versatile tool for network address manipulation,
 * capable of handling both IPv4 and IPv6 networks. It abstracts the complexities associated
 * with the different IP versions and provides a consistent API for network operations.
 * This class is essential for applications that require flexibility in dealing with
 * various IP network types without concern for the underlying IP version.
 */
class ip_network {
public:
    using ip_address_type = ip_address; /**< Type alias for ip_address, representing the IP address within the network. */
    using uint_type = uint128_t; /**< Type alias for a 128-bit unsigned integer type. */

    /**
     * Retrieves the IP version of the network.
     * 
     * @return The IP version as an enumeration (either V4 or V6).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return _version;
    }

    /**
     * Retrieves the prefix length of this network.
     * 
     * This method returns the prefix length of the network. The prefix length is the number
     * of bits in the network address portion of the IP address and is used to calculate the netmask.
     * 
     * @return The prefix length of the network as a size_t value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.prefixlen() : _ipv_net.ipv6.prefixlen();
    }
    
    /**
     * Retrieves the network address of this network.
     * 
     * This method returns the network address component of the ip network object.
     * 
     * @return A reference to the network address.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address network_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.network_address()) : ip_address(_ipv_net.ipv6.network_address());
    }

    /**
     * Retrieves the broadcast address of this network.
     * 
     * The broadcast address is the last address in the network and is used to communicate 
     * with all hosts on this network.
     * 
     * @return The broadcast address of the network as an ip_address object.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address broadcast_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.broadcast_address()) : ip_address(_ipv_net.ipv6.broadcast_address());
    }

    /**
     * Retrieves the netmask of this network.
     * 
     * This method returns the netmask of the network. The netmask is used to determine
     * which portion of an IP address is the network portion and which is the host portion.
     * 
     * @return A reference to the netmask as an ip_address object.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address netmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.netmask()) : ip_address(_ipv_net.ipv6.netmask());
    }

    /**
     * Retrieves the hostmask of this network.
     * 
     * The hostmask identifies the portion of the IP address that is allocated for host identifiers.
     * 
     * @return The hostmask of the network as an ip_address object.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address hostmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.hostmask()) : ip_address(_ipv_net.ipv6.hostmask());
    }

    /**
     * Checks if the network is a multicast network.
     * 
     * This method determines whether the network is a multicast network by checking if both the network
     * address and the broadcast address are multicast addresses.
     * 
     * @return `true` if the network is multicast, `false` otherwise.
     * @see [RFC 3171](https://datatracker.ietf.org/doc/html/rfc3171.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_multicast() : _ipv_net.ipv6.is_multicast();
    }

    /**
     * Checks if the network is a private network.
     * 
     * This method determines whether the network is a private network. Private networks are not routed
     * on the global internet.
     * 
     * @return `true` if the network is private, `false` otherwise.
     * @see [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_private() : _ipv_net.ipv6.is_private();
    }

    /**
     * Checks if the network is a global network.
     * 
     * This method determines whether the network is a global network. Global networks are routable on the
     * internet and are not private.
     * 
     * @return `true` if the network is global, `false` otherwise.
     * @see [iana-ipv4-special-registry](https://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry.xhtml).
     * @see [iana-ipv6-special-registry](https://www.iana.org/assignments/iana-ipv6-special-registry/iana-ipv6-special-registry.xhtml).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_global() : _ipv_net.ipv6.is_global();
    }

    /**
     * Checks if the network is a reserved network.
     * 
     * This method determines whether the network is reserved by checking if both the network address and
     * the broadcast address are reserved addresses.
     * 
     * @return `true` if the network is reserved, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_reserved() : _ipv_net.ipv6.is_reserved();
    }

    /**
     * Checks if the network is a loopback network.
     * 
     * This method determines whether the network is a loopback network by checking if both the network
     * address and the broadcast address are loopback addresses.
     * 
     * @return `true` if the network is loopback, `false` otherwise.
     * @see [RFC 3330](https://datatracker.ietf.org/doc/html/rfc3330.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_loopback() : _ipv_net.ipv6.is_loopback();
    }

    /**
     * Checks if the network is a link-local network.
     * 
     * This method determines whether the network is a link-local network by checking if both the network
     * address and the broadcast address are link-local addresses.
     * 
     * @return `true` if the network is link-local, `false` otherwise.
     * @see [RFC 3927](https://datatracker.ietf.org/doc/html/rfc3927.html).
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_link_local() : _ipv_net.ipv6.is_link_local();
    }

    /**
     * Checks if the network is an unspecified network.
     * 
     * This method determines whether the network is unspecified by checking if both the network address and
     * the broadcast address are unspecified addresses.
     * 
     * @return `true` if the network is unspecified, `false` otherwise.
     * @see [RFC 5735](https://datatracker.ietf.org/doc/html/rfc5735.html) for IPv4.
     * @see [RFC 2373](https://datatracker.ietf.org/doc/html/rfc2373.html) for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_unspecified() : _ipv_net.ipv6.is_unspecified();
    }

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
        return is_v4() ? false : _ipv_net.ipv6.is_site_local();
    }

    /**
     * Checks if the IP network is an IPv4 network.
     * 
     * @return `true` if the IP network is an IPv4 network, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    /**
     * Checks if the IP network is an IPv6 network.
     * 
     * @return `true` if the IP network is an IPv6 network, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    /**
     * Retrieves the size of the IP address.
     * 
     * Depending on the IP version, this function returns the size of the IPv4 or IPv6 address.
     * 
     * @return The size of the IP address in bytes.
     * @remark This is the number of bytes of the IP address that represents the current network.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.size() : _ipv_net.ipv6.size();
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
        return is_v4() ? _ipv_net.ipv4.hash() : _ipv_net.ipv6.hash();
    }

    /**
     * Calculates the total number of addresses in the network.
     * 
     * @return The total number of addresses in the network as a `uint128_t` value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint128_t addresses_count() const IPADDRESS_NOEXCEPT {
        return is_v4() ? uint128_t(_ipv_net.ipv4.addresses_count()) : _ipv_net.ipv6.addresses_count();
    }

    /**
     * Checks if the given IP address is contained within this network.
     * 
     * This method determines whether the provided IP address is part of the network
     * represented by this ip network object, based on the network address and netmask.
     * 
     * @code{.cpp}
     *   constexpr auto result1 = ip_network::parse("192.0.2.0/28").contains(ip_address::parse("192.0.2.6"));
     *   constexpr auto result2 = ip_network::parse("192.0.2.0/28").contains(ip_address::parse("192.0.3.6"));
     * 
     *   std::cout << std::boolalpha << result1 << std::endl;
     *   std::cout << std::boolalpha << result2 << std::endl;
     * 
     *   // out:
     *   // true
     *   // false
     * @endcode
     * @param[in] address The IP address to check.
     * @return `true` if the address is part of the network, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool contains(const ip_address& address) const IPADDRESS_NOEXCEPT {
        if (_version == address.version()) {
            return is_v4() ? _ipv_net.ipv4.contains(address.v4().value()) : _ipv_net.ipv6.contains(address.v6().value());
        } else {
            return false;
        }
    }

    /**
     * Determines if this network overlaps with another network.
     * 
     * This method checks if there is any overlap between the network represented by this
     * ip network object and another network, meaning if any part of one network lies within the other.
     * 
     * @code{.cpp}
     *   constexpr auto a = ip_network::parse("1.2.3.0/24");
     *   constexpr auto b = ip_network::parse("1.2.3.0/30");
     *   constexpr auto overlaps = a.overlaps(b);
     * 
     *   std::cout << std::boolalpha << overlaps << std::endl;
     * 
     *   // out:
     *   // true
     * @endcode
     * @param[in] other The other ip network object to compare with.
     * @return `true` if there is an overlap, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool overlaps(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.overlaps(other.v4().value()) : _ipv_net.ipv6.overlaps(other.v6().value());
        } else {
            return false;
        }
    }

    /**
     * Checks if this network is a subnet of another network.
     * 
     * This method verifies if the network represented by this ip network object is a subnet
     * of the provided network, meaning if this network is entirely contained within the other network.
     * 
     * @code{.cpp}
     *   constexpr auto a = ip_network::parse("192.168.1.0/24");
     *   constexpr auto b = ip_network::parse("192.168.1.128/30");
     *   constexpr auto subnet_of = a.subnet_of(b);
     * 
     *   std::cout << std::boolalpha << subnet_of << std::endl;
     * 
     *   // out:
     *   // false
     * @endcode
     * @param[in] other The other ip network object to compare with.
     * @return `true` if this network is a subnet of the other network, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool subnet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.subnet_of(other.v4().value()) : _ipv_net.ipv6.subnet_of(other.v6().value());
        } else {
            return false;
        }
    }

    /**
     * Checks if this network is a supernet of another network.
     * 
     * This method determines if the network represented by this ip network object is a supernet
     * of the provided network, meaning if the other network is entirely contained within this network.
     * 
     * @code{.cpp}
     *   constexpr auto a = ip_network::parse("192.168.1.0/24");
     *   constexpr auto b = ip_network::parse("192.168.1.128/30");
     *   constexpr auto supernet_of = a.supernet_of(b);
     * 
     *   std::cout << std::boolalpha << supernet_of << std::endl;
     * 
     *   // out:
     *   // true
     * @endcode
     * @param[in] other The other ip network object to compare with.
     * @return True if this network is a supernet of the other network, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool supernet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.supernet_of(other.v4().value()) : _ipv_net.ipv6.supernet_of(other.v6().value());
        } else {
            return false;
        }
    }

    /**
     * Generates a supernet from this network.
     * 
     * The supernet containing this network definition, depending on the argument values. \a prefixlen_diff 
     * is the amount our prefix length should be decreased by. new_prefix is the desired new prefix of the 
     * supernet; it must be smaller than our prefix. One and only one of \a prefixlen_diff and new_prefix must 
     * be set.
     * 
     * @code{.cpp}
     *   constexpr auto supernet = ip_network::parse("192.0.2.0/24").supernet(2);
     *   
     *   std::cout << supernet << std::endl;
     *   
     *   // out:
     *   // 192.0.0.0/22
     * @endcode
     * @param[in] prefixlen_diff The amount by which the prefix length should be decreased. *Defaults to 1*.
     * @param[in] new_prefixlen An optional new prefix length for the supernet. If not specified, the prefix length is determined by subtracting \a prefixlen_diff from the current prefix length.
     * @return An ip network object representing the supernet, or the current network if an error occurs.
     * @throw logic_error Raised if the operation cannot be performed due to invalid parameters or prefix length.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network supernet(size_t prefixlen_diff = 1, const optional<size_t>& new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return is_v4() ? ip_network(_ipv_net.ipv4.supernet(prefixlen_diff, new_prefixlen)) : ip_network(_ipv_net.ipv6.supernet(prefixlen_diff, new_prefixlen));
    }

    /**
     * Generates a supernet from this network with error handling.
     * 
     * The supernet containing this network definition, depending on the argument values. \a prefixlen_diff 
     * is the amount our prefix length should be decreased by. new_prefix is the desired new prefix of the 
     * supernet; it must be smaller than our prefix. One and only one of \a prefixlen_diff and new_prefix must 
     * be set.
     * 
     * @code{.cpp}
     *   auto err = error_code::no_error;
     *   auto supernet = ip_network::parse("192.0.2.0/24").supernet(err, 2);
     *   
     *   if (err == error_code::no_error) {
     *       std::cout << supernet << std::endl;
     *   }
     *   
     *   // out:
     *   // 192.0.0.0/22
     * @endcode
     * @param[out] code An error_code object that will be set if an error occurs during the operation.
     * @param[in] prefixlen_diff The amount by which the prefix length should be decreased. *Defaults to 1*.
     * @param[in] new_prefixlen An optional new prefix length for the supernet. If not specified, the prefix length is determined by subtracting \a prefixlen_diff from the current prefix length.
     * @return An ip network object representing the supernet, or the current network if an error occurs.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network supernet(error_code& code, size_t prefixlen_diff = 1, const optional<size_t>& new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_network(_ipv_net.ipv4.supernet(code, prefixlen_diff, new_prefixlen)) : ip_network(_ipv_net.ipv6.supernet(code, prefixlen_diff, new_prefixlen));
    }

    /**
     * Retrieves a sequence of host addresses in the network.
     * 
     * This method returns a sequence of host addresses within the network, excluding the network and
     * broadcast addresses.
     * 
     * @code{.cpp}
     *   constexpr auto hosts_sequence = ip_network::parse("192.0.2.0/29").hosts();
     *   
     *   for (const auto& addr : hosts_sequence) {
     *      std::cout << addr << std::endl;
     *   }
     * 
     *   // out:
     *   // 192.0.2.1
     *   // 192.0.2.2
     *   // 192.0.2.3
     *   // 192.0.2.4
     *   // 192.0.2.5
     *   // 192.0.2.6
     * @endcode
     * @return A `hosts_sequence` object representing the sequence of host addresses.
     * @retval Ipv4 For ipv4 all the IP addresses that belong to the network, except the network 
     *         address itself and the network broadcast address. For networks with a mask length of 31, 
     *         the network address and network broadcast address are also included in the result. 
     *         Networks with a mask of 32 will return a list containing the single host address.
     * @retval Ipv6 For ipv6 all the IP addresses that belong to the network, except the 
     *         Subnet-Router anycast address. For networks with a mask length of 127, the Subnet-Router 
     *         anycast address is also included in the result. Networks with a mask of 128 will return a 
     *         list containing the single host address.
     * @warning Please note that with IPv6, the number of addresses can be so large that iterating through 
     *          them all may be practically impossible. Therefore, use the hosts() method cautiously to 
     *          avoid endlessly retrieving addresses.
     * @remark `hosts_sequence` uses lazy evaluation to iterate addresses.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_any_sequence hosts() const IPADDRESS_NOEXCEPT {
        if (is_v4()) {
            const auto sequence = _ipv_net.ipv4.hosts();
            return hosts_any_sequence(sequence.begin(), sequence.end());
        } else {
            const auto sequence = _ipv_net.ipv6.hosts();
            return hosts_any_sequence(sequence.begin(), sequence.end());
        }
    }

    /**
     * Generates a sequence of subnets from this network.
     * 
     * The subnets that join to make the current network definition, depending on the argument values. 
     * \a prefixlen_diff is the amount our prefix length should be increased by. new_prefix is the desired 
     * new prefix of the subnets; it must be larger than our prefix. One and only one of \a prefixlen_diff 
     * and new_prefix must be set.
     * 
     * @code{.cpp}
     *   constexpr auto subnets_sequence = ip_network::parse("192.0.2.0/24").subnets(2);
     *   
     *   for (const auto& net : subnets_sequence) {
     *      std::cout << net << std::endl;
     *   }
     * 
     *   // out:
     *   // 192.0.2.0/26
     *   // 192.0.2.64/26
     *   // 192.0.2.128/26
     *   // 192.0.2.192/26
     * @endcode
     * @param[in] prefixlen_diff The difference in prefix length for the subnets. *Defaults to 1*.
     * @param[in] new_prefixlen An optional new prefix length for the subnets. If not specified, the prefix length is determined by adding \a prefixlen_diff to the current prefix length.
     * @return A `subnets_any_sequence` object representing the sequence of subnets.
     * @throw logic_error Raised if the operation cannot be performed due to invalid parameters or prefix length.
     * @remark `subnets_any_sequence` uses lazy evaluation to iterate over the subnets.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence<ip_network> subnets(size_t prefixlen_diff = 1, const optional<size_t>& new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (is_v4()) {
            const auto sequence = _ipv_net.ipv4.subnets(prefixlen_diff, new_prefixlen);
            return subnets_any_sequence<ip_network>(sequence.begin(), sequence.end());
        } else {
            const auto sequence = _ipv_net.ipv6.subnets(prefixlen_diff, new_prefixlen);
            return subnets_any_sequence<ip_network>(sequence.begin(), sequence.end());
        }
    }

    /**
     * Generates a sequence of subnets from this network with error handling.
     * 
     * The subnets that join to make the current network definition, depending on the argument values. 
     * \a prefixlen_diff is the amount our prefix length should be increased by. new_prefix is the desired 
     * new prefix of the subnets; it must be larger than our prefix. One and only one of \a prefixlen_diff 
     * and new_prefix must be set.
     * 
     * @code{.cpp}
     *   auto err = error_code::no_error;
     *   auto subnets_sequence = ip_network::parse("192.0.2.0/24").subnets(err, 2);
     *   
     *   if (err == error_code::no_error) {
     *       for (const auto& net : subnets_sequence) {
     *          std::cout << net << std::endl;
     *       }
     *   }
     * 
     *   // out:
     *   // 192.0.2.0/26
     *   // 192.0.2.64/26
     *   // 192.0.2.128/26
     *   // 192.0.2.192/26
     * @endcode
     * @param[out] code An error_code object that will be set if an error occurs during the operation.
     * @param[in] prefixlen_diff The difference in prefix length for the subnets. *Defaults to 1*.
     * @param[in] new_prefixlen An optional new prefix length for the subnets. If not specified, the prefix length is determined by adding \a prefixlen_diff to the current prefix length.
     * @return A `subnets_any_sequence` object representing the sequence of subnets, or an empty sequence if an error occurs.
     * @remark `subnets_any_sequence` uses lazy evaluation to iterate over the subnets.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE subnets_any_sequence<ip_network> subnets(error_code& code, size_t prefixlen_diff = 1, const optional<size_t>& new_prefixlen = nullptr) const IPADDRESS_NOEXCEPT {
        if (is_v4()) {
            const auto sequence = _ipv_net.ipv4.subnets(code, prefixlen_diff, new_prefixlen);
            return subnets_any_sequence<ip_network>(sequence.begin(), sequence.end());
        } else {
            const auto sequence = _ipv_net.ipv6.subnets(code, prefixlen_diff, new_prefixlen);
            return subnets_any_sequence<ip_network>(sequence.begin(), sequence.end());
        }
    }

    /**
     * Computes the network definitions resulting from removing the given network from this one.
     * 
     * @code{.cpp}
     *   constexpr auto a = ip_network::parse("192.0.2.0/28");
     *   constexpr auto b = ip_network::parse("192.0.2.1/32");
     *   constexpr auto exclude_sequence = a.address_exclude(b);
     *   
     *   for (const auto& net : exclude_sequence) {
     *      std::cout << net << std::endl;
     *   }
     * 
     *   // out:
     *   // 192.0.2.8/29
     *   // 192.0.2.4/30
     *   // 192.0.2.2/31
     *   // 192.0.2.0/32
     * @endcode
     * @param[in] other The other network to exclude from this network.
     * @return An `exclude_network_sequence` object representing the remaining address ranges.
     * @throw logic_error Raise if network is not completely contained in this network.
     * @remark `exclude_network_sequence` uses lazy evaluation to iterate networks.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence<ip_network> address_exclude(const ip_network& other) const IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code code = error_code::no_error;
        const auto result = address_exclude(other, code);
        if (code != error_code::no_error) {
            raise_error(code, 0, "", 0);
        }
        return result;
    }

    /**
     * Computes the network definitions resulting from removing the given network from this one, with error handling.
     * 
     * @code{.cpp}
     *   constexpr auto a = ip_network::parse("192.0.2.0/28");
     *   constexpr auto b = ip_network::parse("192.0.2.1/32");
     * 
     *   auto err = error_code::no_error;
     *   auto exclude_sequence = a.address_exclude(b, err);
     *   
     *   if (err == error_code::no_error) {
     *       for (const auto& net : exclude_sequence) {
     *          std::cout << net << std::endl;
     *       }
     *   }
     * 
     *   // out:
     *   // 192.0.2.8/29
     *   // 192.0.2.4/30
     *   // 192.0.2.2/31
     *   // 192.0.2.0/32
     * @endcode
     * @param[in] other The other network to exclude from this network.
     * @param[out] code An error_code object that will be set if an error occurs during the operation.
     * @return An `exclude_network_sequence` object representing the remaining address ranges, or an empty sequence if an error occurs.
     * @remark `exclude_network_sequence` uses lazy evaluation to iterate networks.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE exclude_network_sequence<ip_network> address_exclude(const ip_network& other, error_code& code) const IPADDRESS_NOEXCEPT {
        code = error_code::no_error;
        if (_version != other._version) {
            code = error_code::invalid_version;
            return exclude_network_sequence<ip_network>();
        }

        auto addr1 = network_address(); addr1.set_scope_id("");
        auto addr2 = other.network_address(); addr2.set_scope_id("");
        auto lhs = ip_network::from_address(addr1, prefixlen());
        auto rhs = ip_network::from_address(addr2, other.prefixlen());

        if (!rhs.subnet_of(lhs)) {
            code = error_code::not_contained_network;
            return exclude_network_sequence<ip_network>();
        }

        if (lhs == rhs) {
            return exclude_network_sequence<ip_network>();
        }

        return exclude_network_sequence<ip_network>(lhs, rhs);
    }

    /**
     * Retrieves the IPv4 network.
     * 
     * If the IP network is an IPv4 network, this function returns the IPv4 network.
     * 
     * @return An optional containing the IPv4 network, or an empty optional if the IP network is not IPv4.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_network> v4() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            return optional<ipv4_network>(_ipv_net.ipv4);
        }
        return optional<ipv4_network>();
    }

    /**
     * Retrieves the IPv6 network.
     * 
     * If the IP network is an IPv6 network, this function returns the IPv6 network.
     * 
     * @return An optional containing the IPv6 network, or an empty optional if the IP network is not IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv6_network> v6() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            return optional<ipv6_network>(_ipv_net.ipv6);
        }
        return optional<ipv6_network>();
    }

    /**
     * Constructs a new IP network object.
     * 
     * Initializes a new instance of the ip_network class with the maximum prefix length
     * for the IPv4 address, effectively setting the network address to the default address
     * and the netmask to the maximum value.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network() IPADDRESS_NOEXCEPT { // NOLINT(modernize-use-equals-default): for C++11 support
    }

    /**
     * Constructor from an ipv4_network.
     * 
     * Constructs an ip_network object with the specified IPv4 network.
     * 
     * @param[in] net4 An ipv4_network object to initialize the ip_network.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv4_network& net4) IPADDRESS_NOEXCEPT : _ipv_net(net4) {
    }

    /**
     * Constructor from an ipv6_network.
     * 
     * Constructs an ip_network object with the specified IPv6 network.
     * 
     * @param[in] net6 An ipv6_network object to initialize the ip_network.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv6_network& net6) IPADDRESS_NOEXCEPT : _ipv_net(net6), _version(ip_version::V6) {
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
     * @return An ip_network object representing the network.
     * @throw parse_error Exception caused by invalid input string.
     * @remark May throw an exception if the address does not conform to the netmask when \a strict is `true`.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network from_address(
        const ip_address& address, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), prefixlen, strict));
    }

    /**
     * Creates an ip network object from a given IP address and prefix length, with error handling.
     * 
     * Similar to the from_address method above, but this version allows for error handling without exceptions.
     * It populates the provided error_code object instead of throwing.
     * 
     * @param[in] address The IP address to use for creating the network.
     * @param[out] code An error_code object that will be set if an error occurs.
     * @param[in] prefixlen The prefix length for the network's netmask. Defaults to the maximum prefix length.
     * @param[in] strict Whether to validate the address against the netmask.
     * @return An ip_network object representing the network, or an object in an error state if an error occurs.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network from_address(
        const ip_address& address, 
        error_code& code, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), code, prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), code, prefixlen, strict));
    }

    /**
     * Converts the network to a string representation.
     * 
     * This method returns a string representation of the network, combining the network address
     * and the prefix length, formatted according to the specified format.
     * 
     * @param[in] fmt The format to use for the string representation. *Defaults to format::compressed*.
     * @return A string representation of the network.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return is_v4() ? _ipv_net.ipv4.to_string(fmt) : _ipv_net.ipv6.to_string(fmt);
    }

    /**
     * Swaps the contents of this network with another network.
     * 
     * This method exchanges the network address, netmask, and prefix length with those of another
     * ip network object.
     * 
     * @param[in,out] net The other ip_network object to swap contents with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(ip_network& net) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = net;
        net = tmp;
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
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE ip_network parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::no_error;

        char net[str.size() + 1]{};
        for (size_t i = 0; i < str.size(); ++i) {
            net[i] = str[i];
        }

        const auto net4 = ipv4_network::parse(net, code, Strict);
        if (code == error_code::no_error) {
            return ip_network(net4);
        }
        
        return ip_network(ipv6_network::parse<FixedString, Strict>());
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::wstring_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u8string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u16string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u32string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::wstring_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u8string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u16string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u32string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
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
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::wstring`.
     * 
     * @param[in] address The `std::wstring` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::wstring& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u16string`.
     * 
     * @param[in] address The `std::u16string` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::u16string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u32string`.
     * 
     * @param[in] address The `std::u32string` representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     * @throw parse_error Exception caused by invalid input string.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::u32string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return internal::net_any_parser<ip_network>::parse(address, strict);
    }

    /**
     * Parses a network address and prefix from a `std::string` and reports errors through an error code.
     * 
     * @param[in] address The `std::string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static IPADDRESS_FORCE_INLINE ip_network parse(const std::string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
    }

    /**
     * Parses a network address and prefix from a `std::wstring` and reports errors through an error code.
     * 
     * @param[in] address The `std::wstring` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static IPADDRESS_FORCE_INLINE ip_network parse(const std::wstring& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u16string` and reports errors through an error code.
     * 
     * @param[in] address The `std::u16string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static IPADDRESS_FORCE_INLINE ip_network parse(const std::u16string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
    }

    /**
     * Parses a network address and prefix from a `std::u32string` and reports errors through an error code.
     * 
     * @param[in] address The `std::u32string` representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    static IPADDRESS_FORCE_INLINE ip_network parse(const std::u32string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return internal::net_any_parser<ip_network>::parse(address, code, strict);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    /**
     * Parses a network address and prefix from a character array.
     * 
     * This function parses an IP network address and prefix length from a character array of a 
     * specified size. Can check and get the result at compile time.
     * 
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array representing the IP network in "address/prefix" format.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(const T(&address)[N], bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::no_error;
        auto result = internal::net_any_parser<ip_network>::parse(address, code, strict);
        if (code != error_code::no_error) {
            raise_error(code, 0, address, N);
        }
        return result;
    }

    /**
     * Parses a network address and prefix from a character array and reports errors through an error code.
     * 
     * This function parses an IP network address and prefix length from a character array of a specified size 
     * and provides an error code if the parsing fails.
     * 
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] address The character array representing the IP network in "address/prefix" format.
     * @param[out] code An error_code object that will be set if an error occurs during parsing.
     * @param[in] strict A boolean flag indicating whether to perform strict validation of the address.
     * @return An ip network object representing the parsed network.
     */
    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(const T(&address)[N], error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        code = error_code::no_error;

        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::no_error) {
            return ip_network(net4);
        }
        
        const auto net6 = ipv6_network::parse(address, code, strict);
        if (code == error_code::no_error) {
            return ip_network(net6);
        }
        
        return ip_network();
    }

    /**
     * Converts the ip network object to a std::string.
     * 
     * @return A `std::string` representation of the ip network object.
     */
    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return is_v4() ? _ipv_net.ipv4.to_string() : _ipv_net.ipv6.to_string();
    }

    /**
     * Equality comparison operator.
     * 
     * Compares this ip network object to another for equality based on the network address and netmask.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if both objects are equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version != rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 == rhs._ipv_net.ipv4) : (_ipv_net.ipv6 == rhs._ipv_net.ipv6);
    }

    /**
     * Inequality comparison operator.
     * 
     * Compares this ip network object to another for inequality.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if both objects are not equal, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Three-way comparison operator (spaceship operator).
     * 
     * Compares this ip network object to another using three-way comparison.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `std::strong_ordering` result of the comparison.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> rhs._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? (_ipv_net.ipv4 <=> rhs._ipv_net.ipv4) : (_ipv_net.ipv6 <=> rhs._ipv_net.ipv6);
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less than comparison operator.
     * 
     * Determines if this ip network object is less than another by comparing network addresses and netmasks.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if this object is less than the other, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version < rhs._version) {
            return true;
        }
        if (_version > rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 < rhs._ipv_net.ipv4) : (_ipv_net.ipv6 < rhs._ipv_net.ipv6);
    }
    
    /**
     * Greater than comparison operator.
     * 
     * Determines if this ip network object is greater than another.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if this object is greater than the other, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    /**
     * Less than or equal to comparison operator.
     * 
     * Determines if this ip network object is less than or equal to another.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if this object is less than or equal to the other, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    /**
     * Greater than or equal to comparison operator.
     * 
     * Determines if this ip network object is greater than or equal to another.
     * 
     * @param[in] rhs The other ip network object to compare with.
     * @return `true` if this object is greater than or equal to the other, `false` otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    union ip_any_network {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network() IPADDRESS_NOEXCEPT : ipv4() {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network(const ipv4_network& net) IPADDRESS_NOEXCEPT : ipv4(net) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network(const ipv6_network& net) IPADDRESS_NOEXCEPT : ipv6(net) {
        }

        ipv4_network ipv4;
        ipv6_network ipv6;
    } _ipv_net {};
    ip_version _version = ip_version::V4;
};

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal operator for creating an ip_network object from a string literal.
     * 
     * This operator allows the creation of ip_network objects using a string literal with the
     * `_net` suffix.
     * 
     * @tparam FixedString A string literal representing the IP network.
     * @return An ip_network object representing the network specified by the string literal.
     */
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD IPADDRESS_CONSTEVAL IPADDRESS_FORCE_INLINE ip_network operator""_net() IPADDRESS_NOEXCEPT {
        return ip_network::parse<FixedString>();
    }

#else // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    /**
     * User-defined literal operator for creating an ip_network object from a string literal.
     * 
     * This operator allows the creation of ip_network objects using a string literal with the
     * `_net` suffix.
     * 
     * @param[in] address The string literal representing the IP network.
     * @param[in] size The size of the string literal.
     * @return An ip_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network operator""_net(const char* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        const auto max_len = ipv6_address::base_max_string_len * 2 + 1;
        if (size > max_len) {
            raise_error(error_code::string_is_too_long, 0, address, size);
        }
        char str[max_len + 1] = {};
        for (size_t i = 0; i < size && i < max_len; ++i) {
            str[i] = address[i];
        }
        return ip_network::parse(str);
    }

    /**
     * User-defined literal operator for creating an ip_network object from a wide string literal.
     * 
     * This operator allows the creation of ip_network objects using a string literal with the
     * `_net` suffix.
     * 
     * @param[in] address The string literal representing the IP network.
     * @param[in] size The size of the string literal.
     * @return An ip_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network operator""_net(const wchar_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        const auto max_len = ipv6_address::base_max_string_len * 2 + 1;
        if (size > max_len) {
            raise_error(error_code::string_is_too_long, 0, address, size);
        }
        wchar_t str[max_len + 1] = {};
        for (size_t i = 0; i < size && i < max_len; ++i) {
            str[i] = address[i];
        }
        return ip_network::parse(str);
    }

    /**
     * User-defined literal operator for creating an ip_network object from UTF-16 string literal.
     * 
     * This operator allows the creation of ip_network objects using a string literal with the
     * `_net` suffix.
     * 
     * @param[in] address The string literal representing the IP network.
     * @param[in] size The size of the string literal.
     * @return An ip_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network operator""_net(const char16_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        const auto max_len = ipv6_address::base_max_string_len * 2 + 1;
        if (size > max_len) {
            raise_error(error_code::string_is_too_long, 0, address, size);
        }
        char16_t str[max_len + 1] = {};
        for (size_t i = 0; i < size && i < max_len; ++i) {
            str[i] = address[i];
        }
        return ip_network::parse(str);
    }

    /**
     * User-defined literal operator for creating an ip_network object from UTF-32 string literal.
     * 
     * This operator allows the creation of ip_network objects using a string literal with the
     * `_net` suffix.
     * 
     * @param[in] address The string literal representing the IP network.
     * @param[in] size The size of the string literal.
     * @return An ip_network object representing the network specified by the string literal.
     */
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network operator""_net(const char32_t* address, size_t size) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        const auto max_len = ipv6_address::base_max_string_len * 2 + 1;
        if (size > max_len) {
            raise_error(error_code::string_is_too_long, 0, address, size);
        }
        char32_t str[max_len + 1] = {};
        for (size_t i = 0; i < size && i < max_len; ++i) {
            str[i] = address[i];
        }
        return ip_network::parse(str);
    }

#endif // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <>
struct hash<IPADDRESS_NAMESPACE::ip_network> {
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t operator()(const IPADDRESS_NAMESPACE::ip_network& network) const IPADDRESS_NOEXCEPT {
        return network.hash();
    }
};

IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_network& net1, IPADDRESS_NAMESPACE::ip_network& net2) IPADDRESS_NOEXCEPT {
    return net1.swap(net2);
}

IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_network& network) {
    return network.to_string();
}

IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_network& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    auto str = network.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        auto end = std::find(str.cbegin(), str.cend(), '%');
        std::transform(str.cbegin(), end, str.begin(), [](char c){
            return std::toupper(c);
        });
    }
    return stream << str;
}

template <typename T>
IPADDRESS_FORCE_INLINE std::basic_istream<T, std::char_traits<T>>& operator>>(std::basic_istream<T, std::char_traits<T>>& stream, IPADDRESS_NAMESPACE::ip_network& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_strict_index());
    auto strict = iword == 0;
    iword = 0;

    std::basic_string<T, std::char_traits<T>, std::allocator<T>> str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err = IPADDRESS_NAMESPACE::error_code::no_error;
    network = IPADDRESS_NAMESPACE::ip_network::parse(str, err, strict);
    if (err != IPADDRESS_NAMESPACE::error_code::no_error) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ANY_NETWORK_HPP
