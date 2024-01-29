#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "endian.hpp"
#include "byte-array.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

enum class version {
    V4 = 4,
    V6 = 6
};

enum class format {
    full = 0,
    compact,
    compressed
};

template <typename Base>
class ip_address_base : public Base {
public:
    template <typename>
    friend class ip_network_base;

    using base_type = typename Base::base_type;

    IPADDRESS_CONSTEXPR ip_address_base() IPADDRESS_NOEXCEPT : _bytes({}) {
    }

    IPADDRESS_CONSTEXPR explicit ip_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _bytes(bytes) {
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base from_bytes(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return ip_address_base(bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base from_bytes(const uint8_t* bytes, size_t byte_count) IPADDRESS_NOEXCEPT {
        base_type data = {};
        for (size_t i = 0; i < Base::size && i < byte_count; ++i) {
            data[i] = bytes[i];
        }
        return ip_address_base(data);
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval ip_address_base parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(str.begin(), str.end(), code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return ip_address_base(result);
    }
#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_address_base parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR ip_address_base parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }
#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_address_base parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static ip_address_base parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    template <size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_address_base parse(const char(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto str = make_fixed_string(address);
        return parse_string(str);
    }

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_address_base parse(const char(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        auto str = make_fixed_string(address);
        return parse_string(str, code);
    }

    IPADDRESS_NODISCARD std::string to_string(format fmt = format::compressed) const {
        return Base::ip_to_string(_bytes, fmt);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _bytes;
    }

    IPADDRESS_CONSTEXPR void swap(ip_address_base& ip) IPADDRESS_NOEXCEPT {
        _bytes.swap(ip._bytes);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t hash() const IPADDRESS_NOEXCEPT {
        size_t seed = Base::hash_scope_id();
        for (size_t i = 0; i < Base::size; i += 4) {
            const auto bytes_hash = 
                size_t(_bytes[i + 0]) << 24 | 
                size_t(_bytes[i + 1]) << 16 | 
                size_t(_bytes[i + 2]) << 8 | 
                size_t(_bytes[i + 3]);
            seed ^= bytes_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    IPADDRESS_NODISCARD std::string reverse_pointer() const {
       return Base::ip_reverse_pointer(_bytes);
    }

    IPADDRESS_NODISCARD explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return _bytes == rhs._bytes && Base::compare_scope_id(*this, rhs) == 0;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        if (auto result = _bytes <=> rhs._bytes; result == std::strong_ordering::equivalent) {
            if (const auto scope = Base::compare_scope_id(*this, rhs); scope < 0) {
                return std::strong_ordering::less;
            } else if (scope == 0) {
                return std::strong_ordering::equivalent;
            } else {
                return std::strong_ordering::greater;
            }
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return _bytes < rhs._bytes ? true : Base::compare_scope_id(*this, rhs) < 0;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_address_base parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(address.begin(), address.end(), code, index);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, index, address.data(), address.size());
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, address.data(), address.size());
        #endif
        }
        return result;
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_address_base parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return Base::ip_from_string(address.begin(), address.end(), code, index);
    }

    base_type _bytes;
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

inline int stream_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

inline std::ostream& full(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::full) + 1;
    return stream;
}

inline std::ostream& compact(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compact) + 1;
    return stream;
}

inline std::ostream& compressed(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compressed) + 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <typename Base>
struct hash<IPADDRESS_NAMESPACE::ip_address_base<Base>> {
    IPADDRESS_CONSTEXPR std::size_t operator()(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) const IPADDRESS_NOEXCEPT {
        return ip.hash();
    }
};

template <typename Base>
inline IPADDRESS_CONSTEXPR void swap(IPADDRESS_NAMESPACE::ip_address_base<Base>& ip1, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip2) IPADDRESS_NOEXCEPT {
    return ip1.swap(ip2);
}

template <typename Base>
inline std::string to_string(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    return ip.to_string();
}

template <typename Base>
inline std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    return stream << ip.to_string(fmt);
}

template <typename Base>
inline std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    ip = IPADDRESS_NAMESPACE::ip_address_base<Base>::parse(str, err);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ADDRESS_BASE_HPP
