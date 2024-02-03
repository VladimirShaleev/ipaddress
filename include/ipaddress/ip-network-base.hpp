#ifndef IPADDRESS_IP_NETWORK_BASE_HPP
#define IPADDRESS_IP_NETWORK_BASE_HPP

#include "ip-address-iterator.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Base>
class ip_network_base : public Base {
public:
    using ip_address_type = typename Base::ip_address_type;

    IPADDRESS_CONSTEXPR ip_network_base() IPADDRESS_NOEXCEPT 
        : 
        _network_address(),
        _netmask(ip_address_type::ip_from_prefix(Base::_max_prefixlen)),
        _prefixlen(Base::_max_prefixlen) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString, bool Strict = true>
    IPADDRESS_NODISCARD static consteval ip_network_base parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = parse_address_with_prefix(str, Strict, code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return result;
    }
#endif // IPADDRESS_NONTYPE_TEMPLATE_PARAMETER

#if IPADDRESS_CPP_VERSION >= 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(std::string_view address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    static IPADDRESS_CONSTEXPR ip_network_base parse(std::string_view address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#else // IPADDRESS_CPP_VERSION < 17

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static ip_network_base parse(const std::string& address, bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_address_with_prefix(address, strict);
    }

    static ip_network_base parse(const std::string& address, error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto index = 0;
        return parse_address_with_prefix(address, strict, code, index);
    }

#endif // IPADDRESS_CPP_VERSION < 17

