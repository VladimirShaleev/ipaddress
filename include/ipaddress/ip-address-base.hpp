#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
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
    static constexpr ip_address_base<Base> parse() noexcept {
        constexpr auto str = FixedString;
        return ip_address_base(Base::ip_from_string(str));
    }
#endif

private:
    BaseType _ip;
};

}

#endif
