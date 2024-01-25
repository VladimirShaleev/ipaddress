#ifndef IPADDRESS_IP_NETWORK_BASE_HPP
#define IPADDRESS_IP_NETWORK_BASE_HPP

namespace IPADDRESS_NAMESPACE {

template <typename Base, typename IpAddress>
class ip_network_base : public Base {
public:

private:
    IpAddress _network_address;
    IpAddress _netmask;
};

}

#endif
