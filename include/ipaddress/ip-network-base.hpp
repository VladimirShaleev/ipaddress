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
        _network_address(),
        _netmask(),
        _prefixlen(0) {
    }

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_network_base<Base> parse(const char(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        auto str = make_fixed_string(address);
        auto index = 0;
        return parse_address_with_prefix(str, code, index);
    }

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_network_base<Base> parse_address_with_prefix(const Str& str, error_code& code, int& index) IPADDRESS_NOEXCEPT {
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

        auto netmask_result = Base::parse_netmask(netmask, str.end(), code, index);
        
        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        ip_network_base<Base> result;
        result._network_address = ip_address_type::parse(address, code);
        result._netmask = ip_address_type(netmask_result.first.bytes());
        result._prefixlen = netmask_result.second;

        if (code != error_code::NO_ERROR) {
            return ip_network_base<Base>();
        }

        auto pack_address = result._network_address.to_uint32();
        auto pack_netmask = result._netmask.to_uint32();
        if ((pack_address & pack_netmask) != pack_address) {
            bool strict = true;
            if (strict) {
                code = error_code::HAS_HOST_BITS_SET;
                return ip_network_base<Base>();
            } else {
                result._network_address = ipv4_address::from_uint32(pack_address & pack_netmask);
            }
        }
        return result;
    }

    ip_address_type _network_address;
    ip_address_type _netmask;
    size_t _prefixlen;
};

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IP_NETWORK_BASE_HPP
