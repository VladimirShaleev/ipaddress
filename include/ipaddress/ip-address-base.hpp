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

    IPADDRESS_CONSTEXPR explicit ip_address_base(const BaseType& ip) IPADDRESS_NOEXCEPT : _ip(ip) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString>
    static consteval ip_address_base<Base> parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(str, code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str);
        }
        return ip_address_base(result);
    }
#endif

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto str = make_fixed_string(address);
        auto result = Base::ip_from_string(str, code, index);
        if (code != error_code::NO_ERROR) {
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, str);
        #endif
        }
        return ip_address_base(result);
    }

    template <size_t N>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return ip_address_base(Base::ip_from_string(make_fixed_string(address), code, index));
    }

    IPADDRESS_CONSTEXPR BaseType ip() const IPADDRESS_NOEXCEPT {
        return _ip;
    }

private:
    BaseType _ip;
};

}

#endif
