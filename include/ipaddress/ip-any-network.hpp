#ifndef IPADDRESS_IP_ANY_NETWORK_HPP
#define IPADDRESS_IP_ANY_NETWORK_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"
#include "ip-any-address.hpp"

namespace IPADDRESS_NAMESPACE {

class ip_network {
public:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return _version;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv_net.ipv4.size() : _ipv_net.ipv6.size();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv4_network& net4) IPADDRESS_NOEXCEPT : _ipv_net(net4), _version(ip_version::V4) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv6_network& net6) IPADDRESS_NOEXCEPT : _ipv_net(net6), _version(ip_version::V6) {
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network from_address(
        const ip_address& address, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), prefixlen, strict));
    }

    static IPADDRESS_CONSTEXPR ip_network from_address(
        const ip_address& address, 
        error_code& code, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), code, prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), code, prefixlen, strict));
    }

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

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_ANY_NETWORK_HPP
