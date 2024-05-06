/**
 * @file      ip-networks.hpp
 * @brief     Defines constants for various reserved IP networks
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file contains arrays of ipv4_network and ipv6_network objects representing
 * reserved IP address ranges, such as private networks and multicast addresses.
 * These constants are used to facilitate the identification and handling of these
 * special address ranges in network-related operations.
 */

#ifndef IPADDRESS_IP_NETWORKS_HPP
#define IPADDRESS_IP_NETWORKS_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename>
struct networks {
#if __cpp_constexpr < 201304L
    static const ipv4_network ipv4_private_networks[];
    static const ipv6_network ipv6_private_networks[];

    static const ipv4_network ipv4_is_public_network;

    static const ipv4_network ipv4_reserved_network;
    static const ipv6_network ipv6_reserved_networks[];

    static const ipv4_network ipv4_is_multicast;
    static const ipv6_network ipv6_is_multicast;

    static const ipv4_network ipv4_is_loopback;

    static const ipv4_network ipv4_is_link_local;
    static const ipv6_network ipv6_is_link_local;

    static const ipv6_network ipv6_is_site_local;
};
#endif

// Private networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_private_networks[] = {
        ipv4_network::parse("0.0.0.0/8"),
        ipv4_network::parse("10.0.0.0/8"),
        ipv4_network::parse("127.0.0.0/8"),
        ipv4_network::parse("169.254.0.0/16"),
        ipv4_network::parse("172.16.0.0/12"),
        ipv4_network::parse("192.0.0.0/29"),
        ipv4_network::parse("192.0.0.170/31"),
        ipv4_network::parse("192.0.2.0/24"),
        ipv4_network::parse("192.168.0.0/16"),
        ipv4_network::parse("198.18.0.0/15"),
        ipv4_network::parse("198.51.100.0/24"),
        ipv4_network::parse("203.0.113.0/24"),
        ipv4_network::parse("240.0.0.0/4"),
        ipv4_network::parse("255.255.255.255/32")
    };

#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_private_networks[] = {
        ipv6_network::parse("::1/128"),
        ipv6_network::parse("::/128"),
        ipv6_network::parse("::ffff:0:0/96"),
        ipv6_network::parse("100::/64"),
        ipv6_network::parse("2001::/23"),
        ipv6_network::parse("2001:2::/48"),
        ipv6_network::parse("2001:db8::/32"),
        ipv6_network::parse("2001:10::/28"),
        ipv6_network::parse("fc00::/7"),
        ipv6_network::parse("fe80::/10")
    };

// Global networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_is_public_network = ipv4_network::parse("100.64.0.0/10");

// Reserved networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_reserved_network = ipv4_network::parse("240.0.0.0/4");

#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_reserved_networks[] = {
        ipv6_network::parse("::/8"),
        ipv6_network::parse("100::/8"),
        ipv6_network::parse("200::/7"),
        ipv6_network::parse("400::/6"),
        ipv6_network::parse("800::/5"),
        ipv6_network::parse("1000::/4"),
        ipv6_network::parse("4000::/3"),
        ipv6_network::parse("6000::/3"),
        ipv6_network::parse("8000::/3"),
        ipv6_network::parse("A000::/3"),
        ipv6_network::parse("C000::/3"),
        ipv6_network::parse("E000::/4"),
        ipv6_network::parse("F000::/5"),
        ipv6_network::parse("F800::/6"),
        ipv6_network::parse("FE00::/9")
    };

// Multicast networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_is_multicast = ipv4_network::parse("224.0.0.0/4");

#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_is_multicast = ipv6_network::parse("ff00::/8");

// Loopback networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_is_loopback = ipv4_network::parse("127.0.0.0/8");

// Link local networks
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_is_link_local = ipv4_network::parse("169.254.0.0/16");

#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_is_link_local = ipv6_network::parse("fe80::/10");

// Site local networks
#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_is_site_local = ipv6_network::parse("fec0::/10");

