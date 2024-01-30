#ifndef IPADDRESS_BASE_V4_HPP
#define IPADDRESS_BASE_V4_HPP

#include "ip-address-base.hpp"
#include "hash.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Ext>
class base_v4 {
public:
    static IPADDRESS_CONSTEXPR version version = version::V4;

    static IPADDRESS_CONSTEXPR size_t size = 4;

    static IPADDRESS_CONSTEXPR uint32_t all_ones = std::numeric_limits<uint32_t>::max();

    static IPADDRESS_CONSTEXPR size_t max_string_len = 15;

    static IPADDRESS_CONSTEXPR size_t max_prefixlen = size * 8;

    using base_type = byte_array_type<size>;

protected:
    template <typename Iter>
    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_string(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
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
        return ip_address_base<Ext>(octets);
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_uint32(uint32_t ip) IPADDRESS_NOEXCEPT {
        ip = is_little_endian() ? swap_bytes(ip) : ip;
        return ip_address_base<Ext>({
            uint8_t(ip & 0xFF),
            uint8_t((ip >> 8) & 0xFF),
            uint8_t((ip >> 16) & 0xFF),
            uint8_t((ip >> 24) & 0xFF) });
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Ext> ip_from_prefix(size_t prefixlen) {
        return ip_address_base<Ext>::ip_from_uint32(all_ones ^ (all_ones >> (prefixlen - 1) >> 1));
    }

    static IPADDRESS_CONSTEXPR uint32_t ip_to_uint32(const base_type& bytes) IPADDRESS_NOEXCEPT {
        const auto ip = 
            (uint32_t(bytes[3]) << 24) | 
            (uint32_t(bytes[2]) << 16) | 
            (uint32_t(bytes[1]) << 8) | 
            (uint32_t(bytes[0]));
        return is_little_endian() ? swap_bytes(ip) : ip;
    }

    static std::string ip_to_string(const base_type& bytes, format fmt) {
        std::ostringstream res;
        res << size_t(bytes[0]) << '.'
            << size_t(bytes[1]) << '.' 
            << size_t(bytes[2]) << '.' 
            << size_t(bytes[3]);
        return res.str();
    }

    static IPADDRESS_CONSTEXPR std::size_t ip_to_hash(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return calc_hash(0, size_t(bytes[0]), size_t(bytes[1]), size_t(bytes[2]), size_t(bytes[3]));
    }

    static std::string ip_reverse_pointer(const base_type& bytes) {
        return ip_from_uint32(swap_bytes(ip_to_uint32(bytes))).to_string() + ".in-addr.arpa";
    }

    template <typename Iter>
    static IPADDRESS_CONSTEXPR std::tuple<ip_address_base<Ext>, ip_address_base<Ext>, size_t> parse_netmask(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        size_t prefixlen = 0;
        auto is_value = true;
        auto has_prefixlen = false;
        for (auto it = begin; it != end; ++it) {
            has_prefixlen = true;
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
                return std::make_tuple(ip_address_base<Ext>(), ip_address_base<Ext>(), 0);
            }
        } else {
            auto ip = ip_to_uint32(ip_from_string(begin, end, code, index).bytes());
            if (code != error_code::NO_ERROR) {
                code = error_code::INVALID_NETMASK;
                return std::make_tuple(ip_address_base<Ext>(), ip_address_base<Ext>(), 0);
            }

            prefixlen = prefix_from_ip_uint32(ip, code);
            if (code != error_code::NO_ERROR) {
                ip = ip ^ all_ones;
                code = error_code::NO_ERROR;
                prefixlen = prefix_from_ip_uint32(ip, code);
                if (code != error_code::NO_ERROR) {
                    return std::make_tuple(ip_address_base<Ext>(), ip_address_base<Ext>(), 0);
                }
            }
        }
        prefixlen  = has_prefixlen ? prefixlen : max_prefixlen;
        auto netmask = ip_from_prefix(prefixlen);
        auto hostmask = ip_from_uint32(netmask.to_uint32() ^ all_ones);
        return std::make_tuple(netmask, hostmask, prefixlen);
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Ext> strict_netmask(const ip_address_base<Ext>& address, const ip_address_base<Ext>& netmask, bool strict, error_code& code) IPADDRESS_NOEXCEPT {
        auto pack_address = address.to_uint32();
        auto pack_netmask = netmask.to_uint32();
        if ((pack_address & pack_netmask) != pack_address) {
            if (strict) {
                code = error_code::HAS_HOST_BITS_SET;
                return ip_address_base<Ext>();
            } else {
                return ip_from_uint32(pack_address & pack_netmask);
            }
        }
        return address;
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
        auto all_ones = (1 << (prefixlen - 1) << 1) - 1;
        if (leading_ones != all_ones) {
            code = error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES;
            return 0;
        }
        return prefixlen;
    }
    
    static IPADDRESS_CONSTEXPR size_t hash_scope_id() IPADDRESS_NOEXCEPT {
        return 0;
    }

    static IPADDRESS_CONSTEXPR bool compare_scope_id(const ip_address_base<Ext>& lhs, const ip_address_base<Ext>& rhs) IPADDRESS_NOEXCEPT {
        return 0;
    }
};

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_BASE_V4_HPP
