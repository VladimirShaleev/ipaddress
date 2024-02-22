/**
 * @file      base-v6.hpp
 * @brief     Adds basic functionality for working with IPv6
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file provides foundational classes and functions necessary for handling IPv6 addresses,
 * including parsing, constructing, and manipulating IPv6 address representations. It serves as
 * a base for higher-level IPv6 address operations and utilities.
 */

#ifndef IPADDRESS_BASE_V6_HPP
#define IPADDRESS_BASE_V6_HPP

#include "ipv4-address.hpp"
#include "uint128.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A template class providing the base functionality for IPv6 addresses.
 * 
 * This class encapsulates the basic properties and operations for IPv6 addresses,
 * such as version identification, size, and conversion from string representations.
 * It is intended to be extended by more specialized IPv6 address classes.
 * 
 * @tparam Ext The extension type that provides additional functionality.
 */
template <typename Ext>
class base_v6 {
public:
    using base_type = byte_array_type<16>;
    using uint_type = ::IPADDRESS_NAMESPACE::uint128_t;

    static IPADDRESS_CONSTEXPR ip_version base_version = ip_version::V6;
    static IPADDRESS_CONSTEXPR size_t base_size = 16;
    static IPADDRESS_CONSTEXPR size_t base_max_string_len = 41 + IPADDRESS_IPV6_SCOPE_MAX_LENGTH;
    static IPADDRESS_CONSTEXPR size_t base_max_prefixlen = base_size * 8;

