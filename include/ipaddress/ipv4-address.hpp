#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"
#include "endian.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
protected:
    typedef uint32_t BaseType;

    template <typename FixedString>
    static IPADDRESS_CONSTEXPR BaseType ip_from_string(const FixedString& address, error_code& code, int& octet) IPADDRESS_NOEXCEPT {
        auto octets = parse_octets(address.begin(), address.end(), code, octet);
        return is_little_endian() ? swap_bytes(octets) : octets;
    }

    // static BaseType ip_from_bytes() {

    // }

private:
    template <typename Iter>
    static IPADDRESS_CONSTEXPR BaseType parse_octets(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        if (begin == end) {
            code = error_code::EMPTY_ADDRESS;
            return 0;
        }
        BaseType octets = 0;
        int digits = 0;
        int octet = 0;
        index = 0;
        code = error_code::NO_ERROR;
        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (c == '\0') {
                break;
            }
            if (index >= 4) {
                code = error_code::EXPECTED_4_OCTETS;
                return 0;
            }
            if (c >= '0' && c <= '9') {
                auto d = c - '0';
                octet = octet * 10 + d;
                ++digits;
                if (digits > 3) {
                    code = error_code::OCTET_MORE_3_CHARACTERS;
                    return 0;
                }
            } else if (c == '.' && digits > 0) {
                if (octet > 255) {
                    code = error_code::OUT_OF_RANGE_OCTET;
                    return 0;
                }
                digits = 0;
                octets |= (octet & 0xFF) << (index * 8);
                octet = 0;
                ++index;
            } else if (c == '.') {
                code = error_code::EMPTY_OCTET;
                return 0;
            } else {
                code = error_code::INVALID_OCTET_SYMBOL;
                return 0;
            }
        }
        if (index != 3) {
            code = error_code::EXPECTED_4_OCTETS;
            return 0;
        }
        if (digits == 0) {
            code = error_code::EMPTY_OCTET;
            return 0;
        }
        if (octet > 255) {
            code = error_code::OUT_OF_RANGE_OCTET;
            return 0;
        }
        octets |= (octet & 0xFF) << (index * 8);
        return octets;
    }
};

using ipv4_address = ip_address_base<base_v4>;

}

#endif
