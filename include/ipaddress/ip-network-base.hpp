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
                netmask = ++it;
                has_slash = true;

            }
            if (!has_slash) {
                address[symbol++] = *it;
            }
        }
        auto prefix = Base::max_prefixlen;
        if (has_slash && netmask == str.end()) {
            code = error_code::EMPTY_NETMASK;
            return ip_network_base<Base>();
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

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORK_BASE_HPP
