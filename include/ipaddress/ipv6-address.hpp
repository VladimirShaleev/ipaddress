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
    static IPADDRESS_CONSTEXPR std::array<fixed_string<4>, max_parts> split_parts(Iter begin, Iter end, int& parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        std::array<char[5], max_parts> parts = {};
        char last_part[16] = {};

        int index = 0;
        int symbol = 0;

        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (index >= max_parts) {
                error = error_code::MOST_8_COLONS_PERMITTED;
                return empty_parts;
            }
            if (c != ':') {
                if (symbol > 15) {
                    error = error_code::PART_IS_MORE_4_CHARS;
                    return empty_parts;
                }
                last_part[symbol++] = c;
                last_part[symbol] = '\0';
            } else {
                if (symbol > 4) {
                    error = error_code::PART_IS_MORE_4_CHARS;
                    return empty_parts;
                }

                auto& current_part = parts[index++];
                symbol = 0;

                current_part[0] = last_part[0];
                current_part[1] = last_part[1];
                current_part[2] = last_part[2];
                current_part[3] = last_part[3];
            }
        }
    
        if (index < min_parts) {
            error = error_code::LEAST_3_PARTS;
            return empty_parts;
        }

        if (index >= max_parts) {
            error = error_code::MOST_8_COLONS_PERMITTED;
            return empty_parts;
        }
        
        auto has_ipv4 = false;

        for (auto i = 0; i < symbol; ++i) {
            if (last_part[i] == '.') {
                has_ipv4 = true;
                break;
            }
        }

        if (has_ipv4) {
            if (index + 1 >= max_parts) {
                error = error_code::MOST_8_COLONS_PERMITTED;
                return empty_parts;
            }

            auto ipv4 = ipv4_address::parse(last_part, error).ip();

            if (error != error_code::NO_ERROR) {
                return empty_parts;
            }

            to_hex(uint16_t((ipv4 >> 16) & 0xFFFF), parts[index++]);
            to_hex(uint16_t(ipv4 & 0xFFFF), parts[index++]);
        } else {
            if (symbol > 4) {
                error = error_code::PART_IS_MORE_4_CHARS;
                return empty_parts;
            }
            
            auto& current_part = parts[index++];
            current_part[0] = last_part[0];
            current_part[1] = last_part[1];
            current_part[2] = last_part[2];
            current_part[3] = last_part[3];
        }

        parts_count = index;
        return {
            make_fixed_string(parts[0]),
            make_fixed_string(parts[1]),
            make_fixed_string(parts[2]),
            make_fixed_string(parts[3]),
            make_fixed_string(parts[4]),
            make_fixed_string(parts[5]),
            make_fixed_string(parts[6]),
            make_fixed_string(parts[7])};
    }

    static IPADDRESS_CONSTEXPR std::tuple<size_t, size_t, size_t> get_parts_bound(const std::array<fixed_string<4>, max_parts>& parts, int parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        int skip = 0;
        for (auto i = 0; i < parts_count - 1; ++i) {
            if (parts[i].empty()) {
                if (skip) {
                    error = error_code::MOST_ONE_DOUBLE_COLON_PERMITTED;
                    return { 0, 0, 0 };
                }
                skip = i;
            }
        }

        if (skip) {
            auto parts_hi = skip;
            auto parts_lo = parts_count - skip - 1;

            if (parts[0].empty()) {
                if (--parts_hi) {
                    error = error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                    return { 0, 0, 0 };
                }
            }

            if (parts[parts_count - 1].empty()) {
                if (--parts_lo) {
                    error = error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                    return { 0, 0, 0 };
                }
            }

            const auto parts_skipped = max_parts - (parts_hi + parts_lo);
            
            if (parts_skipped < 0) {
                error = error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            return { parts_hi, parts_lo, parts_skipped };
        } else {
            if (parts_count != max_parts) {
                error = error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            if (parts[0].empty()) {
                error = error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            if (parts.back().empty()) {
                error = error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            return { parts.size(), 0, 0 };
        }
    }

    static IPADDRESS_CONSTEXPR BaseType parse_parts(const std::array<fixed_string<4>, max_parts>& parts, int parts_count, size_t hi, size_t lo, size_t skipped, error_code& error) IPADDRESS_NOEXCEPT {
        // BaseType ip(0);

        for (size_t i = 0; i < hi; ++i) {
            //ip <<= 16;
            /*ip |=*/ parse_part(parts[i], error);

            if (error != error_code::NO_ERROR) {
                return {};
            }
        }
        // ip <<= 16 * skipped;

        for (size_t i = lo; i > 0; --i) {
            //ip <<= 16;
            /*ip |=*/ parse_part(parts[i], error);

            if (error != error_code::NO_ERROR) {
                return {};
            }
        }
        return {}; // ip
    }

    static IPADDRESS_CONSTEXPR uint16_t parse_part(const fixed_string<4>& part, error_code& error) IPADDRESS_NOEXCEPT {
        uint16_t value = 0;
        for (size_t i = 0; i < part.size(); ++i) {
            const auto c = part[i];
            const auto power = pow16(part.size() - i - 1);
            if (c >= '0' && c <= '9') {
                value += (c - '0') * power;
            } else if(c >= 'A' && c <= 'F') {
                value += (c - 55) * power;
            } else if (c >= 'a' && c <= 'f') {
                value += (c - 87) * power;
            } else {
                error = error_code::PART_HAS_INVALID_SYMBOL;
                return 0;
            }
        }
        return value;
    }

    static IPADDRESS_CONSTEXPR uint16_t pow16(int power) IPADDRESS_NOEXCEPT {
        switch (power) {
            case 0: return 1;
            case 1: return 16;
            case 2: return 16 * 16;
            case 3: return 16 * 16 * 16;
            default:
                std::exit(1);
                return 0;
        }
    }

    static IPADDRESS_CONSTEXPR void to_hex(uint16_t value, char(&result)[5]) {
        char digits[] = "0123456789abcdef";
        for (auto i = 0, j = (4 - 1) * 4; i < 4; ++i, j -= 4) {
            result[i] = digits[(value >> j) & 0x0f];
            result[i + 1] = '\0';
        }
    }

    static IPADDRESS_CONSTEXPR std::array<fixed_string<4>, max_parts> empty_parts = {
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0")};
};

using ipv6_address = ip_address_base<base_v6>;

}

#endif
