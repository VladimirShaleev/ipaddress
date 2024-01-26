#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "base-v4.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv4_address_base : public base_v4<ipv4_address_base> {
public:
    template <uint32_t Ip>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> from_uint32() IPADDRESS_NOEXCEPT {
        return from_uint32(Ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> from_uint32(uint32_t ip) IPADDRESS_NOEXCEPT {
        return ip_from_uint32(ip);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR uint32_t to_uint32() const IPADDRESS_NOEXCEPT {
        const auto& ipv4 = *static_cast<const ip_address_base<ipv4_address_base>*>(this);
        const auto& bytes = ipv4.bytes();
        return ip_to_uint32(bytes);
    }

    IPADDRESS_NODISCARD explicit IPADDRESS_CONSTEXPR operator uint32_t() const IPADDRESS_NOEXCEPT {
        return to_uint32();
    }
}; // ipv4_address_base

using ipv4_address = ip_address_base<ipv4_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    inline consteval ipv4_address operator""_ipv4() IPADDRESS_NOEXCEPT {
        return ipv4_address::parse<FixedString>();
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

inline IPADDRESS_CONSTEXPR ipv4_address operator""_ipv4(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
    assert(size <= 15 && "litteral string is too long");
    char str[16] = {};
    for (size_t i = 0; i < size; ++i) {
        str[i] = address[i];
    }
    return ipv4_address::parse(str);
}

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_ADDRESS_HPP
