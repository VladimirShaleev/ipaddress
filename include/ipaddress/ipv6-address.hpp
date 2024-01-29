#ifndef IPADDRESS_IPV6_ADDRESS_HPP
#define IPADDRESS_IPV6_ADDRESS_HPP

#include "ipv4-address.hpp"
#include "base-v6.hpp"

namespace IPADDRESS_NAMESPACE {

class scope final {
public:
    IPADDRESS_CONSTEXPR scope(const fixed_string<16>& scope_id) IPADDRESS_NOEXCEPT
        : 
        _scope_id(scope_id),
        _scope_id_value(0),
        _has_value(false) {
        parse_value();
    }

    IPADDRESS_NODISCARD std::string get_string() const {
        return std::string(_scope_id.begin(), _scope_id.end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR uint32_t get_uint32() const IPADDRESS_NOEXCEPT {
        return _scope_id_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool has_string() const IPADDRESS_NOEXCEPT {
        return !_scope_id.empty();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool has_uint32() const IPADDRESS_NOEXCEPT {
        return _has_value;
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

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

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

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_CONSTEXPR void parse_value() IPADDRESS_NOEXCEPT {
        if (!_scope_id.empty()) {
            _has_value = true;
            uint32_t value = 0;
            for (auto c : _scope_id) {
                if (c >= '0' && c <= '9') {
                    value = value * 10 + (c - '0');
                } else {
                    _has_value = false;
                    break;
                }
            }
            _scope_id_value = _has_value ? value : 0;
        }
    }

    fixed_string<16> _scope_id;
    uint32_t _scope_id_value;
    bool _has_value;
}; // scope

class ipv6_address_base : public base_v6<ipv6_address_base> {
public:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR scope get_scope_id() const IPADDRESS_NOEXCEPT {
        return scope(_scope_id);
    }

    template <size_t N>
    IPADDRESS_CONSTEXPR void set_scope_id(const char(&scope_id)[N]) IPADDRESS_NOEXCEPT {
        static_assert(N <= 16, "scope id is too long");
        char str[17] = {};
        for (size_t i = 0; i < N; ++i) {
            str[i] = scope_id[i];
        }
        _scope_id = make_fixed_string(str);
    }

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_CONSTEXPR void set_scope_id(std::string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        change_scope_id(scope_id);
    }

    IPADDRESS_CONSTEXPR void set_scope_id(std::string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        change_scope_id(scope_id, code);
    }

#else // IPADDRESS_CPP_VERSION < 17

    void set_scope_id(const std::string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        change_scope_id(scope_id);
    }

    void set_scope_id(const std::string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        change_scope_id(scope_id, code);
    }

#endif // IPADDRESS_CPP_VERSION < 17
}; // ipv6_address_base

using ipv6_address = ip_address_base<ipv6_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    inline consteval ipv6_address operator""_ipv6() IPADDRESS_NOEXCEPT {
        return ipv6_address::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    inline IPADDRESS_CONSTEXPR ipv6_address operator""_ipv6(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::max_string_len && "litteral string is too long");
        char str[ipv6_address::max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv6_address::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV6_ADDRESS_HPP
