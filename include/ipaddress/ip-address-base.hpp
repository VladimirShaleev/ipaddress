#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Base>
class ip_address_base : public Base {
public:
    using BaseType = typename Base::BaseType;

    constexpr explicit ip_address_base(const BaseType& ip) noexcept : _ip(ip) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString>
    static consteval ip_address_base<Base> parse() noexcept {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto octet = 0;
        auto result = Base::ip_from_string(str, code, octet);
        if (code != error_code::NO_ERROR) {
            raise_error(code, octet, str);
        }
        return ip_address_base(result);
    }
#endif

    template <size_t N>
    static constexpr ip_address_base<Base> parse(const char(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto octet = 0;
        auto str = make_fixed_string(address);
        auto result = Base::ip_from_string(str, code, octet);
        if (code != error_code::NO_ERROR) {
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, octet, str);
        #endif
        }
        return ip_address_base(result);
    }

    template <size_t N>
    static constexpr ip_address_base<Base> parse(const char(&address)[N], error_code& code) noexcept {
        code = error_code::NO_ERROR;
        auto octet = 0;
        return ip_address_base(Base::ip_from_string(make_fixed_string(address), code, octet));
    }

private:
    BaseType _ip;
};

}

#endif
