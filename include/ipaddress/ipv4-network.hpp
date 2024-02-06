#ifndef IPADDRESS_IPV4_NETWORK_HPP
#define IPADDRESS_IPV4_NETWORK_HPP

#include "ipv4-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv4_network_base : public base_v4<ipv4_network_base> {
public:
    using ip_address_type = ipv4_address;

protected:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_type remove_scope_id(const ip_address_type& address) IPADDRESS_NOEXCEPT {
        return address;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<ipv4_network_base> remove_scope_id(const ip_network_base<ipv4_network_base>& network) IPADDRESS_NOEXCEPT {
        return network;
    }
    
#ifndef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network_base<ipv4_network_base> operator""_ipv4_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT;

#endif // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
}; // ipv4_network_base

using ipv4_network = ip_network_base<ipv4_network_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ipv4_network operator""_ipv4_net() IPADDRESS_NOEXCEPT {
        return ipv4_network::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv4_network operator""_ipv4_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_network::_max_string_len * 2 + 1 && "litteral string is too long");
        char str[ipv4_network::_max_string_len * 2 + 2] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_network::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_NETWORK_HPP
