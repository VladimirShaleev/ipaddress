#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
protected:
    typedef uint32_t BaseType;

    template <typename FixedString>
    static constexpr BaseType ip_from_string(const FixedString& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto octets = parse_octets(address.begin(), address.end());
        return is_little_endian() ? swap(octets) : octets;
    }

    // static BaseType ip_from_bytes() {

    // }

private:
    template <typename Iter>
    static constexpr BaseType parse_octets(Iter begin, Iter end) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        BaseType octets = 0;
        size_t index = 0;
        int octet = 0;
        int digits = 0;
        for (auto it = begin; it != end; ++it) {
            auto c = *it;
            if (index >= 4) {    
            #ifdef IPADDRESS_NO_EXCEPTIONS
                return 0;
            #else
                throw parse_error("expected 4 octets in", std::string(begin, end));
            #endif
            }
            if (c >= '0' && c <= '9') {
                auto d = c - '0';
                octet = octet * 10 + d;
                ++digits;
                if (digits > 3) {
                #ifdef IPADDRESS_NO_EXCEPTIONS
                    return 0;
                #else
                    throw parse_error("in octet", index, "of address" + std::string(begin, end), "more 3 characters");
                #endif
                }
            } else if (c == '.' && digits > 0) {
                if (octet > 255) {
                #ifdef IPADDRESS_NO_EXCEPTIONS
                    return 0;
                #else
                    throw parse_error("octet", index, "of address", std::string(begin, end), "exceeded 255");
                #endif
                }
                digits = 0;
                octets |= (octet & 0xFF) << (index * 8);
                octet = 0;
                ++index;
            } else if (c == '.') {
                #ifdef IPADDRESS_NO_EXCEPTIONS
                    return 0;
                #else
                    throw parse_error("empty octet", index, "in address", std::string(begin, end));
                #endif
            } else {
                #ifdef IPADDRESS_NO_EXCEPTIONS
                    return 0;
                #else
                    throw parse_error("in octet", index, "of address", std::string(begin, end), "has invalid symbol");
                #endif
            }
        }
        if (index != 3) {
        #ifdef IPADDRESS_NO_EXCEPTIONS
            return 0;
        #else
            throw parse_error("expected 4 octets in", std::string(begin, end));
        #endif
        }
        if (digits == 0) {
        #ifdef IPADDRESS_NO_EXCEPTIONS
            return 0;
        #else
            throw parse_error("empty octet", index, "in address", std::string(begin, end));
        #endif
        }
        octets |= (octet & 0xFF) << (index * 8);
        return octets;
    }

    static constexpr bool is_little_endian() noexcept {
    #if IPADDRESS_CPP_VERSION >= 20
        return std::endian::native == std::endian::little;
    #elif defined(IPADDRESS_BIG_ENDIAN)
        return !IPADDRESS_BIG_ENDIAN;
    #else
    #   error "Define IPADDRESS_BIG_ENDIAN with 0 is little-endian and 1 is big-endian"
    #endif
    }
    
    static constexpr BaseType swap(BaseType value) noexcept {
        value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0x00FF00FF);
        value = (value << 16) | (value >> 16);
        return value;
    }
};

using ipv4_address = ip_address_base<base_v4>;

}

#endif
