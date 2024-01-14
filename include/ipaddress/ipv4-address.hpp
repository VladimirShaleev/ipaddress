#ifndef IPADDRESS_IPV4_ADDRESS_HPP
#define IPADDRESS_IPV4_ADDRESS_HPP

#include "ip-address-base.hpp"

namespace IPADDRESS_NAMESPACE {

class base_v4 {
protected:
    typedef uint32_t BaseType;

private:
};

using ipv4_address = ip_address_base<base_v4>;
    
}

#endif
