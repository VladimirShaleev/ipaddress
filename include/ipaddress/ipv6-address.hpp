#ifndef IPADDRESS_IPV6_ADDRESS_HPP
#define IPADDRESS_IPV6_ADDRESS_HPP

#include "ipv4-address.hpp"
#include "base-v6.hpp"

namespace IPADDRESS_NAMESPACE {

class scope final {
public:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope(const fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>& scope_id) IPADDRESS_NOEXCEPT
        : 
        _scope_id(scope_id),
        _scope_id_value(0),
        _has_value(false) {
        parse_value();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string get_string() const {
        return std::string(_scope_id.begin(), _scope_id.end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint32_t get_uint32() const IPADDRESS_NOEXCEPT {
        return _scope_id_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_string() const IPADDRESS_NOEXCEPT {
        return !_scope_id.empty();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_uint32() const IPADDRESS_NOEXCEPT {
        return _has_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return has_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE operator std::string() const {
        return get_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE operator uint32_t() const IPADDRESS_NOEXCEPT {
        return get_uint32();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id == rhs._scope_id;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

     IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const scope& rhs) const IPADDRESS_NOEXCEPT {
         return _scope_id <=> rhs._scope_id;
     }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return _scope_id < rhs._scope_id;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const scope& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void parse_value() IPADDRESS_NOEXCEPT {
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

    fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH> _scope_id;
    uint32_t _scope_id_value;
    bool _has_value;
}; // scope

class ipv6_address_base : public base_v6<ipv6_address_base> {
public:
    using base_type = typename base_v6<ipv6_address_base>::base_type;
    using uint_type = typename base_v6<ipv6_address_base>::uint_type;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope get_scope_id() const IPADDRESS_NOEXCEPT {
        return scope(
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            make_fixed_string("")
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
        );
    }

    template <size_t N>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void set_scope_id(const char(&scope_id)[N]) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        static_assert(N <= IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1, "scope id is too long");
        char str[IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1] = {};
        for (size_t i = 0; i < N; ++i) {
            str[i] = scope_id[i];
        }
        _data.scope_id = make_fixed_string(str);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        change_scope_id(scope_id);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        change_scope_id(scope_id, code);
    }

#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        change_scope_id(scope_id);
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        change_scope_id(scope_id, code);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<ipv6_address_base> from_uint(uint_type ip) IPADDRESS_NOEXCEPT {
        ip_address_base<ipv6_address_base> result;
        auto& bytes = result._data.bytes;
        uint64_t shift = 0;
        uint64_t inc = 8;
        if (is_little_endian()) {
            shift = 56;
            inc = -8;
        }
        for (int i = 0, s = shift; i < 8; ++i, s += inc) {
            bytes[i] = uint8_t(ip.upper() >> s);
            bytes[i + 8] = uint8_t(ip.lower() >> s);
        }
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE uint_type to_uint() const IPADDRESS_NOEXCEPT {
        const auto& bytes = _data.bytes;
        uint64_t upper = 0;
        uint64_t lower = 0;
        uint64_t shift = 0;
        uint64_t inc = 8;
        if (is_little_endian()) {
            shift = 56;
            inc = -8;
        }
        for (int i = 0, s = shift; i < 8; ++i, s += inc) {
            upper |= uint64_t(bytes[i]) << s;
            lower |= uint64_t(bytes[i + 8]) << s;
        }
        return uint_type(upper, lower);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _data.bytes;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> ipv4_mapped() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[10] != 0xFF || b[11] != 0xFF) {
            return nullptr;
        } 
        ipv4_address::base_type ipv4_bytes = { b[12], b[13], b[14], b[15] };
        return ipv4_address(ipv4_bytes);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> sixtofour() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[0] != 0x20 || b[1] != 0x02) {
            return nullptr;
        }
        ipv4_address::base_type ipv4_bytes = { b[2], b[3], b[4], b[5] };
        return ipv4_address(ipv4_bytes);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<std::pair<ipv4_address, ipv4_address>> teredo() const IPADDRESS_NOEXCEPT {
        const auto& b = bytes();
        if (b[0] != 0x20 || b[1] != 0x01 || b[2] != 0x00 || b[3] != 0x00) {
            return nullptr;
        }
        ipv4_address::base_type server_bytes = { b[4], b[5], b[6], b[7] };
        ipv4_address::base_type client_bytes = { uint8_t(~b[12]), uint8_t(~b[13]), uint8_t(~b[14]), uint8_t(~b[15]) };
        return std::make_pair(ipv4_address(server_bytes), ipv4_address(client_bytes));
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT;

protected:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address_base() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ipv6_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _data({ bytes }) {
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(ip_address_base<ipv6_address_base>& lhs, ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        lhs._data.bytes.swap(rhs._data.bytes);
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        lhs._data.scope_id.swap(rhs._data.scope_id);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t hash(const base_type& bytes) const IPADDRESS_NOEXCEPT {
        return calc_hash(
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id.hash(),
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            0,
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            size_t(bytes[0]), size_t(bytes[1]), size_t(bytes[2]), size_t(bytes[3]), 
            size_t(bytes[4]), size_t(bytes[5]), size_t(bytes[6]), size_t(bytes[7]), 
            size_t(bytes[8]), size_t(bytes[9]), size_t(bytes[10]), size_t(bytes[11]), 
            size_t(bytes[12]), size_t(bytes[13]), size_t(bytes[14]), size_t(bytes[15]));
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool equals(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._data.bytes == rhs._data.bytes
        
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            && lhs._data.scope_id.compare(rhs._data.scope_id) == 0
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
            ;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool less(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
        return lhs._data.bytes < rhs._data.bytes 
        
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            ? true : lhs._data.scope_id.compare(rhs._data.scope_id) < 0;
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
            ;
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering compare(const ip_address_base<ipv6_address_base>& lhs, const ip_address_base<ipv6_address_base>& rhs) IPADDRESS_NOEXCEPT {
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        if (auto result = lhs._data.bytes <=> rhs._data.bytes; result == std::strong_ordering::equivalent) {
            if (const auto scope = lhs._data.scope_id.compare(rhs._data.scope_id); scope < 0) {
                return std::strong_ordering::less;
            } else if (scope == 0) {
                return std::strong_ordering::equivalent;
            } else {
                return std::strong_ordering::greater;
            }
        } else {
            return result;
        }
    #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
        return lhs._data.bytes <=> rhs._data.bytes;
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
    }

#endif // IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD std::string IPADDRESS_FORCE_INLINE ip_reverse_pointer(const base_type& bytes) const {
        return base_v6<ipv6_address_base>::ip_reverse_pointer(bytes, 
        
        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            make_fixed_string("")
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
        );
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t ip_to_chars(const base_type& bytes, format fmt, char (&result)[_max_string_len + 1]) const IPADDRESS_NOEXCEPT {
        return base_v6<ipv6_address_base>::ip_to_chars(bytes, 

        #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
            _data.scope_id,
        #else // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            make_fixed_string(""),
        #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH <= 0
            fmt,
            result);
    }

private:
    template <typename Str>
    #if !defined(__clang_major__) || __clang_major__ > 14
    IPADDRESS_CONSTEXPR
    #endif
    IPADDRESS_FORCE_INLINE void change_scope_id(const Str& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        error_code err = error_code::NO_ERROR;
        change_scope_id(scope_id, err);
    #ifndef IPADDRESS_NO_EXCEPTIONS
        if (err != error_code::NO_ERROR) {
            raise_error(err, 0, "<bytes>", 7);
        }
    #endif
    }

    template <typename Str>
    #if !defined(__clang_major__) || __clang_major__ > 14
    IPADDRESS_CONSTEXPR
    #endif
    IPADDRESS_FORCE_INLINE void change_scope_id(const Str& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        if (scope_id.size() > IPADDRESS_IPV6_SCOPE_MAX_LENGTH) {
            code = error_code::SCOPE_ID_IS_TOO_LONG;
            return;
        }
        char scope[IPADDRESS_IPV6_SCOPE_MAX_LENGTH + 1] = {};
        for (size_t i = 0; i < scope_id.size(); ++i) {
            scope[i] = scope_id[i];
        }
        _data.scope_id = make_fixed_string(scope);
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    }
    
    struct ipv6_data {
        base_type bytes{};
    #if IPADDRESS_IPV6_SCOPE_MAX_LENGTH > 0
        fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH> scope_id;
    #endif // IPADDRESS_IPV6_SCOPE_MAX_LENGTH
    } _data;

#ifndef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base<ipv6_address_base> operator""_ipv6(const char* address, std::size_t size) IPADDRESS_NOEXCEPT;
    
#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

}; // ipv6_address_base

using ipv6_address = ip_address_base<ipv6_address_base>;

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6() IPADDRESS_NOEXCEPT {
        return ipv6_address::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ipv6_address operator""_ipv6(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::_max_string_len && "litteral string is too long");
        char str[ipv6_address::_max_string_len + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ipv6_address::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV6_ADDRESS_HPP