    template <size_t N>
    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base parse(const char(&address)[N], bool strict = true) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto str = make_fixed_string(address);
        return parse_address_with_prefix(str, strict);
    }

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_network_base parse(const char(&address)[N], error_code& code, bool strict = true) IPADDRESS_NOEXCEPT {
        auto str = make_fixed_string(address);
        auto index = 0;
        return parse_address_with_prefix(str, strict, code, index);
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base from_address(
        const ip_address_type& address, 
        size_t prefixlen = ip_address_type::max_prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto result = from_address(address, code, prefixlen, strict);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                char str[ip_address_type::_max_string_len + 1]{};
                const auto len = address.ip_to_chars(address.bytes(), format::compressed, str);
                raise_error(code, 0, str, len);
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            char str[ip_address_type::_max_string_len + 1]{};
            const auto len = address.ip_to_chars(address.bytes(), format::compressed, str);
            raise_error(code, 0, str, len);
        #endif
        }
        return result;
    }

    IPADDRESS_NODISCARD_WHEN_NO_EXCEPTIONS static IPADDRESS_CONSTEXPR ip_network_base from_address(
        const ip_address_type& address, 
        error_code& code, 
        size_t prefixlen = ip_address_type::max_prefixlen, 
        bool strict = true
    ) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        code = error_code::NO_ERROR;

        const auto netmask = ip_address_type::ip_from_prefix(prefixlen); 

        ip_network_base result;
        result._network_address = ip_address_type::strict_netmask(address, netmask, strict, code);

        if (code != error_code::NO_ERROR) {
            return result;
        }

        result._netmask = netmask;
        result._prefixlen = prefixlen;
        return result;
    }

    IPADDRESS_NODISCARD IPADDRESS_FORCE_INLINE IPADDRESS_CONSTEXPR bool contains(const ip_address_type& address) const IPADDRESS_NOEXCEPT {
        const auto& ip_bytes = address.bytes();
        const auto& address_bytes = network_address().bytes();
        const auto& netmask_bytes = netmask().bytes();

        for (size_t i = 0; i < ip_address_type::_size; ++i) {
            if ((ip_bytes[i] & netmask_bytes[i]) != address_bytes[i]) {
                return false;
            }
        }
        return true;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool overlaps(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return other.contains(network_address()) || other.contains(broadcast_address()) || contains(other.network_address()) || contains(other.broadcast_address());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool subnet_of(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return is_subnet_of(*this, other);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool supernet_of(const ip_network_base& other) const IPADDRESS_NOEXCEPT {
        return is_subnet_of(other, *this);
    }

    IPADDRESS_NODISCARD std::string to_string(format fmt = format::compressed) const {
        return _network_address.to_string(fmt) + '/' + std::to_string(_prefixlen);
    }

    IPADDRESS_CONSTEXPR void swap(ip_network_base& network) IPADDRESS_NOEXCEPT {
        _network_address.swap(network._network_address);
        _netmask.swap(network._netmask);
        auto tmp = _prefixlen;
        _prefixlen = network._prefixlen;
        network._prefixlen = tmp;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t hash() const IPADDRESS_NOEXCEPT {
        return calc_hash(_network_address.hash(), _netmask.hash());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const ip_address_type& network_address() const IPADDRESS_NOEXCEPT {
        return _network_address;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR ip_address_type broadcast_address() const IPADDRESS_NOEXCEPT {
        typename ip_address_type::base_type bytes {};
        const auto& network_address_bytes = network_address().bytes();
        const auto hostmask_bytes = hostmask().bytes();
        for (size_t i = 0; i < ip_address_type::_size; ++i) {
            bytes[i] = uint8_t(network_address_bytes[i] | hostmask_bytes[i]);
        }
        return ip_address_type(bytes);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const ip_address_type& netmask() const IPADDRESS_NOEXCEPT {
        return _netmask;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR ip_address_type hostmask() const IPADDRESS_NOEXCEPT {
        const auto& netmask_bytes = netmask().bytes();
        typename ip_address_type::base_type bytes {};
        for (size_t i = 0; i < ip_address_type::_size; ++i) {
            bytes[i] = uint8_t(netmask_bytes[i] ^ 0xFF);
        }
        return ip_address_type(bytes);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return _prefixlen;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_multicast() const IPADDRESS_NOEXCEPT {
        return network_address().is_multicast() && broadcast_address().is_multicast();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_private() const IPADDRESS_NOEXCEPT;

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_global() const IPADDRESS_NOEXCEPT {
        return !is_private();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_reserved() const IPADDRESS_NOEXCEPT {
        return network_address().is_reserved() && broadcast_address().is_reserved();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_loopback() const IPADDRESS_NOEXCEPT {
        return network_address().is_loopback() && broadcast_address().is_loopback();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_link_local() const IPADDRESS_NOEXCEPT {
        return network_address().is_link_local() && broadcast_address().is_link_local();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_unspecified() const IPADDRESS_NOEXCEPT {
        return network_address().is_unspecified() && broadcast_address().is_unspecified();
    }

    // IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_t addresses_count() const IPADDRESS_NOEXCEPT {
    //     return ip_address_type::addresses_count(broadcast_address(), network_address());
    // }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE hosts_sequence<ip_address_type> hosts() const IPADDRESS_NOEXCEPT {
        return hosts_sequence<ip_address_type>(network_address(), broadcast_address());
    }

    IPADDRESS_NODISCARD explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return _network_address == rhs._network_address && _netmask == rhs._netmask;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (auto result = _network_address <=> rhs._network_address; result != std::strong_ordering::equivalent) {
            return result;
        }
        return _netmask <=> rhs._netmask;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (_network_address != rhs._network_address) {
            return _network_address < rhs._network_address;
        }
        if (_netmask != rhs._netmask) {
            return _netmask < rhs._netmask;
        }
        return false;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return rhs < *this;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(rhs < *this);
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator>=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this < rhs);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_network_base parse_address_with_prefix(const Str& str, bool strict) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = parse_address_with_prefix(str, strict, code, index);
        if (code != error_code::NO_ERROR) {
            if (IPADDRESS_IS_CONST_EVALUATED(code)) {
                raise_error(code, index, str.data(), str.size());
            }
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, str.data(), str.size());
        #endif
        }
        return result;
    }

    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_network_base parse_address_with_prefix(const Str& str, bool strict, error_code& code, int& index) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        
        auto has_slash = false;
        auto netmask = str.end();
        auto symbol = 0;
        char address[ip_address_type::_max_string_len + 1] = {};
        for (auto it = str.begin(); it != str.end(); ++it) {
            if (*it == '/') {
                if (has_slash) {
                    code = error_code::ONLY_ONE_SLASH_PERMITTED;
                    return ip_network_base<Base>();
                }
                if (it + 1 == str.end()) {
                    code = error_code::EMPTY_NETMASK;
                    return ip_network_base<Base>();
                }
                netmask = it + 1;
                has_slash = true;
            }
            if (!has_slash) {
                address[symbol++] = *it;
            }
        }

        auto netmask_result = ip_address_type::parse_netmask(netmask, str.end(), code, index);
        
        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        ip_network_base<Base> result;
        result._network_address = ip_address_type::parse(address, code);
        result._netmask = std::get<0>(netmask_result);
        result._prefixlen = std::get<1>(netmask_result);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        result._network_address = ip_address_type::strict_netmask(result._network_address, result._netmask, strict, code);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        return result;
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool is_subnet_of(const ip_network_base& lhs, const ip_network_base& rhs) IPADDRESS_NOEXCEPT {
        return rhs.network_address() <= lhs.network_address() && rhs.broadcast_address() >= lhs.broadcast_address();
    }

    ip_address_type _network_address;
    ip_address_type _netmask;
    size_t _prefixlen;
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

inline int network_strict_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

inline std::istream& strict(std::istream& stream) {
    stream.iword(network_strict_index()) = 0;
    return stream;
}

inline std::istream& not_strict(std::istream& stream) {
    stream.iword(network_strict_index()) = 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

template <typename Base>
struct hash<IPADDRESS_NAMESPACE::ip_network_base<Base>> {
    IPADDRESS_CONSTEXPR std::size_t operator()(const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) const IPADDRESS_NOEXCEPT {
        return network.hash();
    }
};

template <typename Base>
inline IPADDRESS_CONSTEXPR void swap(IPADDRESS_NAMESPACE::ip_network_base<Base>& net1, IPADDRESS_NAMESPACE::ip_network_base<Base>& net2) IPADDRESS_NOEXCEPT {
    return net1.swap(net2);
}

template <typename Base>
inline std::string to_string(const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    return network.to_string();
}

template <typename Base>
inline std::ostream& operator<<(std::ostream& stream, const IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    return stream << network.to_string(fmt);
}

template <typename Base>
inline std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_strict_index());
    auto strict = iword == 0;
    iword = 0;

    std::string str;
    stream >> str;
    IPADDRESS_NAMESPACE::error_code err;
    network = IPADDRESS_NAMESPACE::ip_network_base<Base>::parse(str, err, strict);
    if (err != IPADDRESS_NAMESPACE::error_code::NO_ERROR) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

} // std

#endif // IPADDRESS_NO_OVERLOAD_STD

#endif // IPADDRESS_IP_NETWORK_BASE_HPP
