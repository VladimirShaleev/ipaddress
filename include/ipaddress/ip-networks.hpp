#ifndef IPADDRESS_IP_NETWORKS_HPP
#define IPADDRESS_IP_NETWORKS_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"

namespace IPADDRESS_NAMESPACE {

template<>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv4_address::is_private() const IPADDRESS_NOEXCEPT {
    constexpr ipv4_network private_networks[] = {
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

    for (const auto& private_network : private_networks) {
        if (private_network.contains(*this)) {
            return true;
        }
    }

    return false;
}

template<>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address::is_private() const IPADDRESS_NOEXCEPT {
    auto mapped = false;
    const auto ipv4 = this->ipv4_mapped(mapped);

    if (mapped) {
        return ipv4.is_private();
    }

    constexpr ipv6_network private_networks[] = {
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

    for (const auto& private_network : private_networks) {
        if (private_network.contains(*this)) {
            return true;
        }
    }

    return false;
}

}

#endif
