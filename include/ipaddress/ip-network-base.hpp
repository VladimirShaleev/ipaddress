#ifndef IPADDRESS_IP_NETWORK_BASE_HPP
#define IPADDRESS_IP_NETWORK_BASE_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Base>
class ip_network_base : public Base {
public:
    using ip_address_type = typename Base::ip_address_type;

    IPADDRESS_CONSTEXPR ip_network_base() IPADDRESS_NOEXCEPT 
        : 
        _address(),
        _netmask(ip_address_type::ip_from_prefix(Base::max_prefixlen)),
        _hostmask(),
        _prefixlen(Base::max_prefixlen) {
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

    IPADDRESS_NODISCARD std::string to_string(format fmt = format::compressed) const {
        return _address.to_string(fmt) + '/' + std::to_string(_prefixlen);
    }

    IPADDRESS_CONSTEXPR void swap(ip_network_base& network) IPADDRESS_NOEXCEPT {
        _address.swap(network._address);
        _netmask.swap(network._netmask);
        _hostmask.swap(network._hostmask);
        std::swap(_prefixlen, network._prefixlen);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const ip_address_type& address() const IPADDRESS_NOEXCEPT {
        return _address;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const ip_address_type& netmask() const IPADDRESS_NOEXCEPT {
        return _netmask;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const ip_address_type& hostmask() const IPADDRESS_NOEXCEPT {
        return _hostmask;
    }
    
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR size_t prefixlen() const IPADDRESS_NOEXCEPT {
        return _prefixlen;
    }
    
    IPADDRESS_NODISCARD explicit operator std::string() const {
        return to_string();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator==(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return _address == rhs._address && _netmask == rhs._netmask;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator!=(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        return !(*this == rhs);
    }

#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR std::strong_ordering operator<=>(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (auto result = _address <=> rhs._address; result != std::strong_ordering::equivalent) {
            return result;
        }
        return _netmask <=> rhs._netmask;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR bool operator<(const ip_network_base& rhs) const IPADDRESS_NOEXCEPT {
        if (_address != rhs._address) {
            return _address < rhs._address;
        }
        if (_netmask != rhs._netmask) {
            return _netmask < rhs.netmask;
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
        char address[ip_address_type::max_string_len + 1] = {};
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
        result._address = ip_address_type::parse(address, code);
        result._netmask = std::get<0>(netmask_result);
        result._hostmask = std::get<1>(netmask_result);
        result._prefixlen = std::get<2>(netmask_result);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        result._address = ip_address_type::strict_netmask(result._address, result._netmask, strict, code);

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        return result;
    }

    ip_address_type _address;
    ip_address_type _netmask;
    ip_address_type _hostmask;
    size_t _prefixlen;
};

#ifndef IPADDRESS_NO_OVERLOAD_STD

inline int network_stream_index() { 
    static int i = std::ios_base::xalloc();
    return i;
}

inline std::istream& strict(std::istream& stream) {
    stream.iword(network_stream_index()) = 0;
    return stream;
}

inline std::istream& not_strict(std::istream& stream) {
    stream.iword(network_stream_index()) = 1;
    return stream;
}

#endif // IPADDRESS_NO_OVERLOAD_STD

} // IPADDRESS_NAMESPACE

#ifndef IPADDRESS_NO_OVERLOAD_STD

namespace std {

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
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_stream_index());
    auto fmt = iword
        ? (IPADDRESS_NAMESPACE::format) (iword - 1) 
        : IPADDRESS_NAMESPACE::format::compressed;
    iword = 0;
    return stream << network.to_string(fmt);
}

template <typename Base>
inline std::istream& operator>>(std::istream& stream, IPADDRESS_NAMESPACE::ip_network_base<Base>& network) {
    auto& iword = stream.iword(IPADDRESS_NAMESPACE::network_stream_index());
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
