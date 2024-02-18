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

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.prefixlen() : _ipv_net.ipv6.prefixlen();
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address network_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.network_address()) : ip_address(_ipv_net.ipv6.network_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address broadcast_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.broadcast_address()) : ip_address(_ipv_net.ipv6.broadcast_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address netmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.netmask()) : ip_address(_ipv_net.ipv6.netmask());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address hostmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.hostmask()) : ip_address(_ipv_net.ipv6.hostmask());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_multicast() : _ipv_net.ipv6.is_multicast();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_private() : _ipv_net.ipv6.is_private();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_global() : _ipv_net.ipv6.is_global();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_reserved() : _ipv_net.ipv6.is_reserved();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_loopback() : _ipv_net.ipv6.is_loopback();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_link_local() : _ipv_net.ipv6.is_link_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_unspecified() : _ipv_net.ipv6.is_unspecified();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT {
        return is_v4() ? false : _ipv_net.ipv6.is_site_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.size() : _ipv_net.ipv6.size();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.hash() : _ipv_net.ipv6.hash();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool contains(const ip_address& address) const IPADDRESS_NOEXCEPT {
        if (_version == address.version()) {
            return is_v4() ? _ipv_net.ipv4.contains(address.v4().value()) : _ipv_net.ipv6.contains(address.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool overlaps(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.overlaps(other.v4().value()) : _ipv_net.ipv6.overlaps(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool subnet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.subnet_of(other.v4().value()) : _ipv_net.ipv6.subnet_of(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool supernet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.supernet_of(other.v4().value()) : _ipv_net.ipv6.supernet_of(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_network> v4() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            auto net = _ipv_net.ipv4;
            return optional<ipv4_network>(std::move(net));
        }
        return optional<ipv4_network>();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv6_network> v6() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            auto net = _ipv_net.ipv6;
            return optional<ipv6_network>(std::move(net));
        }
        return optional<ipv6_network>();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv4_network& net4) IPADDRESS_NOEXCEPT : _ipv_net(net4), _version(ip_version::V4) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv6_network& net6) IPADDRESS_NOEXCEPT : _ipv_net(net6), _version(ip_version::V6) {
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network from_address(
        const ip_address& address, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), prefixlen, strict));
    }

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

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return is_v4() ? _ipv_net.ipv4.to_string(fmt) : _ipv_net.ipv6.to_string(fmt);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_network& net) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = net;
        net = tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return is_v4() ? _ipv_net.ipv4.to_string() : _ipv_net.ipv6.to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version != rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 == rhs._ipv_net.ipv4) : (_ipv_net.ipv6 == rhs._ipv_net.ipv6);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> rhs._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? (_ipv_net.ipv4 <=> rhs._ipv_net.ipv4) : (_ipv_net.ipv6 <=> rhs._ipv_net.ipv6);
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version < rhs._version) {
            return true;
        }
        if (_version > rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 < rhs._ipv_net.ipv4) : (_ipv_net.ipv6 < rhs._ipv_net.ipv6);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
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

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_ANY_NETWORK_HPP
