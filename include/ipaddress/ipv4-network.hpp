#ifndef IPADDRESS_IPV4_NETWORK_HPP
#define IPADDRESS_IPV4_NETWORK_HPP

#include "ipv4-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

class ipv4_network_base : public base_v4<ipv4_network_base> {
public:
    using ip_address_type = ipv4_address;
}; // ipv4_network_base

using ipv4_network = ip_network_base<ipv4_network_base>;

} // IPADDRESS_NAMESPACE

#endif // IPADDRESS_IPV4_NETWORK_HPP
