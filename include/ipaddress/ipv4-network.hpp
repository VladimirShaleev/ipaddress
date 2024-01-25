#ifndef IPADDRESS_IPV4_NETWORK_HPP
#define IPADDRESS_IPV4_NETWORK_HPP

#include "ipv4-address.hpp"
#include "ip-network-base.hpp"

namespace IPADDRESS_NAMESPACE {

class network_v4 {
public:
};

using ipv4_network = ip_network_base<network_v4, ipv4_address>;

}

#endif
