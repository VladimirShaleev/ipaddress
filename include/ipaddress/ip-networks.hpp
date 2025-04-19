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

    static const ipv4_network ipv4_private_networks_exceptions[];
    static const ipv6_network ipv6_private_networks_exceptions[];

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
        ipv4_network::parse("192.0.0.0/24"),
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
        ipv6_network::parse("64:ff9b:1::/48"),
        ipv6_network::parse("100::/64"),
        ipv6_network::parse("2001::/23"),
        ipv6_network::parse("2001:db8::/32"),
        ipv6_network::parse("2002::/16"),
        ipv6_network::parse("fc00::/7"),
        ipv6_network::parse("fe80::/10")
    };

// Private networks exceptions
#if __cpp_constexpr >= 201304L
static constexpr ipv4_network
#else
template <typename T> const ipv4_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv4_private_networks_exceptions[] = {
        ipv4_network::parse("192.0.0.9/32"),
        ipv4_network::parse("192.0.0.10/32")
    };

#if __cpp_constexpr >= 201304L
static constexpr ipv6_network
#else
template <typename T> const ipv6_network networks<T>::
#endif
    // NOLINTNEXTLINE(cert-err58-cpp): for C++11
    ipv6_private_networks_exceptions[] = {
        ipv6_network::parse("2001:1::1/128"),
        ipv6_network::parse("2001:1::2/128"),
        ipv6_network::parse("2001:3::/32"),
        ipv6_network::parse("2001:4:112::/48"),
        ipv6_network::parse("2001:20::/28"),
        ipv6_network::parse("2001:30::/28")
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
constexpr ipv4_network networks<T>::ipv4_private_networks_exceptions[];

template <typename T>
constexpr ipv6_network networks<T>::ipv6_private_networks_exceptions[];

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

template <typename T, int N>
IPADDRESS_NODISCARD constexpr IPADDRESS_FORCE_INLINE int array_size(const T (&)[N]) IPADDRESS_NOEXCEPT {
    return N;
}

template <typename>
struct props;

template <>
struct props<ipv4_network> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private(const ipv4_network& net) IPADDRESS_NOEXCEPT {
        auto ip_private = false;
        const auto& address = net.network_address();
        const auto broadcast = net.broadcast_address();
        constexpr auto count = array_size(nets::ipv4_private_networks);
        for (int i = 0; i < count; ++i) {
            if (nets::ipv4_private_networks[i].contains(address) && nets::ipv4_private_networks[i].contains(broadcast)) {
                ip_private = true;
                break;
            }
        }
        if (ip_private) {
            constexpr auto countExceptions = array_size(nets::ipv4_private_networks_exceptions);
            for (int i = 0; i < countExceptions; ++i) {
                if (nets::ipv4_private_networks_exceptions[i].contains(address) || nets::ipv4_private_networks_exceptions[i].contains(broadcast)) {
                    ip_private = false;
                    break;
                }
            }
        }
        return ip_private;
    }
    
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global(const ipv4_network& net) IPADDRESS_NOEXCEPT {
        const auto& network = nets::ipv4_is_public_network;
        const auto& address = net.network_address();
        const auto broadcast = net.broadcast_address();
        return !(network.contains(address) && network.contains(broadcast)) && !is_private(net);
    }
};

template <>
struct props<ipv6_network> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private(const ipv6_network& net) IPADDRESS_NOEXCEPT {
        auto ip_private = false;
        const auto& address = net.network_address();
        const auto broadcast = net.broadcast_address();
        constexpr auto count = array_size(nets::ipv6_private_networks);
        for (int i = 0; i < count; ++i) {
            if (nets::ipv6_private_networks[i].contains(address) && nets::ipv6_private_networks[i].contains(broadcast)) {
                ip_private = true;
                break;
            }
        }
        if (ip_private) {
            constexpr auto countExceptions = array_size(nets::ipv6_private_networks_exceptions);
            for (int i = 0; i < countExceptions; ++i) {
                if (nets::ipv6_private_networks_exceptions[i].contains(address) || nets::ipv6_private_networks_exceptions[i].contains(broadcast)) {
                    ip_private = false;
                    break;
                }
            }
        }
        return ip_private;
    }
    
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global(const ipv6_network& net) IPADDRESS_NOEXCEPT {
        return !is_private(net);
    }
};

template <>
struct props<ipv4_address> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        auto ip_private = false;
        constexpr auto count = array_size(nets::ipv4_private_networks);
        for (int i = 0; i < count; ++i) {
            if (nets::ipv4_private_networks[i].contains(ip)) {
                ip_private = true;
                break;
            }
        }
        if (ip_private) {
            constexpr auto countExceptions = array_size(nets::ipv4_private_networks_exceptions);
            for (int i = 0; i < countExceptions; ++i) {
                if (nets::ipv4_private_networks_exceptions[i].contains(ip)) {
                    ip_private = false;
                    break;
                }
            }
        }
        return ip_private;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        return !nets::ipv4_is_public_network.contains(ip) && !is_private(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv4_is_multicast.contains(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv4_reserved_network.contains(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv4_is_loopback.contains(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local(const ipv4_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv4_is_link_local.contains(ip);
    }
};

template <>
struct props<ipv6_address> {
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        const auto ipv4 = ip.ipv4_mapped();
        if (ipv4) {
            return ipv4->is_private();
        }

        auto ip_private = false;
        constexpr auto count = array_size(nets::ipv6_private_networks);
        for (int i = 0; i < count; ++i) {
            if (nets::ipv6_private_networks[i].contains(ip)) {
                ip_private = true;
                break;
            }
        }
        if (ip_private) {
            constexpr auto countExceptions = array_size(nets::ipv6_private_networks_exceptions);
            for (int i = 0; i < countExceptions; ++i) {
                if (nets::ipv6_private_networks_exceptions[i].contains(ip)) {
                    ip_private = false;
                    break;
                }
            }
        }
        return ip_private;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        const auto ipv4 = ip.ipv4_mapped();
        if (ipv4) {
            return ipv4->is_global();
        }
        return !is_private(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv6_is_multicast.contains(ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        constexpr auto count = array_size(nets::ipv6_reserved_networks);
        for (int i = 0; i < count; ++i) {
            if (nets::ipv6_reserved_networks[i].contains(ip)) {
                return true;
            }
        }
        return false;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        const auto& b = ip.bytes();
        return b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0 
            && b[4] == 0 && b[5] == 0 && b[6] == 0 && b[7] == 0 
            && b[8] == 0 && b[9] == 0 && b[10] == 0 && b[11] == 0 
            && b[12] == 0 && b[13] == 0 && b[14] == 0 && b[15] == 1;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local(const ipv6_address& ip) IPADDRESS_NOEXCEPT {
        return nets::ipv6_is_link_local.contains(ip);
    }
};

} // namespace IPADDRESS_NAMESPACE::internal

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_network_base<Base>::is_private() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_network_base<Base>>::is_private(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_network_base<Base>::is_global() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_network_base<Base>>::is_global(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_private() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_private(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_global() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_global(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_multicast() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_multicast(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_reserved() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_reserved(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_loopback() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_loopback(*this);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ip_address_base<Base>::is_link_local() const IPADDRESS_NOEXCEPT {
    return internal::props<ip_address_base<Base>>::is_link_local(*this);
}

IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool ipv6_address_base::is_site_local() const IPADDRESS_NOEXCEPT {
    ipv6_address address(bytes());
    return internal::nets::ipv6_is_site_local.contains(address);
}

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORKS_HPP
