#ifndef IPADDRESS_IP_ANY_NETWORK_HPP
#define IPADDRESS_IP_ANY_NETWORK_HPP

#include "ipv4-network.hpp"
#include "ipv6-network.hpp"
#include "ip-any-address.hpp"

namespace IPADDRESS_NAMESPACE {

class ip_network {
public:
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_version version() const IPADDRESS_NOEXCEPT {
        return _version;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.prefixlen() : _ipv_net.ipv6.prefixlen();
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address network_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.network_address()) : ip_address(_ipv_net.ipv6.network_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address broadcast_address() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.broadcast_address()) : ip_address(_ipv_net.ipv6.broadcast_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address netmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.netmask()) : ip_address(_ipv_net.ipv6.netmask());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_address hostmask() const IPADDRESS_NOEXCEPT {
        return is_v4() ? ip_address(_ipv_net.ipv4.hostmask()) : ip_address(_ipv_net.ipv6.hostmask());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_multicast() : _ipv_net.ipv6.is_multicast();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_private() : _ipv_net.ipv6.is_private();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_global() : _ipv_net.ipv6.is_global();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_reserved() : _ipv_net.ipv6.is_reserved();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_loopback() : _ipv_net.ipv6.is_loopback();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_link_local() : _ipv_net.ipv6.is_link_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.is_unspecified() : _ipv_net.ipv6.is_unspecified();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_site_local() const IPADDRESS_NOEXCEPT {
        return is_v4() ? false : _ipv_net.ipv6.is_site_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v4() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V4;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_v6() const IPADDRESS_NOEXCEPT {
        return _version == ip_version::V6;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t size() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.size() : _ipv_net.ipv6.size();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t hash() const IPADDRESS_NOEXCEPT {
        return is_v4() ? _ipv_net.ipv4.hash() : _ipv_net.ipv6.hash();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool contains(const ip_address& address) const IPADDRESS_NOEXCEPT {
        if (_version == address.version()) {
            return is_v4() ? _ipv_net.ipv4.contains(address.v4().value()) : _ipv_net.ipv6.contains(address.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool overlaps(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.overlaps(other.v4().value()) : _ipv_net.ipv6.overlaps(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool subnet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.subnet_of(other.v4().value()) : _ipv_net.ipv6.subnet_of(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool supernet_of(const ip_network& other) const IPADDRESS_NOEXCEPT {
        if (_version == other.version()) {
            return is_v4() ? _ipv_net.ipv4.supernet_of(other.v4().value()) : _ipv_net.ipv6.supernet_of(other.v6().value());
        } else {
            return false;
        }
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv4_network> v4() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V4) {
            auto net = _ipv_net.ipv4;
            return optional<ipv4_network>(std::move(net));
        }
        return optional<ipv4_network>();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional<ipv6_network> v6() const IPADDRESS_NOEXCEPT {
        if (_version == ip_version::V6) {
            auto net = _ipv_net.ipv6;
            return optional<ipv6_network>(std::move(net));
        }
        return optional<ipv6_network>();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv4_network& net4) IPADDRESS_NOEXCEPT : _ipv_net(net4), _version(ip_version::V4) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network(const ipv6_network& net6) IPADDRESS_NOEXCEPT : _ipv_net(net6), _version(ip_version::V6) {
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network from_address(
        const ip_address& address, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), prefixlen, strict));
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network from_address(
        const ip_address& address, 
        error_code& code, 
        size_t prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT {
        return address.is_v4() 
            ? ip_network(ipv4_network::from_address(address.v4().value(), code, prefixlen, strict)) 
            : ip_network(ipv6_network::from_address(address.v6().value(), code, prefixlen, strict));
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE std::string to_string(format fmt = format::compressed) const {
        return is_v4() ? _ipv_net.ipv4.to_string(fmt) : _ipv_net.ipv6.to_string(fmt);
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE void swap(ip_network& net) IPADDRESS_NOEXCEPT {
        const auto tmp = *this;
        *this = net;
        net = tmp;
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString, bool Strict = true>
    IPADDRESS_NODISCARD static consteval IPADDRESS_FORCE_INLINE ip_network parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;

        char net[str.size() + 1]{};
        for (size_t i = 0; i < str.size(); ++i) {
            net[i] = str[i];
        }

        const auto net4 = ipv4_network::parse(net, code, Strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net4);
        }
        
        return ip_network(ipv6_network::parse<FixedString, Strict>());
    }

#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::wstring_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u16string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u32string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::wstring_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u16string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u32string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u8string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(std::u8string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

#endif // __cpp_char8_t

#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::wstring& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::u16string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::u32string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    static IPADDRESS_FORCE_INLINE ip_network parse(const std::string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_FORCE_INLINE ip_network parse(const std::wstring& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_FORCE_INLINE ip_network parse(const std::u16string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

    static IPADDRESS_FORCE_INLINE ip_network parse(const std::u32string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

#if __cpp_char8_t >= 201811L

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_FORCE_INLINE ip_network parse(const std::u8string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address, strict);
    }

    static IPADDRESS_FORCE_INLINE ip_network parse(const std::u8string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        return parse_string(address, code, strict);
    }

#endif // __cpp_char8_t

#endif // IPADDRESS_CPP_VERSION < 17

    template <typename T, size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(const T(&address)[N], bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        internal::is_char_type<T>();
        auto code = error_code::NO_ERROR;
        auto result = parse_string(address, code, strict);
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
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse(const T(&address)[N], error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        internal::is_char_type<T>();
        code = error_code::NO_ERROR;

        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net4);
        }
        
        const auto net6 = ipv6_network::parse(address, code, strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net6);
        }
        
        return ip_network();
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE explicit operator std::string() const {
        return is_v4() ? _ipv_net.ipv4.to_string() : _ipv_net.ipv6.to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version != rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 == rhs._ipv_net.ipv4) : (_ipv_net.ipv6 == rhs._ipv_net.ipv6);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (const auto result = _version <=> rhs._version; result == std::strong_ordering::equivalent) {
            return _version == ip_version::V4 ? (_ipv_net.ipv4 <=> rhs._ipv_net.ipv4) : (_ipv_net.ipv6 <=> rhs._ipv_net.ipv6);
        } else {
            return result;
        }
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        if (_version < rhs._version) {
            return true;
        }
        if (_version > rhs._version) {
            return false;
        }
        return _version == ip_version::V4 ? (_ipv_net.ipv4 < rhs._ipv_net.ipv4) : (_ipv_net.ipv6 < rhs._ipv_net.ipv6);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const ip_network& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse_string(const Str& address, bool strict) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net4);
        }
        return ip_network(ipv6_network::parse(address, strict));
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network parse_string(const Str& address, error_code& code, bool strict) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        const auto net4 = ipv4_network::parse(address, code, strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net4);
        }
        
        const auto net6 = ipv6_network::parse(address, code, strict);
        if (code == error_code::NO_ERROR) {
            return ip_network(net6);
        }
        
        return ip_network();
    }

    union ip_any_network {
        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network() IPADDRESS_NOEXCEPT : ipv4() {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network(const ipv4_network& net) IPADDRESS_NOEXCEPT : ipv4(net) {
        }

        IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_any_network(const ipv6_network& net) IPADDRESS_NOEXCEPT : ipv6(net) {
        }

        ipv4_network ipv4;
        ipv6_network ipv6;
    } _ipv_net {};
    ip_version _version = ip_version::V4;
};

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    template <fixed_string FixedString>
    IPADDRESS_NODISCARD consteval IPADDRESS_FORCE_INLINE ip_network operator""_net() IPADDRESS_NOEXCEPT {
        return ip_network::parse<FixedString>();
    }

#else // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE ip_network operator""_net(const char* address, std::size_t size) IPADDRESS_NOEXCEPT {
        assert(size <= ipv6_address::base_max_string_len * 2 + 1 && "literal string is too long");
        char str[ipv6_address::base_max_string_len * 2 + 2] = {};
        for (size_t i = 0; i < size; ++i) {
            str[i] = address[i];
        }
        return ip_network::parse(str);
    }

#endif // !IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

} // namespace IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <>
struct hash<IPADDRESS_NAMESPACE::ip_network> {
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::size_t operator()(const IPADDRESS_NAMESPACE::ip_network& network) const IPADDRESS_NOEXCEPT {
        return network.hash();
    }
};

IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(IPADDRESS_NAMESPACE::ip_network& net1, IPADDRESS_NAMESPACE::ip_network& net2) IPADDRESS_NOEXCEPT {
    return net1.swap(net2);
}

IPADDRESS_FORCE_INLINE std::string to_string(const IPADDRESS_NAMESPACE::ip_network& network) {
    return network.to_string();
}

IPADDRESS_FORCE_INLINE std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_network& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    auto str = network.to_string(fmt);
    if (stream.flags() & ios_base::uppercase) {
        auto end = std::find(str.cbegin(), str.cend(), '%');
        std::transform(str.cbegin(), end, str.begin(), [](auto c){
            return std::toupper(c);
        });
    }
    return stream << str;
}

IPADDRESS_FORCE_INLINE std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_network& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_strict_index());
    auto strict = iword == 0;
    iword = 0;

    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    network = IPADDRESS_NAMESPACE::ip_network::parse(str, err, strict);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // namespace std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_ANY_NETWORK_HPP
