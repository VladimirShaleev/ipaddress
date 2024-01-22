#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
public:
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
    static IPADDRESS_CONSTEXPR size_t size = 4;

    using base_type = byte_array_type<size>;

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
    
    static std::string ip_to_string(const base_type& bytes) {
        std::ostringstream res;
        res << size_t(bytes[0]) << '.'
            << size_t(bytes[1]) << '.' 
            << size_t(bytes[2]) << '.' 
            << size_t(bytes[3]);
        return res.str();
    }

    static IPADDRESS_CONSTEXPR bool compare_scope_id(const ip_address_base<base_v4>& lhs, const ip_address_base<base_v4>& rhs) IPADDRESS_NOEXCEPT {
        return 0;
    }
};

using ipv4_address = ip_address_base<base_v4>;

}

#endif
