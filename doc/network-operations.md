@page operations Network Operations

@tableofcontents

Here describes various ways to operate with networks and addresses, for example how to get all hosts on a given network, or exclude one network from another, etc.

@note All iteration operations in this library are implemented with lazy evaluation. That is, the list is not generated in advance; instead, each element is generated upon request.

## Enumerating network addresses

Below is a demonstration of how to get all hosts on a given network

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    for (const auto& addr : ip_network::parse("192.0.2.0/29").hosts()) {
        std::cout << addr << std::endl;
    }
    // 192.0.2.1
    // 192.0.2.2
    // 192.0.2.3
    // 192.0.2.4
    // 192.0.2.5
    // 192.0.2.6

    // All operations in this library support constexpr
    constexpr auto hosts_sequence = ip_network::parse("192.0.2.0/29").hosts();
    constexpr auto last_host = hosts_sequence.back(); // can even iterate through it in a loop
    std::cout << last_host << std::endl; // 192.0.2.6

    return 0;
}
```

@warning Please note that with IPv6, the number of addresses can be so large that iterating through them all may be practically impossible. Therefore, use the `hosts()` method cautiously to avoid endlessly retrieving addresses.

@note For ipv4 all the IP addresses that belong to the network, except the network address itself and the network broadcast address. For networks with a mask length of 31, the network address and network broadcast address are also included in the result. Networks with a mask of 32 will return a list containing the single host address. <br> For ipv6 all the IP addresses that belong to the network, except the Subnet-Router anycast address. For networks with a mask length of 127, the Subnet-Router anycast address is also included in the result. Networks with a mask of 128 will return a list containing the single host address.

## Supernet and Subnets

Get the subnets that are combined to create the current network definition.

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    std::cout << ip_network::parse("192.0.2.0/24").supernet(2) << std::endl; // 192.0.0.0/22

    for (const auto& subnet : ip_network::parse("192.0.2.0/24").subnets(2)) {
        std::cout << subnet << std::endl;
    }
    // 192.0.2.0/26
    // 192.0.2.64/26
    // 192.0.2.128/26
    // 192.0.2.192/26

    return 0;
}
```

## Removing one network from another network

Calculates the network definitions that arise from subtracting the specified network from the current one.

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    constexpr auto a = ip_network::parse("192.0.2.0/28");
    constexpr auto b = ip_network::parse("192.0.2.1/32");

    for (const auto& net : a.address_exclude(b)) {
       std::cout << net << std::endl;
    }
    // 192.0.2.8/29
    // 192.0.2.4/30
    // 192.0.2.2/31
    // 192.0.2.0/32
 
    return 0;
}
```

## Other operations

This library does not overload arithmetic operators for IP addresses and networks. But what if they need address arithmetic to resolve your problems? You can use integer arithmetic for this.

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    constexpr auto ip1 = ipv4_address::parse("192.0.2.0");
    constexpr auto ip2 = ipv6_address::parse("2001:db8::");
    constexpr auto ip3 =   ip_address::parse("2001:db8::");

    // Convert to and from unsigned integer.
    // 
    // Please also note that if the original IPv6 has a scope id 
    // and you need it, it will need to be copied separately
    //
    // Also don't forget to account for integer overflow if it 
    // might arise in your situation
    //
    constexpr auto new_ip1 = ipv4_address::from_uint((std::uint32_t) ip1 + 1000);
    constexpr auto new_ip2 = ipv6_address::from_uint((    uint128_t) ip2 + 1);
    constexpr auto new_ip3 =   ip_address::from_uint((    uint128_t) ip3 + 1);

    std::cout << new_ip1 << std::endl; // 192.0.5.232
    std::cout << new_ip2 << std::endl; // 2001:db8::1
    std::cout << new_ip3 << std::endl; // 2001:db8::1

    return 0;
}
```

@htmlonly

<div class="cards">

<div class="card">
  <a href="tutorial.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Tutorial</h2>
    <p>Back to tutorial.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="errors.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Error Handling</h2>
    <p>To learn how to handle errors using exceptions, and what to do if you are not allowed to use exceptions in your product or have them disabled.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
