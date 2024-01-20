#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
public:
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

    using base_type = std::array<uint8_t, size>;

    template <typename Iter>
    static IPADDRESS_CONSTEXPR base_type ip_from_string(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
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
        return octets;
    }
};

using ipv4_address = ip_address_base<base_v4>;

}

#endif
