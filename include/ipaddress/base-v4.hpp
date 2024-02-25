/**
 * @file      base-v4.hpp
 * @brief     Adds basic functionality for working with IPv4
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This file provides foundational classes and functions necessary for handling IPv4 addresses,
 * including parsing, constructing, and manipulating IPv4 address representations. It serves as
 * a base for higher-level IPv4 address operations and utilities.
 */

#ifndef IPADDRESS_BASE_V4_HPP
#define IPADDRESS_BASE_V4_HPP

#include "ip-address-base.hpp"
#include "hash.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A template class providing the base functionality for IPv4 addresses.
 * 
 * This class encapsulates the basic properties and operations for IPv4 addresses,
 * such as version identification, size, and conversion from string representations.
 * It is intended to be extended by more specialized IPv4 address classes.
 * 
 * @tparam Ext The extension type that provides additional functionality.
 */
template <typename Ext>
class base_v4 {
public:
    using base_type = byte_array<4>;
    using uint_type = uint32_t;

    static IPADDRESS_CONSTEXPR ip_version base_version = ip_version::V4;
    static IPADDRESS_CONSTEXPR size_t base_size = 4;
    static IPADDRESS_CONSTEXPR size_t base_max_string_len = 15;
    static IPADDRESS_CONSTEXPR size_t base_max_prefixlen = base_size * 8;
    static IPADDRESS_CONSTEXPR uint_type base_all_ones = std::numeric_limits<uint_type>::max();

    /**
     * Retrieves the IP version of the address.
     * 
     * @return The IP version enumeration value for IPv4.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return base_version;
    }

    /**
     * Retrieves the size of the IPv4 address.
     * 
     * @return The size of the IPv4 address in bytes.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return base_size;
    }

protected:
    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> ip_from_string(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
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
            auto c = char(*it);
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> ip_from_uint32(uint_type ip) IPADDRESS_NOEXCEPT {
        ip = is_little_endian() ? swap_bytes(ip) : ip;
        return ip_address_base<Ext>({
            uint8_t(ip & 0xFF),
            uint8_t((ip >> 8) & 0xFF),
            uint8_t((ip >> 16) & 0xFF),
            uint8_t((ip >> 24) & 0xFF) });
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> ip_from_prefix(size_t prefixlen) {
        return prefixlen != 0
            ? ip_address_base<Ext>::ip_from_uint32(base_all_ones ^ (base_all_ones >> (prefixlen - 1) >> 1))
            : ip_address_base<Ext>::ip_from_uint32(0);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type ip_to_uint32(const base_type& bytes) IPADDRESS_NOEXCEPT {
        const auto ip = 
            (uint32_t(bytes[3]) << 24) | 
            (uint32_t(bytes[2]) << 16) | 
            (uint32_t(bytes[1]) << 8) | 
            (uint32_t(bytes[0]));
        return is_little_endian() ? swap_bytes(ip) : ip;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t ip_to_chars(const base_type& bytes, format fmt, char (&result)[base_max_string_len + 1]) IPADDRESS_NOEXCEPT {
        size_t offset = 0;
        char buffer[4] {};
        for (size_t b = 0; b < 4; ++b) {
            size_t length = byte_to_string(bytes[b], buffer);
            for (size_t i = 0; i < length; ++i) {
                result[offset++] = buffer[i];
            }
            if (b < 3) {
                result[offset++] = '.';
            }
        }
        result[offset] = '\0';
        return offset;
    }

    IPADDRESS_NODISCARD static IPADDRESS_FORCE_INLINE std::string ip_reverse_pointer(const base_type& bytes) {
        return ip_from_uint32(swap_bytes(ip_to_uint32(bytes))).to_string() + ".in-addr.arpa";
    }

    template <typename Iter>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::tuple<ip_address_base<Ext>, size_t> parse_netmask(Iter begin, Iter end, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        size_t prefixlen = 0;
        auto is_value = true;
        auto has_prefixlen = false;
        for (auto it = begin; it != end; ++it) {
            has_prefixlen = true;
            const auto c = char(*it);
            if (c >= '0' && c <= '9') {
                prefixlen = prefixlen * 10 + (c - '0');
            } else {
                is_value = false;
                break;
            }
        }
        if (is_value) {
            if (prefixlen > base_max_prefixlen) {
                code = error_code::INVALID_NETMASK;
                return std::make_tuple(ip_address_base<Ext>(), 0);
            }
        } else {
            auto ip = ip_to_uint32(ip_from_string(begin, end, code, index).bytes());
            if (code != error_code::NO_ERROR) {
                code = error_code::INVALID_NETMASK;
                return std::make_tuple(ip_address_base<Ext>(), 0);
            }

            prefixlen = prefix_from_ip_uint32(ip, code);
            if (code != error_code::NO_ERROR) {
                ip = ip ^ base_all_ones;
                code = error_code::NO_ERROR;
                prefixlen = prefix_from_ip_uint32(ip, code);
                if (code != error_code::NO_ERROR) {
                    return std::make_tuple(ip_address_base<Ext>(), 0);
                }
            }
        }
        prefixlen = has_prefixlen ? prefixlen : base_max_prefixlen;
        auto netmask = ip_from_prefix(prefixlen);
        return std::make_tuple(netmask, prefixlen);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<Ext> strict_netmask(const ip_address_base<Ext>& address, const ip_address_base<Ext>& netmask, bool strict, error_code& code) IPADDRESS_NOEXCEPT {
        auto pack_address = address.to_uint();
        auto pack_netmask = netmask.to_uint();
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

private:
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t prefix_from_ip_uint32(uint_type ip, error_code& code) IPADDRESS_NOEXCEPT {
        auto trailing_zeroes = count_righthand_zero_bits(ip, base_max_prefixlen);
        auto prefixlen = base_max_prefixlen - trailing_zeroes;
        auto leading_ones = trailing_zeroes != 32 ? (ip >> trailing_zeroes) : 0;
        auto all_ones = (uint_type(1) << (prefixlen - 1) << 1) - uint_type(1);
        if (leading_ones != all_ones) {
            code = error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES;
            return 0;
        }
        return prefixlen;
    }
    
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t count_righthand_zero_bits(uint32_t number, uint32_t bits) IPADDRESS_NOEXCEPT {
        if (number == 0) {
            return bits;
        } else {
            number = (~number & (number - 1));
            size_t count = 0;
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

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t byte_to_string(uint8_t byte, char (&str)[4]) IPADDRESS_NOEXCEPT {
        size_t length = 0;
        char* p = str;
        do {
            *p++ = char((byte % 10) + '0');
            byte /= 10;
            ++length;
        } while (byte != 0);

        *p = '\0';

        for (size_t i = 0, midle = length / 2; i < midle; ++i) {
            auto tmp = str[i];
            str[i] = *--p;
            *p = tmp;
        }

        return length;
    }
};

template <typename Ext>
IPADDRESS_CONSTEXPR ip_version base_v4<Ext>::base_version;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v4<Ext>::base_size;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v4<Ext>::base_max_string_len;

template <typename Ext>
IPADDRESS_CONSTEXPR size_t base_v4<Ext>::base_max_prefixlen;

template <typename Ext>
IPADDRESS_CONSTEXPR typename base_v4<Ext>::uint_type base_v4<Ext>::base_all_ones;

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_BASE_V4_HPP
