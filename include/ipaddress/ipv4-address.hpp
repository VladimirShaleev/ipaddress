#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
public:
    static IPADDRESS_CONSTEXPR version version = version::V4;

    static IPADDRESS_CONSTEXPR size_t size = 4;

    static IPADDRESS_CONSTEXPR uint32_t all_ones = std::numeric_limits<uint32_t>::max();

    static IPADDRESS_CONSTEXPR size_t max_string_len = 15;

    static IPADDRESS_CONSTEXPR size_t max_prefixlen = size * 8;

    using base_type = byte_array_type<size>;

    template <uint32_t Ip>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<base_v4> from_uint32() IPADDRESS_NOEXCEPT {
        return from_uint32(Ip);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<base_v4> from_uint32(uint32_t ip) IPADDRESS_NOEXCEPT {
        ip = is_little_endian() ? swap_bytes(ip) : ip;
        return ip_address_base<base_v4>({
            uint8_t(ip & 0xFF),
            uint8_t((ip >> 8) & 0xFF),
            uint8_t((ip >> 16) & 0xFF),
            uint8_t((ip >> 24) & 0xFF) });
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR uint32_t to_uint32() const IPADDRESS_NOEXCEPT {
        const auto& ipv4 = *static_cast<const ip_address_base<base_v4>*>(this);
        const auto& bytes = ipv4.bytes();
        const auto ip = 
            (uint32_t(bytes[3]) << 24) | 
            (uint32_t(bytes[2]) << 16) | 
            (uint32_t(bytes[1]) << 8) | 
            (uint32_t(bytes[0]));
        return is_little_endian() ? swap_bytes(ip) : ip;
    }

    explicit IPADDRESS_CONSTEXPR operator uint32_t() const IPADDRESS_NOEXCEPT {
        return to_uint32();
    }

protected:
    template <typename Iter>
    static IPADDRESS_CONSTEXPR ip_address_base<base_v4> ip_from_string(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        if (begin == end) {
            code = error_code::EMPTY_ADDRESS;
            return {};
        }

        base_type octets = {};
        char first_symbol = '\0';
        int digits = 0;
        int octet = 0;
        
        index = 0;
        code = error_code::NO_ERROR;
    
        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (index >= 4) {
                code = error_code::EXPECTED_4_OCTETS;
                return {};
            }
            if (c >= '0' && c <= '9') {
                if (digits > 0 && first_symbol == '0') {
                    code = error_code::LEADING_0_ARE_NOT_PERMITTED;
                    return {};
                } else if (digits == 0) {
                    first_symbol = c;
                }
                auto d = c - '0';
                octet = octet * 10 + d;
                ++digits;
                if (digits > 3) {
                    code = error_code::OCTET_MORE_3_CHARACTERS;
                    return {};
                }
            } else if (c == '.' && digits > 0) {
                if (octet > 255) {
                    code = error_code::OCTET_EXCEEDED_255;
                    return {};
                }
                octets[index++] = uint8_t(octet & 0xFF);
                digits = 0;
                octet = 0;
            } else if (c == '.') {
                code = error_code::EMPTY_OCTET;
                return {};
            } else {
                code = error_code::OCTET_HAS_INVALID_SYMBOL;
                return {};
            }
        }
        if (index != 3) {
            code = error_code::EXPECTED_4_OCTETS;
            return {};
        }
        if (digits == 0) {
            code = error_code::EMPTY_OCTET;
            return {};
        }
        if (octet > 255) {
            code = error_code::OCTET_EXCEEDED_255;
            return {};
        }
        octets[index] = uint8_t(octet & 0xFF);
        return ip_address_base<base_v4>(octets);
    }

    static IPADDRESS_CONSTEXPR ip_address_base<base_v4> ip_from_prefix(size_t prefixlen) {
        return ip_address_base<base_v4>::from_uint32(all_ones & (all_ones >> prefixlen));
    }

    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::pair<ip_address_base<base_v4>, size_t> parse_netmask(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        size_t prefixlen = 0;
        auto is_value = true;
        for (auto it = begin; it != end; ++it) {
            if (*it >= '0' && *it <= '9') {
                prefixlen = prefixlen * 10 + (*it - '0');
            } else {
                is_value = false;
                break;
            }
        }
        if (is_value) {
            if (prefixlen > max_prefixlen) {
                code = error_code::INVALID_NETMASK;
                return std::make_pair(ip_address_base<base_v4>(), 0);
            }
        } else {
            auto ip = ip_from_string(begin, end, code, index).to_uint32();
            if (code != error_code::NO_ERROR) {
                code = error_code::INVALID_NETMASK;
                return std::make_pair(ip_address_base<base_v4>(), 0);
            }

            prefixlen = prefix_from_ip_uint32(ip, code);
            if (code != error_code::NO_ERROR) {
                ip = ip ^ all_ones;
                code = error_code::NO_ERROR;
                prefixlen = prefix_from_ip_uint32(ip, code);
                if (code != error_code::NO_ERROR) {
                    return std::make_pair(ip_address_base<base_v4>(), 0);
                }
            }
        }
        return std::make_pair(ip_from_prefix(prefixlen), prefixlen);
    }

    static IPADDRESS_CONSTEXPR size_t count_righthand_zero_bits(uint32_t number, uint32_t bits) IPADDRESS_NOEXCEPT {
        if (number == 0) {
            return bits;
        } else {
            number = (~number & (number - 1));
            auto count = 0;
            while (number != 0) {
                count += number & 0x1;
                number >>= 1;
            }
            if (bits < count) {
                return bits;
            } else {
                return count;
            }
        }
    }

    static IPADDRESS_CONSTEXPR size_t prefix_from_ip_uint32(uint32_t ip, error_code& code) IPADDRESS_NOEXCEPT {
        auto trailing_zeroes = count_righthand_zero_bits(ip, max_prefixlen);
        auto prefixlen = max_prefixlen - trailing_zeroes;
        auto leading_ones = ip >> trailing_zeroes;
        auto all_ones = (1 << prefixlen) - 1;
        if (leading_ones != all_ones) {
            code = error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES;
            return 0;
        }
        return prefixlen;
    }
    
    static std::string ip_to_string(const base_type& bytes, format fmt) {
        std::ostringstream res;
        res << size_t(bytes[0]) << '.'
            << size_t(bytes[1]) << '.' 
            << size_t(bytes[2]) << '.' 
            << size_t(bytes[3]);
        return res.str();
    }

    std::string reverse_pointer(const base_type& bytes) const {
        return from_uint32(swap_bytes(to_uint32())).to_string() + ".in-addr.arpa";
    }

    IPADDRESS_CONSTEXPR size_t hash_scope_id() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    static IPADDRESS_CONSTEXPR bool compare_scope_id(const ip_address_base<base_v4>& lhs, const ip_address_base<base_v4>& rhs) IPADDRESS_NOEXCEPT {
        return 0;
    }
};

using ipv4_address = ip_address_base<base_v4>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
template <fixed_string FixedString>
inline consteval ipv4_address operator""_ipv4() IPADDRESS_NOEXCEPT {
    return ipv4_address::parse<FixedString>();
}
#endif

inline IPADDRESS_CONSTEXPR ipv4_address operator""_ipv4(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
    assert(size <= 15 && "litteral string is too long");
    char str[16] = {};
    for (size_t i = 0; i < size; ++i) {
        str[i] = address[i];
    }
    return ipv4_address::parse(str);
}

}

#endif
