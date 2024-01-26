#ifndef IPADDRESS_IPV6_NETWORK_HPP
#define IPADDRESS_IPV6_NETWORK_HPP

#include "ipv6-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv6_network_base : public base_v6<ipv6_network_base> {
public:
    using ip_address_type = ipv6_address;
}; // ipv6_network_base

using ipv6_network = ip_network_base<ipv6_network_base>;

} // IPADDRESS_NAMESPACE

#endif
