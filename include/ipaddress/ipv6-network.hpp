#ifndef IPADDRESS_IPV6_NETWORK_HPP
#define IPADDRESS_IPV6_NETWORK_HPP

#include "ipv6-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv6_network_base : public base_v6<ipv6_network_base> {
public:
    using ip_address_type = ipv6_address;

#ifndef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    friend IPADDRESS_CONSTEXPR ip_network_base<ipv6_network_base> operator""_ipv6_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT;

#endif // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
}; // ipv6_network_base

using ipv6_network = ip_network_base<ipv6_network_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    inline consteval ipv6_network operator""_ipv6_net() IPADDRESS_NOEXCEPT {
        return ipv6_network::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    inline IPADDRESS_CONSTEXPR ipv6_network operator""_ipv6_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_network::_max_string_len + 4 && "litteral string is too long");
        char str[ipv6_network::_max_string_len + 5] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv6_network::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // IPADDRESS_NAMESPACE

#endif