    /**
     * Retrieves the IP version of the address.
     * 
     * @return The IP version enumeration value for IPv6.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return base_version;
    }

    /**
     * Retrieves the size of the IPv6 address.
     * 
     * @return The size of the IPv6 address in bytes.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return base_size;
    }

protected:
    static IPADDRESS_CONSTEXPR size_t _min_parts = 3;
    static IPADDRESS_CONSTEXPR size_t _max_parts = 8;

    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> ip_from_string(Iter begin, Iter end, error_code& code, int& parts_count) IPADDRESS_NOEXCEPT {
        if (begin == end) {
            code = error_code::EMPTY_ADDRESS;
            return {};
        }

        auto ip_and_scope = split_scope_id(begin, end, code);
        end = ip_and_scope.first;

        if (code != error_code::NO_ERROR) {
            return {};
        }

        const auto parts = split_parts(begin, end, parts_count, code);

        if (code != error_code::NO_ERROR) {
            return {};
        }

        const auto result = get_parts_bound(parts, parts_count, code);

        if (code != error_code::NO_ERROR) {
            return {};
        }

        auto ip = ip_address_base<Ext>(parse_parts(parts, parts_count, std::get<0>(result), std::get<1>(result), std::get<2>(result), code));

        char scope_id[IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1] = {};
        for (size_t i = 0; i < ip_and_scope.second.size(); ++i) {
            scope_id[i] = ip_and_scope.second[i];
        }
        ip.set_scope_id(scope_id);

        return ip;
    }
    
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> ip_from_prefix(size_t prefixlen) {
        base_type bytes {};
        for (size_t i = 0; i < (prefixlen >> 3); ++i) {
            bytes[i] = 0xFF;
        }
        auto shift = (prefixlen - ((prefixlen >> 3) << 3));
        if (shift > 0) {
            auto byte = 0xFF ^ uint8_t(uint8_t(0xFF) >> shift);
            bytes[prefixlen >> 3] = byte;
        }
        return ip_address_base<Ext>(bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t ip_to_chars(const base_type& bytes, const fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& scope_id, format fmt, char (&result)[base_max_string_len + 1]) IPADDRESS_NOEXCEPT {
        char hextets[base_size >> 1][5] = {};
        const size_t max_hextets = base_size >> 1;
        for (size_t i = 0; i < max_hextets; ++i) {
            uint16_t value = (uint16_t(bytes[i * 2]) << 8) | uint16_t(bytes[i * 2 + 1]);
            to_hex(value, hextets[i]);
        }
        
        auto count = max_hextets;
        auto has_doublecolon_start = false;
        auto has_doublecolon_end = false;

        if (fmt == format::compact || fmt == format::compressed) {
            for (size_t i = 0; i < max_hextets; ++i) {
                auto& hextet = hextets[i];

                for (size_t h = 0; h < 3; ++h) {
                    if (hextet[0] != '0') {
                        break;
                    }
                    hextet[0] = hextet[1];
                    hextet[1] = hextet[2];
                    hextet[2] = hextet[3];
                    hextet[3] = hextet[4];
                }
            }

            if (fmt == format::compressed) {
                int best_doublecolon_len = 0;
                int best_doublecolon_start = -1;
                int doublecolon_len = 0;
                int doublecolon_start = -1;

                for (size_t i = 0; i < max_hextets; ++i) {
                    const auto& hextet = hextets[i];
                    if (hextet[0] == '0') {
                        doublecolon_len += 1;

                        if (doublecolon_start == -1) {
                            doublecolon_start = int(i);
                        }
                        if (doublecolon_len > best_doublecolon_len) {
                            best_doublecolon_len = doublecolon_len;
                            best_doublecolon_start = doublecolon_start;
                        }
                    } else {
                        doublecolon_len = 0;
                        doublecolon_start = -1;
                    }
                }

                if (best_doublecolon_len > 1) {
                    int best_doublecolon_end = best_doublecolon_start + best_doublecolon_len;
                    if (best_doublecolon_end == max_hextets) {
                        has_doublecolon_end = true;
                    }
                    hextets[best_doublecolon_start][0] = '\0';
                    for (int h = best_doublecolon_start; h < max_hextets - 1; ++h) {
                        const auto lindex = h + 1;
                        const auto rindex = h + best_doublecolon_len;
                        if (rindex >= max_hextets) {
                            break;
                        }
                        hextets[lindex][0] = hextets[rindex][0];
                        hextets[lindex][1] = hextets[rindex][1];
                        hextets[lindex][2] = hextets[rindex][2];
                        hextets[lindex][3] = hextets[rindex][3];
                    }
                    count -= best_doublecolon_len - 1;
                    
                    if (best_doublecolon_start == 0) {
                        has_doublecolon_start = true;
                    }
                }
            }
        }

        size_t offset = 0;
        if (has_doublecolon_start) {
            result[offset++] = ':';
        }
        for (size_t i = 0; i < count - 1; ++i) {
            const auto& hextet = hextets[i];
            for (size_t c = 0; hextet[c] != '\0'; ++c) {
                result[offset++] = hextet[c];
            }
            result[offset++] = ':';
        }
        const auto hextet = hextets[count - 1];
        for (size_t c = 0; hextet[c] != '\0'; ++c) {
            result[offset++] = hextet[c];
        }
        if (has_doublecolon_end) {
            result[offset++] = ':';
        }
        if (!scope_id.empty()) {
            result[offset++] = '%';
            for (const auto c : scope_id) {
                result[offset++] = c;
            }
        }
        result[offset] = '\0';
        return offset;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE std::string ip_reverse_pointer(const base_type& bytes, const fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& scope_id) {
        char result[base_max_string_len + 1] {};
        const auto len = ip_to_chars(bytes, scope_id, format::full, result);
        auto ip = std::string(result, len);
        ip.erase(std::remove(ip.begin(), ip.end(), ':'), ip.end());
        auto res = std::accumulate(std::next(ip.rbegin()), ip.rend(), std::string{ip.back()}, [](auto a, auto b) {
            return std::move(a) + '.' + b;
        });
        return res + ".ip6.arpa";
    }

    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::tuple<ip_address_base<Ext>, size_t> parse_netmask(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        size_t prefixlen = 0;
        auto has_prefixlen = false;
        for (auto it = begin; it != end; ++it) {
            has_prefixlen = true;
            const auto c = char(*it);
            if (c >= '0' && c <= '9') {
                prefixlen = prefixlen * 10 + (c - '0');
            } else {
                code = error_code::INVALID_NETMASK;
                return std::make_tuple(ip_address_base<Ext>(), 0);
            }
        }
        
        if (prefixlen > base_max_prefixlen) {
            code = error_code::INVALID_NETMASK;
            return std::make_tuple(ip_address_base<Ext>(), 0);
        }
        
        prefixlen = has_prefixlen ? prefixlen : base_max_prefixlen;

        auto netmask = ip_from_prefix(prefixlen);
        return std::make_tuple(netmask, prefixlen);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> strict_netmask(const ip_address_base<Ext>& address, const ip_address_base<Ext>& netmask, bool strict, error_code& code) IPADDRESS_NOEXCEPT {
        const auto& bytes_address = address.bytes();
        const auto& bytes_netmask = netmask.bytes();
        base_type bytes{};

        for (auto i = 0; i < base_size; ++i) {
            bytes[i] = bytes_address[i] & bytes_netmask[i];
        }

        if (bytes != bytes_address) {
            if (strict) {
                code = error_code::HAS_HOST_BITS_SET;
                return ip_address_base<Ext>();
            } else {
                return ip_address_base<Ext>(bytes);
            }
        }
        return address;
    }

private:
    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::pair<Iter, fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>> split_scope_id(Iter begin, Iter end, error_code& error) IPADDRESS_NOEXCEPT {
        char scope_id[IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1] = {};
        auto index = 0;
        Iter end_ip = begin;
        auto scope = false;
        for (auto it = begin; it != end; ++it) {
            const auto c = char(*it);
            if (!scope && c != '%') {
                end_ip = it + 1;
            } else if (scope) {
                if (index > IPADDRESS_IPV6_SCOPE_MAX_LENGTH - 1) {
                    error = error_code::SCOPE_ID_IS_TOO_LONG;
                    return std::make_pair(end_ip, make_fixed_string(scope_id));
                }
                if (c == '%' || c == '/') {
                    error = error_code::INVALID_SCOPE_ID;
                    return std::make_pair(end_ip, make_fixed_string(scope_id));
                }
                scope_id[index++] = c;
            } else {
                scope = true;
            }
        }
        if (scope && scope_id[0] == '\0') {
            error = error_code::INVALID_SCOPE_ID;
            return std::make_pair(end_ip, make_fixed_string(scope_id));
        }
        return std::make_pair(end_ip, make_fixed_string(scope_id));
    }

    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::array<fixed_string<4>, _max_parts + 1> split_parts(Iter begin, Iter end, int& parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        IPADDRESS_CONSTEXPR std::array<fixed_string<4>, _max_parts + 1> empty_parts = {
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0"),
            make_fixed_string("\0\0\0\0")};

        char parts[_max_parts + 1][5] = {};
        char last_part[17] = {};
        
        parts_count = 0;
        int symbol = 0;
        char prev_c = '\0';
        bool has_double_colon = false;

        for (auto it = begin; it != end; ++it) {
            auto c = char(*it);
            if (!has_double_colon && c == ':' && prev_c == ':') {
                has_double_colon = true;
            }
            if (parts_count > _max_parts) {
                error = has_double_colon
                    ? error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON 
                    : error_code::MOST_8_COLONS_PERMITTED;
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

                auto& current_part = parts[parts_count++];
                current_part[0] = last_part[0];
                current_part[1] = last_part[1];
                current_part[2] = last_part[2];
                current_part[3] = last_part[3];

                symbol = 0;
                last_part[0] = '\0';
            }
            prev_c = c;
        }
    
        if (parts_count > _max_parts) {
            if (parts[0][0] == '\0' && parts[1][0] != '\0') {
                error = error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
            } else if (last_part[0] == '\0') {
                error = error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
            } else {
                error = error_code::MOST_8_COLONS_PERMITTED;
            }
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
            if (parts_count + 1 >= _max_parts) {
                error = error_code::MOST_8_COLONS_PERMITTED;
                return empty_parts;
            }

            auto ipv4 = ipv4_address::parse(last_part, error).to_uint();

            if (error != error_code::NO_ERROR) {
                return empty_parts;
            }

            to_hex(uint16_t((ipv4 >> 16) & 0xFFFF), parts[parts_count++]);
            to_hex(uint16_t(ipv4 & 0xFFFF), parts[parts_count++]);
        } else {
            if (symbol > 4) {
                error = error_code::PART_IS_MORE_4_CHARS;
                return empty_parts;
            }
            
            auto& current_part = parts[parts_count++];
            current_part[0] = last_part[0];
            current_part[1] = last_part[1];
            current_part[2] = last_part[2];
            current_part[3] = last_part[3];
        }

        if (parts_count < _min_parts) {
            error = error_code::LEAST_3_PARTS;
            return empty_parts;
        }

        return {
            make_fixed_string(parts[0]),
            make_fixed_string(parts[1]),
            make_fixed_string(parts[2]),
            make_fixed_string(parts[3]),
            make_fixed_string(parts[4]),
            make_fixed_string(parts[5]),
            make_fixed_string(parts[6]),
            make_fixed_string(parts[7]),
            make_fixed_string(parts[8])};
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::tuple<size_t, size_t, size_t> get_parts_bound(const std::array<fixed_string<4>, _max_parts + 1>& parts, size_t parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        size_t skip = 0;
        for (size_t i = 1; i < parts_count - 1; ++i) {
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

            const auto parts_skipped = _max_parts - (parts_hi + parts_lo);
            
            if (parts_skipped < 1) {
                error = error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            return { parts_hi, parts_lo, parts_skipped };
        } else {
            if (parts_count != _max_parts) {
                error = error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            if (parts[0].empty()) {
                error = error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            if (parts[parts_count - 1].empty()) {
                error = error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            return { parts_count, 0, 0 };
        }
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE base_type parse_parts(const std::array<fixed_string<4>, _max_parts + 1>& parts, size_t parts_count, size_t hi, size_t lo, size_t skipped, error_code& error) IPADDRESS_NOEXCEPT {
        base_type result = {};
        size_t index = 0;

        for (size_t i = 0; i < hi; ++i) {
            const auto part = parse_part(parts[i], error);
            result[index++] = uint8_t(part >> 8);
            result[index++] = uint8_t(part & 0xFF);

            if (error != error_code::NO_ERROR) {
                return {};
            }
        }
        index += skipped << 1;

        assert(parts_count > lo);
        for (size_t i = parts_count - lo; i < parts_count; ++i) {
            const auto part = parse_part(parts[i], error);
            result[index++] = uint8_t(part >> 8);
            result[index++] = uint8_t(part & 0xFF);

            if (error != error_code::NO_ERROR) {
                return {};
            }
        }
        return result;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint16_t parse_part(const fixed_string<4>& part, error_code& error) IPADDRESS_NOEXCEPT {
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint16_t pow16(size_t power) IPADDRESS_NOEXCEPT {
        switch (power) {
            case 0: return 1;
            case 1: return 16;
            case 2: return 16 * 16;
            case 3: return 16 * 16 * 16;
            default:
                assert(!"Unreachable code");
                return 0;
        }
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void to_hex(uint16_t value, char(&result)[5]) IPADDRESS_NOEXCEPT {
        char digits[] = "0123456789abcdef";
        for (auto i = 0, j = (4 - 1) * 4; i < 4; ++i, j -= 4) {
            result[i] = digits[(value >> j) & 0x0f];
            result[i + 1] = '\0';
        }
    }
};

template <typename Ext>
IPADDRESS_CONSTEXPR ip_version base_v6<Ext>::base_version;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v6<Ext>::base_size;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v6<Ext>::base_max_string_len;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v6<Ext>::base_max_prefixlen;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v6<Ext>::_min_parts;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v6<Ext>::_max_parts;

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_BASE_V6_HPP
