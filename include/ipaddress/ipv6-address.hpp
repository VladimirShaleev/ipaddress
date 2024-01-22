#ifndef IPADDRESS_IPV6_ADDRESS_HPP
#define IPADDRESS_IPV6_ADDRESS_HPP

#include "ipv4-address.hpp"

namespace IPADDRESS_NAMESPACE {

class scope final {
public:
    IPADDRESS_CONSTEXPR scope(const fixed_string<16>& scope_id) IPADDRESS_NOEXCEPT
        : 
        _scope_id(scope_id),
        _scope_id_value(0),
        _parse_result(parse_result::UNDEFINED) {
    }

    IPADDRESS_NODISCARD std::string get_string() const {
        return std::string(_scope_id.begin(), _scope_id.end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR uint32_t get_uint32() const IPADDRESS_NOEXCEPT {
        parse_value();
        return _scope_id_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool has_string() const IPADDRESS_NOEXCEPT {
        return !_scope_id.empty();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool has_uint32() const IPADDRESS_NOEXCEPT {
        parse_value();
        return _parse_result == parse_result::HAS_VALUE;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR explicit operator bool() const IPADDRESS_NOEXCEPT {
        return has_string();
    }

    IPADDRESS_NODISCARD operator std::string() const {
        return get_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR operator uint32_t() const IPADDRESS_NOEXCEPT {
        return get_uint32();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id == rhs._scope_id;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR
     IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const scope& rhs) const IPADDRESS_NOEXCEPT {
         return _scope_id <=> rhs._scope_id;
     }
#else
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id < rhs._scope_id;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }
#endif

private:
    IPADDRESS_CONSTEXPR void parse_value() const IPADDRESS_NOEXCEPT {
        if (_parse_result == parse_result::UNDEFINED) {
            if (_scope_id.empty()) {
                _parse_result = parse_result::NO_VALUE;
            } else {
                _parse_result = parse_result::HAS_VALUE;
                uint32_t value = 0;
                for (auto c : _scope_id) {
                    if (c >= '0' && c <= '9') {
                        value = value * 10 + (c - '0');
                    } else {
                        _parse_result = parse_result::NO_VALUE;
                        break;
                    }
                }
                _scope_id_value = _parse_result == parse_result::HAS_VALUE ? value : 0;
            }
        }
    }

    enum class parse_result {
        UNDEFINED = 0,
        NO_VALUE,
        HAS_VALUE,
    };

    fixed_string<16> _scope_id;
    mutable uint32_t _scope_id_value;
    mutable parse_result _parse_result;
};

class base_v6 {
public:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR scope scope_id() const IPADDRESS_NOEXCEPT {
        return scope(_scope_id);
    }

protected:
    static IPADDRESS_CONSTEXPR size_t min_parts = 3;
    static IPADDRESS_CONSTEXPR size_t max_parts = 8;
    static IPADDRESS_CONSTEXPR size_t size = 16;

    using base_type = byte_array_type<size>;

    template <typename Iter>
    static IPADDRESS_CONSTEXPR ip_address_base<base_v6> ip_from_string(Iter begin, Iter end, error_code& code, int& parts_count) IPADDRESS_NOEXCEPT {
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

        auto ip = ip_address_base<base_v6>(parse_parts(parts, parts_count, std::get<0>(result), std::get<1>(result), std::get<2>(result), code));
        ip._scope_id = ip_and_scope.second;

        return ip;
    }
    
    static std::string ip_to_string(const base_type& bytes) {
        std::ostringstream res;
        return res.str();
    }

    static IPADDRESS_CONSTEXPR bool is_equals_scope(const ip_address_base<base_v6>& lhs, const ip_address_base<base_v6>& rhs) IPADDRESS_NOEXCEPT {
        return lhs.scope_id() == rhs.scope_id();
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR
     IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering compare_scope(const ip_address_base<base_v6>& lhs, const ip_address_base<base_v6>& rhs) const IPADDRESS_NOEXCEPT {
         return lhs.scope_id() <=> rhs.scope_id();
     }
#else
    static IPADDRESS_CONSTEXPR bool is_less_scope(const ip_address_base<base_v6>& lhs, const ip_address_base<base_v6>& rhs) IPADDRESS_NOEXCEPT {
        return lhs.scope_id() < rhs.scope_id();
    }
#endif

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
        std::array<char[5], max_parts> parts = {};
        char last_part[16] = {};

        parts_count = 0;
        int symbol = 0;

        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (parts_count >= max_parts) {
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

                auto& current_part = parts[parts_count++];
                current_part[0] = last_part[0];
                current_part[1] = last_part[1];
                current_part[2] = last_part[2];
                current_part[3] = last_part[3];

                symbol = 0;
                last_part[0] = '\0';
            }
        }
    
        if (parts_count >= max_parts) {
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
            
            if (parts_skipped < 1) {
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

    fixed_string<16> _scope_id{"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
};

using ipv6_address = ip_address_base<base_v6>;

}

#endif
