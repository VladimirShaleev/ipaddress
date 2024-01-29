#ifndef IPADDRESS_BASE_V6_HPP
#define IPADDRESS_BASE_V6_HPP

#include "ipv4-address.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Ext>
class base_v6 {
public:
    static IPADDRESS_CONSTEXPR version version = version::V6;

    static IPADDRESS_CONSTEXPR size_t size = 16;

    // static IPADDRESS_CONSTEXPR uint32_t all_ones = ...;

    static IPADDRESS_CONSTEXPR size_t max_string_len = 56;

    static IPADDRESS_CONSTEXPR size_t max_prefixlen = size * 8;

    using base_type = byte_array_type<size>;

protected:
    static IPADDRESS_CONSTEXPR size_t min_parts = 3;
    static IPADDRESS_CONSTEXPR size_t max_parts = 8;

    template <typename Iter>
    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_string(Iter begin, Iter end, error_code& code, int& parts_count) IPADDRESS_NOEXCEPT {
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
        ip._scope_id = ip_and_scope.second;

        return ip;
    }
    
    std::string ip_to_string(const base_type& bytes, format fmt) const {
        char hextets[size >> 1][5] = {};
        const size_t max_hextets = size >> 1;
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
                            doublecolon_start = i;
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

        std::ostringstream res;
        if (has_doublecolon_start) {
            res << ':';
        }
        for (size_t i = 0; i < count - 1; ++i) {
            res << hextets[i] << ':';
        }
        res << hextets[count - 1];
        if (has_doublecolon_end) {
            res << ':';
        }
        if (!_scope_id.empty()) {
            res << '%' << _scope_id.data();
        }
        return res.str();
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_prefix(size_t prefixlen) {
    #ifdef __SIZEOF_INT128__
        constexpr __uint128_t all_ones = ~0;
        __uint128_t bits = all_ones ^ (all_ones >> (prefixlen - 1) >> 1);
        return ip_from_bits(bits);
    
    #else
        assert(!"not implemented");
        return ip_address_base<Ext>();
    #endif
    }

    template <typename Bits>
    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_bits(const Bits& bits) IPADDRESS_NOEXCEPT {
        if (is_little_endian()) {
            base_type bytes = {
                uint8_t((bits >> 120) & 0xFF),
                uint8_t((bits >> 112) & 0xFF),
                uint8_t((bits >> 104) & 0xFF),
                uint8_t((bits >> 96) & 0xFF),
                uint8_t((bits >> 88) & 0xFF),
                uint8_t((bits >> 80) & 0xFF),
                uint8_t((bits >> 72) & 0xFF),
                uint8_t((bits >> 64) & 0xFF),
                uint8_t((bits >> 56) & 0xFF),
                uint8_t((bits >> 48) & 0xFF),
                uint8_t((bits >> 40) & 0xFF),
                uint8_t((bits >> 32) & 0xFF),
                uint8_t((bits >> 24) & 0xFF),
                uint8_t((bits >> 16) & 0xFF),
                uint8_t((bits >> 8) & 0xFF),
                uint8_t((bits >> 0) & 0xFF)
            };
            return ip_address_base<Ext>(bytes);
        } else {
            base_type bytes = {
                uint8_t((bits >> 0) & 0xFF),
                uint8_t((bits >> 8) & 0xFF),
                uint8_t((bits >> 16) & 0xFF),
                uint8_t((bits >> 24) & 0xFF),
                uint8_t((bits >> 32) & 0xFF),
                uint8_t((bits >> 40) & 0xFF),
                uint8_t((bits >> 48) & 0xFF),
                uint8_t((bits >> 56) & 0xFF),
                uint8_t((bits >> 64) & 0xFF),
                uint8_t((bits >> 72) & 0xFF),
                uint8_t((bits >> 80) & 0xFF),
                uint8_t((bits >> 88) & 0xFF),
                uint8_t((bits >> 96) & 0xFF),
                uint8_t((bits >> 104) & 0xFF),
                uint8_t((bits >> 112) & 0xFF),
                uint8_t((bits >> 120) & 0xFF)
            };
            return ip_address_base<Ext>(bytes);
        }
    }

    std::string ip_reverse_pointer(const base_type& bytes) const {
        auto ip = ip_to_string(bytes, format::full);
        ip.erase(std::remove(ip.begin(), ip.end(), ':'), ip.end());
        auto res = std::accumulate(std::next(ip.rbegin()), ip.rend(), std::string{ip.back()}, [](auto a, auto b) {
            return std::move(a) + '.' + b;
        });
        return res + ".ip6.arpa";
    }

    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::pair<ip_address_base<Ext>, size_t> parse_netmask(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        size_t prefixlen = 0;
        for (auto it = begin; it != end; ++it) {
            if (*it >= '0' && *it <= '9') {
                prefixlen = prefixlen * 10 + (*it - '0');
            } else {
                code = error_code::INVALID_NETMASK;
                return std::make_pair(ip_address_base<Ext>(), 0);
            }
        }
        
        if (prefixlen > max_prefixlen) {
            code = error_code::INVALID_NETMASK;
            return std::make_pair(ip_address_base<Ext>(), 0);
        }
        
        return std::make_pair(ip_from_prefix(prefixlen), prefixlen);
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Ext> strict_netmask(const ip_address_base<Ext>& address, const ip_address_base<Ext>& netmask, bool strict, error_code& code) IPADDRESS_NOEXCEPT {
    #ifdef __SIZEOF_INT128__
        const auto& bytes_address = address.bytes();
        const auto& bytes_netmask = netmask.bytes();
        __uint128_t pack_address = 0;
        __uint128_t pack_netmask = 0;

        const auto start = is_little_endian() ? (size - 1) * 8 : 0;
        const auto step = is_little_endian() ? -8 : 8;

        for (size_t i = 0, s = start; i < size; ++i, s += step) {
            pack_address |= (__uint128_t(bytes_address[i]) << s);
            pack_netmask |= (__uint128_t(bytes_netmask[i]) << s);
        }

        if ((pack_address & pack_netmask) != pack_address) {
            if (strict) {
                code = error_code::HAS_HOST_BITS_SET;
                return ip_address_base<Ext>();
            } else {
                const auto bits = pack_address & pack_netmask;
                return ip_from_bits(bits);
            }
        }
        return address;
    #else
        assert(!"not implemented");
        return ip_address_base<Ext>();
    #endif
    }

    IPADDRESS_CONSTEXPR size_t hash_scope_id() const IPADDRESS_NOEXCEPT {
        return _scope_id.hash();
    }

    static IPADDRESS_CONSTEXPR int compare_scope_id(const ip_address_base<Ext>& lhs, const ip_address_base<Ext>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._scope_id.compare(rhs._scope_id);
    }

    template <typename Str>
    IPADDRESS_CONSTEXPR void change_scope_id(const Str& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code err = error_code::NO_ERROR;
        change_scope_id(scope_id, err);
    #ifndef IPADDRESS_NO_EXCEPTIONS
        if (err != error_code::NO_ERROR) {
            raise_error(err, 0, "<bytes>", 7);
        }
    #endif
    }

    template <typename Str>
    IPADDRESS_CONSTEXPR void change_scope_id(const Str& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (scope_id.size() > 16) {
            code = error_code::SCOPE_ID_IS_TOO_LONG;
            return;
        }
        char scope[17] = {};
        for (size_t i = 0; i < scope_id.size(); ++i) {
            scope[i] = scope_id[i];
        }
        _scope_id = make_fixed_string(scope);
    }

    fixed_string<16> _scope_id{"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};

private:
    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::pair<Iter, fixed_string<16>> split_scope_id(Iter begin, Iter end, error_code& error) IPADDRESS_NOEXCEPT {
        char scope_id[17] = {};
        auto index = 0;
        Iter end_ip = begin;
        auto scope = false;
        for (auto it = begin; it != end; ++it) {
            if (!scope && *it != '%') {
                end_ip = it + 1;
            } else if (scope) {
                if (index > 15) {
                    error = error_code::SCOPE_ID_IS_TOO_LONG;
                    return std::make_pair(end_ip, make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"));
                }
                if (*it == '%' || *it == '/') {
                    error = error_code::INVALID_SCOPE_ID;
                    return std::make_pair(end_ip, make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"));
                }
                scope_id[index++] = *it;
            } else {
                scope = true;
            }
        }
        if (scope && scope_id[0] == '\0') {
            error = error_code::INVALID_SCOPE_ID;
            return std::make_pair(end_ip, make_fixed_string("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"));
        }
        return std::make_pair(end_ip, make_fixed_string(scope_id));
    }

    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::array<fixed_string<4>, max_parts> split_parts(Iter begin, Iter end, int& parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        char parts[max_parts][5] = {};
        char last_part[16] = {};
        
        parts_count = 0;
        int symbol = 0;
        char prev_c = '\0';
        bool has_double_colon = false;

        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (!has_double_colon && c == ':' && prev_c == ':') {
                has_double_colon = true;
            }
            if (parts_count >= max_parts) {
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
    
        if (parts_count >= max_parts) {
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
            if (parts_count + 1 >= max_parts) {
                error = error_code::MOST_8_COLONS_PERMITTED;
                return empty_parts;
            }

            auto ipv4 = ipv4_address::parse(last_part, error).to_uint32();

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

        if (parts_count < min_parts) {
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
            make_fixed_string(parts[7])};
    }

    static IPADDRESS_CONSTEXPR std::tuple<size_t, size_t, size_t> get_parts_bound(const std::array<fixed_string<4>, max_parts>& parts, int parts_count, error_code& error) IPADDRESS_NOEXCEPT {
        int skip = 0;
        for (auto i = 1; i < parts_count - 1; ++i) {
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

            if (parts[parts_count - 1].empty()) {
                error = error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON;
                return { 0, 0, 0 };
            }

            return { parts.size(), 0, 0 };
        }
    }

    static IPADDRESS_CONSTEXPR base_type parse_parts(const std::array<fixed_string<4>, max_parts>& parts, int parts_count, size_t hi, size_t lo, size_t skipped, error_code& error) IPADDRESS_NOEXCEPT {
        base_type result = {};
        int index = 0;

        for (size_t i = 0; i < hi; ++i) {
            const auto part = parse_part(parts[i], error);
            result[index++] = uint8_t(part >> 8);
            result[index++] = uint8_t(part & 0xFF);

            if (error != error_code::NO_ERROR) {
                return {};
            }
        }
        index += skipped << 1;

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

    static IPADDRESS_CONSTEXPR uint16_t pow16(size_t power) IPADDRESS_NOEXCEPT {
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

    static IPADDRESS_CONSTEXPR void to_hex(uint16_t value, char(&result)[5]) IPADDRESS_NOEXCEPT {
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

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_BASE_V6_HPP
