#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "base-v4.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv4_address_base : public base_v4<ipv4_address_base> {
public:
    using base_type = typename base_v4<ipv4_address_base>::base_type;

    template <uint32_t Ip>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> from_uint32() IPADDRESS_NOEXCEPT {
        return from_uint32(Ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> from_uint32(uint32_t ip) IPADDRESS_NOEXCEPT {
        return ip_from_uint32(ip);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR uint32_t to_uint32() const IPADDRESS_NOEXCEPT {
        return ip_to_uint32(_bytes);
    }

    IPADDRESS_NODISCARD explicit IPADDRESS_CONSTEXPR operator uint32_t() const IPADDRESS_NOEXCEPT {
        return to_uint32();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _bytes;
    }

protected:
    IPADDRESS_CONSTEXPR ipv4_address_base() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR explicit ipv4_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _bytes(bytes) {
    }

    static IPADDRESS_CONSTEXPR void swap(ip_address_base<ipv4_address_base>& lhs, ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        lhs._bytes.swap(rhs._bytes);
    }

    static IPADDRESS_CONSTEXPR std::size_t hash(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return calc_hash(0, size_t(bytes[0]), size_t(bytes[1]), size_t(bytes[2]), size_t(bytes[3]));
    }

    static IPADDRESS_CONSTEXPR bool equals(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes == rhs._bytes;
    }

    static IPADDRESS_CONSTEXPR bool less(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes < rhs._bytes;
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    static IPADDRESS_CONSTEXPR std::strong_ordering compare(const ip_address_base<ipv4_address_base>& lhs, const ip_address_base<ipv4_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._bytes <=> rhs._bytes;
    }

#endif // IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    base_type _bytes{};

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    friend consteval ip_address_base<ipv4_address_base> operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT;

#else // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    friend IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> operator""_ipv4(const char* address, std::size_t size) IPADDRESS_NOEXCEPT;
    friend IPADDRESS_CONSTEXPR ip_address_base<ipv4_address_base> operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT;

#endif // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
}; // ipv4_address_base

using ipv4_address = ip_address_base<ipv4_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    inline consteval ipv4_address operator""_ipv4() IPADDRESS_NOEXCEPT {
        return ipv4_address::parse<FixedString>();
    }

    inline consteval ipv4_address operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT {
        assert(value <= ipv4_address::_all_ones && "litteral integer is too long");
        return ipv4_address::from_uint32(uint32_t(value));
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    inline IPADDRESS_CONSTEXPR ipv4_address operator""_ipv4(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv4_address::_max_string_len && "litteral string is too long");
        char str[ipv4_address::_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv4_address::parse(str);
    }

    inline IPADDRESS_CONSTEXPR ipv4_address operator""_ipv4(unsigned long long value) IPADDRESS_NOEXCEPT {
        assert(value <= ipv4_address::_all_ones && "litteral integer is too long");
        return ipv4_address::from_uint32(uint32_t(value));
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_ADDRESS_HPP