#if __cpp_constexpr >= 201304L
};

template <typename T>
constexpr ipv4_network networks<T>::ipv4_private_networks[];

template <typename T>
constexpr ipv6_network networks<T>::ipv6_private_networks[];

template <typename T>
constexpr ipv4_network networks<T>::ipv4_is_public_network;

template <typename T>
constexpr ipv4_network networks<T>::ipv4_reserved_network;

template <typename T>
constexpr ipv6_network networks<T>::ipv6_reserved_networks[];

template <typename T>
constexpr ipv4_network networks<T>::ipv4_is_multicast;

template <typename T>
constexpr ipv6_network networks<T>::ipv6_is_multicast;

template <typename T>
constexpr ipv4_network networks<T>::ipv4_is_loopback;

template <typename T>
constexpr ipv4_network networks<T>::ipv4_is_link_local;

template <typename T>
constexpr ipv6_network networks<T>::ipv6_is_link_local;

template <typename T>
constexpr ipv6_network networks<T>::ipv6_is_site_local;

#endif

using nets = networks<int>;

} // namespace IPADDRESS_NAMESPACE::internal

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_network::is_private() const IPADDRESS_NOEXCEPT {
    const auto& address = network_address();
    const auto broadcast = broadcast_address();
    for (const auto& private_network : internal::nets::ipv4_private_networks) {
        if (private_network.contains(address) && private_network.contains(broadcast)) {
            return true;
        }
    }
    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_network::is_private() const IPADDRESS_NOEXCEPT {
    const auto& address = network_address();
    const auto broadcast = broadcast_address();
    for (const auto& private_network : internal::nets::ipv6_private_networks) {
        if (private_network.contains(address) && private_network.contains(broadcast)) {
            return true;
        }
    }
    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_network::is_global() const IPADDRESS_NOEXCEPT {
    const auto& network = internal::nets::ipv4_is_public_network;
    const auto& address = network_address();
    const auto broadcast = broadcast_address();
    return !(network.contains(address) && network.contains(broadcast)) && !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_network::is_global() const IPADDRESS_NOEXCEPT {
    return !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_multicast() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv4_is_multicast.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_multicast() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv6_is_multicast.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_private() const IPADDRESS_NOEXCEPT {
    for (const auto& private_network : internal::nets::ipv4_private_networks) {
        if (private_network.contains(*this)) {
            return true;
        }
    }
    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_private() const IPADDRESS_NOEXCEPT {
    const auto ipv4 = this->ipv4_mapped();

    if (ipv4) {
        return ipv4->is_private();
    }

    for (const auto& private_network : internal::nets::ipv6_private_networks) {
        if (private_network.contains(*this)) {
            return true;
        }
    }

    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_global() const IPADDRESS_NOEXCEPT {
    return !internal::nets::ipv4_is_public_network.contains(*this) && !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_global() const IPADDRESS_NOEXCEPT {
    return !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_reserved() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv4_reserved_network.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_reserved() const IPADDRESS_NOEXCEPT {
    for (const auto& reserved_network : internal::nets::ipv6_reserved_networks) {
        if (reserved_network.contains(*this)) {
            return true;
        }
    }
    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_loopback() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv4_is_loopback.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_loopback() const IPADDRESS_NOEXCEPT {
    const auto b = bytes();
    return b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0 
        && b[4] == 0 && b[5] == 0 && b[6] == 0 && b[7] == 0 
        && b[8] == 0 && b[9] == 0 && b[10] == 0 && b[11] == 0 
        && b[12] == 0 && b[13] == 0 && b[14] == 0 && b[15] == 1;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_link_local() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv4_is_link_local.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_link_local() const IPADDRESS_NOEXCEPT {
    return internal::nets::ipv6_is_link_local.contains(*this);
}

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address_base::is_site_local() const IPADDRESS_NOEXCEPT {
    ipv6_address address(this->bytes());
    return internal::nets::ipv6_is_site_local.contains(address);
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORKS_HPP
