#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "endian.hpp"
#include "byte-array.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Base>
class ip_address_base : public Base {
public:
    using base_type = typename Base::base_type;

    IPADDRESS_CONSTEXPR ip_address_base() IPADDRESS_NOEXCEPT : _bytes({}) {
    }

    IPADDRESS_CONSTEXPR explicit ip_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _bytes(bytes) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval ip_address_base<Base> parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(str.begin(), str.end(), code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return ip_address_base(result);
    }
#endif

#if IPADDRESS_CPP_VERSION >= 17
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }
#else
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_address_base<Base> parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static ip_address_base<Base> parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }
#endif

    template <size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto str = make_fixed_string(address);
        return parse_string(str);
    }

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        auto str = make_fixed_string(address);
        return parse_string(str, code);
    }

    IPADDRESS_NODISCARD std::string to_string() const {
        return Base::ip_to_string(_bytes);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _bytes;
    }

    IPADDRESS_CONSTEXPR void swap(ip_address_base<Base>& ip) IPADDRESS_NOEXCEPT {
        _bytes.swap(ip._bytes);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t hash() const IPADDRESS_NOEXCEPT {
        size_t seed = 0;
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

    IPADDRESS_CONSTEXPR bool operator==(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return _bytes == rhs._bytes;
    }

    IPADDRESS_CONSTEXPR bool operator!=(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR
     IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
         return _bytes <=> rhs._bytes;
     }
#else
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return _bytes < rhs._bytes;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const ip_address_base<Base>& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }
#endif

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(address.begin(), address.end(), code, index);
        if (code != error_code::NO_ERROR) {
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, address.data(), address.size());
        #endif
        }
        return result;
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return Base::ip_from_string(address.begin(), address.end(), code, index);
    }

    base_type _bytes;
};

}

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
    return stream << to_string(ip);
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

}

#endif

#endif
