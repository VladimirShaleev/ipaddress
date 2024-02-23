#ifndef IPADDRESS_IP_NETWORKS_HPP
#define IPADDRESS_IP_NETWORKS_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

static IPADDRESS_CONSTEXPR ipv4_network _ipv4_private_networks[] = {
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

static IPADDRESS_CONSTEXPR ipv6_network _ipv6_private_networks[] = {
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

} // namespace IPADDRESS_NAMESPACE::internal

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_network::is_private() const IPADDRESS_NOEXCEPT {
    const auto& address = network_address();
    const auto broadcast = broadcast_address();
    for (const auto& private_network : internal::_ipv4_private_networks) {
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
    for (const auto& private_network : internal::_ipv6_private_networks) {
        if (private_network.contains(address) && private_network.contains(broadcast)) {
            return true;
        }
    }
    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_network::is_global() const IPADDRESS_NOEXCEPT {
    constexpr auto network = ipv4_network::parse("100.64.0.0/10");
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
    constexpr auto multicast_networks = ipv4_network::parse("224.0.0.0/4");
    return multicast_networks.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_multicast() const IPADDRESS_NOEXCEPT {
    constexpr auto multicast_networks = ipv6_network::parse("ff00::/8");
    return multicast_networks.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_private() const IPADDRESS_NOEXCEPT {
    for (const auto& private_network : internal::_ipv4_private_networks) {
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

    for (const auto& private_network : internal::_ipv6_private_networks) {
        if (private_network.contains(*this)) {
            return true;
        }
    }

    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_global() const IPADDRESS_NOEXCEPT {
    constexpr auto public_network = ipv4_network::parse("100.64.0.0/10");
    return !public_network.contains(*this) && !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_global() const IPADDRESS_NOEXCEPT {
    return !is_private();
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_reserved() const IPADDRESS_NOEXCEPT {
    constexpr auto reserved_network = ipv4_network::parse("240.0.0.0/4");
    return reserved_network.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_reserved() const IPADDRESS_NOEXCEPT {
    constexpr ipv6_network reserved_networks[] = {
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

    for (const auto& reserved_network : reserved_networks) {
        if (reserved_network.contains(*this)) {
            return true;
        }
    }

    return false;
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_loopback() const IPADDRESS_NOEXCEPT {
    constexpr auto loopback_network = ipv4_network::parse("127.0.0.0/8");
    return loopback_network.contains(*this);
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
    constexpr auto link_local_network = ipv4_network::parse("169.254.0.0/16");
    return link_local_network.contains(*this);
}

template<>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_link_local() const IPADDRESS_NOEXCEPT {
    constexpr auto link_local_network = ipv6_network::parse("fe80::/10");
    return link_local_network.contains(*this);
}

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address_base::is_site_local() const IPADDRESS_NOEXCEPT {
    constexpr auto site_local_network = ipv6_network::parse("fec0::/10");
    ipv6_address address(this->bytes());
    return site_local_network.contains(address);
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORKS_HPP