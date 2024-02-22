/**
 * @file      ip-address-base.hpp
 * @brief     Provides basic functionality for IP addresses
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * This header file includes the core functionalities and definitions that are common to
 * both IPv4 and IPv6 address handling. It serves as a foundational component for building
 * specialized IP address classes and utilities.
 */

#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "endian.hpp"
#include "optional.hpp"
#include "byte-array.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * Enumerates the IP address versions.
 * 
 * Defines constants representing the two versions of Internet Protocol: IPv4 and IPv6.
 */
enum class ip_version {
    V4 = 4, /**< IPv4 version identifier. */
    V6 = 6 /**< IPv6 version identifier. */
};

/**
 * Enumerates the formatting options for IP address strings.
 * 
 * Defines the formatting styles that can be applied when converting IP addresses to strings,
 * such as full, compact, or compressed representations.
 */
enum class format {
    full = 0, /**< Full format with no compression or omission. */
    compact, /**< Compact format with possible omission of leading zeros. */
    compressed /**< Compressed format with maximal omission of segments or octets. */
};

/**
 * A template base class for IP address representations.
 * 
 * This class template serves as a base for creating IP address objects. It 
 * inherits from a base class that provides the necessary functionality, and 
 * it is extended by more specific IP address classes. 
 * 
 * @tparam Base the base class providing storage and low-level IP address functionalities
 */
template <typename Base>
class ip_address_base : public Base {
public:
    template <typename>
    friend class ip_network_base;

    using base_type = typename Base::base_type;
    using uint_type = typename Base::uint_type;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base() IPADDRESS_NOEXCEPT : Base() {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit ip_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : Base(bytes) {
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base from_bytes(const base_type& bytes) IPADDRESS_NOEXCEPT {
        return ip_address_base(bytes);
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base from_bytes(const uint8_t* bytes, size_t byte_count) IPADDRESS_NOEXCEPT {
        base_type data = {};
        for (size_t i = 0; i < Base::base_size && i < byte_count; ++i) {
            data[i] = bytes[i];
        }
        return ip_address_base(data);
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval IPADDRESS_FORCE_INLINE ip_address_base<Base> parse() IPADDRESS_NOEXCEPT {
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

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::wstring_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u16string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u32string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::wstring_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u16string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u32string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u8string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(std::u8string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // __cpp_char8_t

#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::wstring& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u16string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u32string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::wstring& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u16string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u32string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u8string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    static IPADDRESS_FORCE_INLINE ip_address_base parse(const std::u8string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }

#endif // __cpp_char8_t

#endif // IPADDRESS_CPP_VERSION < 17

    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(const T(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        return parse_string(str);
    }

    template <typename T, size_t N>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse(const T(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        auto str = make_fixed_string(address);
        return parse_string(str, code);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const uint8_t* data() const IPADDRESS_NOEXCEPT {
        return Base::bytes().data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        const auto& b = Base::bytes();
        for (size_t i = 0; i < Base::base_size; ++i) {
            if (b[i] != 0) {
                return false;
            }
        }
        return true;
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        char res[Base::base_max_string_len + 1]{};
        const auto len = Base::ip_to_chars(Base::bytes(), fmt, res);
        return std::string(res, len);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_address_base& ip) IPADDRESS_NOEXCEPT {
        Base::swap(*this, ip);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return Base::hash(Base::bytes());
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string reverse_pointer() const {
       return Base::ip_reverse_pointer(Base::bytes());
    }

    IPADDRESS_NODISCARD explicit IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE operator uint_type() const IPADDRESS_NOEXCEPT {
        return Base::to_uint();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::equals(*this, rhs);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::compare(*this, rhs);
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return Base::less(*this, rhs);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_address_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address_base parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return Base::ip_from_string(address.begin(), address.end(), code, index);
    }
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE int stream_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

IPADDRESS_FORCE_INLINE std::ostream& full(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::full) + 1;
    return stream;
}

IPADDRESS_FORCE_INLINE std::ostream& compact(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compact) + 1;
    return stream;
}

IPADDRESS_FORCE_INLINE std::ostream& compressed(std::ostream& stream) {
    stream.iword(stream_index()) = long(format::compressed) + 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <typename Base>
struct hash<IPADDRESS_NAMESPACE::ip_address_base<Base>> {
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) const IPADDRESS_NOEXCEPT {
        return ip.hash();
    }
};

template <typename Base>
IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_address_base<Base>& ip1, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip2) IPADDRESS_NOEXCEPT {
    ip1.swap(ip2);
}

template <typename Base>
IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    return ip.to_string();
}

template <typename Base>
IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
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

template <typename Base>
IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_address_base<Base>& ip) {
    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    ip = IPADDRESS_NAMESPACE::ip_address_base<Base>::parse(str, err);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ADDRESS_BASE_HPP
