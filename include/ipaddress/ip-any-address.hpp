#ifndef IPADDRESS_IP_ANY_ADDRESS_HPP
#define IPADDRESS_IP_ANY_ADDRESS_HPP

#include "ipv6-address.hpp"

namespace IPADDRESS_NAMESPACE {

class ip_address {
public:
    using base_type_ipv4 = typename ipv4_address::base_type;
    using base_type_ipv6 = typename ipv6_address::base_type;

    using uint_type_ipv4 = typename ipv4_address::uint_type;
    using uint_type_ipv6 = typename ipv6_address::uint_type;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return _version;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_multicast() : _ipv.ipv6.is_multicast();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_private() : _ipv.ipv6.is_private();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_global() : _ipv.ipv6.is_global();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_reserved() : _ipv.ipv6.is_reserved();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_loopback() : _ipv.ipv6.is_loopback();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_link_local() : _ipv.ipv6.is_link_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.is_unspecified() : _ipv.ipv6.is_unspecified();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? false : _ipv.ipv6.is_site_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.size() : _ipv.ipv6.size();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> ipv4_mapped() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<ipv4_address>() : _ipv.ipv6.ipv4_mapped();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> sixtofour() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<ipv4_address>() : _ipv.ipv6.sixtofour();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<std::pair<ipv4_address, ipv4_address>> teredo() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? optional<std::pair<ipv4_address, ipv4_address>>() : _ipv.ipv6.teredo();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4 ? _ipv.ipv4.hash() : _ipv.ipv6.hash();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_address> v4() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            return optional<ipv4_address>();
        }
        auto ip = _ipv.ipv4;
        return optional<ipv4_address>(std::move(ip));
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv6_address> v6() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            return optional<ipv6_address>();
        }
        auto ip = _ipv.ipv6;
        return optional<ipv6_address>(std::move(ip));
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address(const ipv4_address& ipv4) IPADDRESS_NOEXCEPT : _ipv(ipv4), _version(ip_version::V4) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address(const ipv6_address& ipv6) IPADDRESS_NOEXCEPT : _ipv(ipv6), _version(ip_version::V6) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT : _ipv(bytes), _version(ip_version::V4) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT : _ipv(bytes), _version(ip_version::V6) {
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT {
        return ip_address(bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT {
        return ip_address(bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_bytes(const uint8_t* bytes, size_t byte_count, ip_version version) IPADDRESS_NOEXCEPT {
        return version == ip_version::V4 ? ip_address(ipv4_address::from_bytes(bytes, byte_count)) : ip_address(ipv6_address::from_bytes(bytes, byte_count));
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_uint(uint_type_ipv4 ip) IPADDRESS_NOEXCEPT {
        return ip_address(ipv4_address::from_uint(ip));
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address from_uint(const uint_type_ipv6& ip) IPADDRESS_NOEXCEPT {
        return ip_address(ipv6_address::from_uint(ip));
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return _version == ip_version::V4 ? _ipv.ipv4.to_string(fmt) : _ipv.ipv6.to_string(fmt);
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string reverse_pointer() const {
       return _version == ip_version::V4 ? _ipv.ipv4.reverse_pointer() : _ipv.ipv6.reverse_pointer();
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_address& ip) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = ip;
        ip = tmp;
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval IPADDRESS_FORCE_INLINE ip_address parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;

        char ip[str.size() + 1]{};
        for (size_t i = 0; i < str.size(); ++i) {
            ip[i] = str[i];
        }

        const auto ipv4 = ipv4_address::parse(ip, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        const auto ipv6 = ipv6_address::parse(ip, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv6);
        }

        raise_error(code, 0, str.data(), str.size());
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::wstring_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u16string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u32string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::wstring_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u16string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u32string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::wstring_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u16string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u32string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u8string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(std::u8string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void set_scope_id(std::u8string_view scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#endif // __cpp_char8_t

#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::wstring& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::u16string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address parse(const std::u32string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_FORCE_INLINE ip_address parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address parse(const std::wstring& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address parse(const std::u16string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address parse(const std::u32string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::wstring& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u16string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u32string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u8string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u8string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u8string& scope_id) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_FORCE_INLINE void set_scope_id(const std::u8string& scope_id, error_code& code) IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id, code);
        }
    }

#endif // __cpp_char8_t

#endif // IPADDRESS_CPP_VERSION < 17

    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(const T(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto result = parse_string(address, code);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, 0, address, N);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, 0, address, N);
        #endif
        }
        return result;
    }

    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse(const T(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        const auto ipv6 = ipv6_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv6);
        }
        
        return ip_address();
    }

    template <typename T, size_t N>
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void set_scope_id(const T(&scope_id)[N]) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        if (_version == ip_version::V6) {
            _ipv.ipv6.set_scope_id(scope_id);
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE scope get_scope_id() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            return fixed_string<IPADDRESS_IPV6_SCOPE_MAX_LENGTH>();
        }
        return _ipv.ipv6.get_scope_id();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return _version == ip_version::V4 ? _ipv.ipv4.to_string() : _ipv.ipv6.to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (_version != rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv.ipv4 == _ipv.ipv4) : (_ipv.ipv6 == _ipv.ipv6);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> rhs._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? (_ipv.ipv4 <=> _ipv.ipv4) : (_ipv.ipv6 <=> _ipv.ipv6);
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        if (_version < rhs._version) {
            return true;
        }
        if (_version > rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv.ipv4 < _ipv.ipv4) : (_ipv.ipv6 < _ipv.ipv6);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        return ip_address(ipv6_address::parse(address));
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        const auto ipv4 = ipv4_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv4);
        }
        
        const auto ipv6 = ipv6_address::parse(address, code);
        if (code == error_code::NO_ERROR) {
            return ip_address(ipv6);
        }
        
        return ip_address();
    }

    union ip_any_address {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address() IPADDRESS_NOEXCEPT : ipv4() {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address(const ipv4_address& ip) IPADDRESS_NOEXCEPT : ipv4(ip) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_address(const ipv6_address& ip) IPADDRESS_NOEXCEPT : ipv6(ip) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_any_address(const base_type_ipv4& bytes) IPADDRESS_NOEXCEPT : ipv4(bytes) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_any_address(const base_type_ipv6& bytes) IPADDRESS_NOEXCEPT : ipv6(bytes) {
        }

        ipv4_address ipv4;
        ipv6_address ipv6;
    } _ipv {};
    ip_version _version = ip_version::V4;
};

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ip_address operator""_ip() IPADDRESS_NOEXCEPT {
        return ip_address::parse<FixedString>();
    }

#else // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address operator""_ip(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= 57 && "literal string is too long");
        char str[57 + 1] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_address::parse(str);
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <>
struct hash<IPADDRESS_NAMESPACE::ip_address> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::ip_address& ip) const IPADDRESS_NOEXCEPT {
        return ip.hash();
    }
};

IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_address& ip1, IPADDRESS_NAMESPACE::ip_address& ip2) IPADDRESS_NOEXCEPT {
    ip1.swap(ip2);
}

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_address& ip) {
    return ip.to_string();
}

IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_address& ip) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    auto str = ip.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        auto end = std::find(str.cbegin(), str.cend(), '%');
        std::transform(str.cbegin(), end, str.begin(), [](auto c){
            return std::toupper(c);
        });
    }
    return stream << str;
}

IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_address& ip) {
    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    ip = IPADDRESS_NAMESPACE::ip_address::parse(str, err);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ANY_ADDRESS_HPP
