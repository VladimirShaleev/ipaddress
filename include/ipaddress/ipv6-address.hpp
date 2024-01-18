#ifndef IPADDRESS_IPV6_ADDRESS_HPP
#define IPADDRESS_IPV6_ADDRESS_HPP

#include "ipv4-address.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v6 {
protected:
    typedef std::array<uint8_t, 16> BaseType;

    static IPADDRESS_CONSTEXPR size_t min_parts = 3;
    static IPADDRESS_CONSTEXPR size_t max_parts = 8;

    template <typename FixedString>
    static IPADDRESS_CONSTEXPR BaseType ip_from_string(const FixedString& address, error_code& code, int& octet) IPADDRESS_NOEXCEPT {
        if (address.empty()) {
            code = error_code::EMPTY_ADDRESS;
            return {};
        }

        auto parts_count = 0;
        const auto parts = split_parts(address.begin(), address.end(), parts_count, code);

        if (code != error_code::NO_ERROR) {
            return {};
        }

        const auto result = get_parts_bound(parts, parts_count, code);

        if (code != error_code::NO_ERROR) {
            return {};
        }

        return parse_parts(parts, parts_count, std::get<0>(result), std::get<1>(result), std::get<2>(result), code);
    }

private:
    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::array<fixed_string<45>, max_parts> split_parts(Iter begin, Iter end, int& parts_count, error_code& code) IPADDRESS_NOEXCEPT {
        std::array<fixed_string<45>, max_parts> parts = {
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"),
            make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0")};

        int index = 0;
        int symbol = 0;
        char str[46] = {};

        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (c == '\0') {
                break;
            }
            if (index >= max_parts) {
                // error
                // return 
            }
            if (c != ':') {
                if (symbol > 45) {
                    // error
                }
                str[symbol++] = c;
                str[symbol] = '\0';
            } else {
                parts[index++] = make_fixed_string(str);
                symbol = 0;
            }
        }
    
        if (index < min_parts) {
            // error
        }

        if (index >= max_parts) {
            // error
            // return 
        }
        
        if (symbol > 45) {
            // error
        }

        parts[index] = make_fixed_string(str);

        auto& last_part = parts[index];
        auto has_ipv4 = false;

        for (auto it = last_part.begin(); it != last_part.end(); ++it) {
            if (*it == '\0') {
                break;
            }
            if (*it == '.') {
                has_ipv4 = true;
                break;
            }
        }

        if (has_ipv4) {
            if (index + 1 >= max_parts) {
                // error
                // return 
            }

            char address[46]={};
            for (auto it = last_part.begin(); it != last_part.end(); ++it) {
                address[it - last_part.begin()] = *it;
            }
            auto ipv4 = ipv4_address::parse(address, code).ip();

            if (code != error_code::NO_ERROR) {
                // error
            }

            parts[index++] = to_hex(uint16_t((ipv4 >> 16) & 0xFFFF));
            parts[index] = to_hex(uint16_t(ipv4 & 0xFFFF));
        }

        parts_count = index;
        return parts;
    }

    static IPADDRESS_CONSTEXPR std::tuple<size_t, size_t, size_t> get_parts_bound(const std::array<fixed_string<45>, max_parts>& parts, int parts_count, error_code& code) IPADDRESS_NOEXCEPT {
        int skip = 0;
        for (auto i = 0; i < parts_count - 1; ++i) {
            if (parts[i].at(0) == '\0') {
                if (skip) {
                    // error
                }
                skip = i;
            }
        }

        if (skip) {
            auto parts_hi = skip;
            auto parts_lo = parts_count - skip - 1;

            if (parts[0].at(0) == '\0') {
                if (--parts_hi) {
                    // error
                }
            }

            if (parts[parts_count - 1].at(0) == '\0') {
                if (--parts_lo) {
                    // error
                }
            }

            const auto parts_skipped = max_parts - (parts_hi + parts_lo);
            
            if (parts_skipped < 0) {
                // error
            }

            return { parts_hi, parts_lo, parts_skipped };
        } else {
            if (parts_count != max_parts) {
                // error
            }

            if (parts[0].at(0) == '\0') {
                // error
            }

            if (parts.back().at(0) == '\0') {
                // error
            }

            return { parts.size(), 0, 0 };
        }
    }

    static IPADDRESS_CONSTEXPR BaseType parse_parts(const std::array<fixed_string<45>, max_parts>& parts, int parts_count, size_t hi, size_t lo, size_t skipped, error_code& code) IPADDRESS_NOEXCEPT {
        return {};        
    }

    static IPADDRESS_CONSTEXPR fixed_string<45> to_hex(uint16_t value) {
        char digits[] = "0123456789abcdef";
        char result[46] = {};
        for (auto i = 0, j = (4 - 1) * 4; i < 4; ++i, j -= 4) {
            result[i] = digits[(value >> j) & 0x0f];
        }
        return make_fixed_string(result);
    }
};

using ipv6_address = ip_address_base<base_v6>;

}

#endif
