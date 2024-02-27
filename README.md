# ipaddress

[![GitHub Release](https://img.shields.io/github/v/release/vladimirshaleev/ipaddress?sort=semver&display_name=tag)](https://github.com/VladimirShaleev/ipaddress/releases)
[![GitHub License](https://img.shields.io/github/license/vladimirshaleev/ipaddress)](https://github.com/VladimirShaleev/ipaddress/blob/main/LICENSE)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/vladimirshaleev/ipaddress/tests.yml?branch=main&logo=github&label=tests)
](https://github.com/VladimirShaleev/ipaddress/actions/workflows/tests.yml)

A library for working and manipulating IPv4/IPv6 addresses and networks in modern C++.

## Introduction

This cross-platfrom **header-only** library is inspired by the [ipaddress API in Python](https://docs.python.org/3.12/library/ipaddress.html), 
from which it derives its name. It aims to be simpler to use due to its familiar interface. However, the C++ implementation takes 
a different approach: it uses **static polymorphism** through the strategy pattern instead of dynamic polymorphism to handle 
differences between IP versions (IPv4 and IPv6). This design choice eliminates the overhead of dynamic calls and virtual tables. 
For instance, an instance of the **ipv4_address** class will be represented by 4 bytes.

The library leverages modern C++ features, ensuring that all IP address and network operations support constant expressions. 
For more details on constant expressions, refer to the relevant section **TODO**.

![Constexpr](doc/img/constexpr.png "Errors are known at Compile Time")

## Compatibility

The library has been tested on the following compilers:

* Clang 6.0.1 and newer;
* Apple Clang 13.0.0 and newer;
* GCC 7.5.0 and newer;
* MSVC 14.29 (Visual Studio 16.11) and newer

## Installation



## Quick Start

The library provides capabilities for creating, analyzing and managing IPv4 and IPv6 addresses and networks using classes such 
as **ipv4_address**, **ipv6_address**, **ip_address** (any IP version), **ipv4_network**, **ipv6_network** and **ip_network** 
(any IP version). It also offers error handling methods with or without exceptions if for some reason exceptions are not used 
or disabled in your project.

To start using the library, simply include the header `ipaddress/ipaddress.hpp`, and you’re ready to begin your work!

Here is an example demonstrating some of the library's features (this example also demonstrates the new C++20 syntax for 
**Literal Classes as Non-type Template Parameters**, so a compiler with at least C++20 support is required to compile this example):

```cpp
#include <iostream>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

void parse_ip_sample() {
    constexpr auto ip = ipv6_address::parse("fec0::1ff:fe23:4567:890a%eth2");
    constexpr auto is_site_local = ip.is_site_local();

    std::cout << "ip " << ip << " is local: " << std::boolalpha << is_site_local << std::endl;
    std::cout << "DNS PTR " << ip.reverse_pointer() << std::endl << std::endl;
    
    // ipv4_address parser examples
    constexpr auto sample1 = ipv4_address::parse("127.0.0.1");
    constexpr auto sample2 = ipv4_address::parse<"127.0.0.1">();
    constexpr auto sample3 = "127.0.0.1"_ipv4;

    // ipv6_address parser examples
    constexpr auto sample4 = ipv6_address::parse("::");
    constexpr auto sample5 = ipv6_address::parse<"::">();
    constexpr auto sample6 = "::"_ipv6;

    // ip_address parser examples
    constexpr auto sample7 = ip_address::parse("127.0.0.1");
    constexpr auto sample8 = ip_address::parse<"::">();
    constexpr auto sample9 = "127.0.0.1"_ip;
}

void terade_sample() {
    constexpr auto teredo_ip = "2001:0000:4136:e378:8000:63bf:3fff:fdd2"_ipv6;
    auto [server, client] = teredo_ip.teredo().value();

    std::cout << "server: " << server << " and client: " << client << " for " << teredo_ip << std::endl << std::endl;
}

void subnets_sample() {
    constexpr auto net = ipv4_network::parse("192.0.2.0/24");

    std::cout << "subnets for " << net << ':' << std::endl;
    for (const auto subnet : net.subnets(2)) {
        std::cout << "  " << subnet << std::endl;
    }

    constexpr auto last_subnet = net.subnets(2).back();
    std::cout << "last subnet " << last_subnet << std::endl;
}

int main() {
    parse_ip_sample();
    terade_sample();
    subnets_sample();
    return 0;
}
```

> [!note]
> Note that to support the **Literal Classes as Non-type Template Parameters** syntax, a compiler 
> with support for **C++20** or higher is required

> [!note]
> Also note that the library uses relaxed constexpr for its operations, so they are not available in **C++11**
>
> ```cpp
> // C++14 and higher support constexpr
> constexpr auto ip = ipv6_address::parse("fec0::1ff:fe23:4567:890a%eth2");
> 
> // C++11 doesn't support relaxed constexpr, so they can't be used
> auto ip = ipv6_address::parse("fec0::1ff:fe23:4567:890a%eth2");
> ```

## Contributing

### Issues

### Workflow

### Licensing

ipaddress is licensed under the **MIT License**. See [LICENSE](https://github.com/VladimirShaleev/ipaddress/blob/main/LICENSE) for the full license text.
