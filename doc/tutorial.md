@page tutorial Tutorial

@tableofcontents

## IP addresses

For working with IP addresses, the library provides three classes:

- **ipv4_address** — A class for working with IPv4 addresses, where the instance size is always 4 bytes.
- **ipv6_address** — A class for working with IPv6 addresses, where the instance size includes both the space allocated for the IPv6 address itself and the scope id (zone index). As a result, the instance size will be 16 bytes plus the maximum length of the scope id (plus aligned bytes if any). Read about scope ids below.
- **ip_address** — Combines the `ipv4_address` and `ipv6_address` classes via a union. This ensures version-independent IP address manipulation. It has implicit constructors for converting from `ipv4_address` and `ipv6_address`. The instance size is the same as that of `ipv6_address`.

@parblock
@note Regardless of which class you use, the internal address is stored as an array of bytes, not as an unsigned integer. This design choice ensures that the address is stored in network byte order (big-endian) regardless of the platform. This is convenient because ultimately, this address can be used in sockets or similar libraries that require passing addresses in network byte order.
@endparblock
@parblock
@note All operations on addresses support compile-time computations. If an address is specified as a constant, during compilation, we get: errors if the address was entered incorrectly, the resulting IP address represented as bytes, and any other operations on addresses.
@endparblock

### Parsing ip strings

Let's look at examples of obtaining IP addresses from string literals and strings.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    // Parsing string literals for IPv4 and IPv6. Note that IPv6 addresses
    // in string form can be specified in various formats. The following addresses are equivalent:
    //    - fe80::1ff:fe23:4567:890a%eth2
    //    - FE80::1FF:FE23:4567:890A%eth2
    //    - fe80:0:0:0:1ff:fe23:4567:890a%eth2
    //    - fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2
    //
    // Thanks to constexpr, for example, ip1 will be expanded into an array of bytes during compilation,
    // and parsing will not occur at runtime. It can be presented like this for clarity:
    //    constexpr const ipaddress::ipv4_address ip1 =
    //        {{{}, {{(unsigned char)192U, (unsigned char)168U, (unsigned char)1U, (unsigned char)1U}}}};
    //
    constexpr auto ip1 = ipv4_address::parse("192.168.1.1");
    constexpr auto ip2 = ipv6_address::parse("64:ff9b::192.0.2.128");
    constexpr auto ip3 =   ip_address::parse("fe80::1ff:fe23:4567:890a%eth2"); // any version address

    // Parsing string literals for IPv4 and IPv6 with the new syntax starting from C++20
    // (Literal Classes as Non-type Template Parameters)
    constexpr auto ip4 = ipv4_address::parse<"192.168.1.1">();
    constexpr auto ip5 = ipv6_address::parse<"fe80::1ff:fe23:4567:890a%eth2">();
    constexpr auto ip6 =   ip_address::parse<"192.168.1.1">(); // any version address

    // Parsing with User-defined literals
    constexpr auto ip7 = "192.168.1.1"_ipv4;
    constexpr auto ip8 = "fe80::1ff:fe23:4567:890a%eth2"_ipv6;
    constexpr auto ip9 = "fe80::1ff:fe23:4567:890a%eth2"_ip; // any version address

    // Parsing at runtime. For example, when a string comes from an input 
    // field during execution or from any other source.
    const char*       str1 = "192.168.1.1";
    const std::string str2 = "fe80::1ff:fe23:4567:890a%eth2";
    auto ip10 = ipv4_address::parse(str1);
    auto ip11 = ipv6_address::parse(str2);
    auto ip12 =   ip_address::parse(str2); // any version address

    return 0;
}
```

@remark For compile-time computations, the **ipaddress** library uses relaxed `constexpr`, which was introduced only in C++14. Therefore, for C++11, the **ipaddress** library does not support constant expressions. It is recommended to use the C++14 language standard or newer versions.

Access to specific versions of IP addresses for ip_address is demonstrated below.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    // With ipv4_address and ipv6_address everything is clear, 
    // everything is known from their types
    constexpr auto ipv4 = ipv4_address::parse("192.168.1.1");
    constexpr auto ipv6 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth2");
    constexpr auto ipv4_version = ipv4.version(); // V4
    constexpr auto ipv6_version = ipv6.version(); // V6
    constexpr auto ipv4_size    = ipv4.size(); // 4
    constexpr auto ipv6_size    = ipv6.size(); // 16

    // But that's not the case for ip_address
    constexpr auto ip1 = ip_address::parse("192.168.1.1");
    constexpr auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    // Below is how to distinguish between different versions of IP
    constexpr auto ip1_version = ip1.version(); // V4
    constexpr auto ip2_version = ip2.version(); // V6
    constexpr auto ip1_size    = ip1.size(); // 4
    constexpr auto ip2_size    = ip2.size(); // 16

    // Check which IP version is stored in ip_address for ip2
    constexpr auto ip2_is_v4 = ip2.is_v4(); // false
    constexpr auto ip2_is_v6 = ip2.is_v6(); // true

    // Access to specific versions of IP addresses for ip2
    constexpr auto ip2_get_ipv4 = ip2.v4();
    constexpr auto ip2_get_ipv6 = ip2.v6();

    if (ip2_get_ipv4) { // false
        constexpr auto result = ip2_get_ipv4.value();
    }
    if (ip2_get_ipv6.has_value()) { // true
        constexpr auto result = ip2_get_ipv6.value(); // get ipv6
    }
    return 0;
}
```

### From uint/To uint

Below is code demonstrating how to create IP addresses from unsigned integers and convert them back to unsigned integers.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    // Unsigned integers in host endianness
    constexpr ipv4_address::uint_type u1 = 0xC0A80001;         // std::uint32_t
    constexpr ipv6_address::uint_type u2 = 281470681743360ULL; // ipaddress::uint128_t

    constexpr auto ip1 = ipv4_address::from_uint(u1);
    constexpr auto ip2 = ipv6_address::from_uint(u2);
    constexpr auto ip3 =   ip_address::from_uint(u1); // any version address

    // Get unsigned integers in host endianness
    //
    // Explicit conversions are also available:
    //   - constexpr auto value1 =        (std::uint32_t) ip1;
    //   - constexpr auto value2 = (ipaddress::uint128_t) ip2;
    //   - constexpr auto value3 =        (std::uint32_t) ip3;
    constexpr auto value1 = ip1.to_uint();
    constexpr auto value2 = ip2.to_uint();
    constexpr auto value3 = ip3.to_uint32(); // ip_address does not define to_uint to avoid accidental errors

    return 0;
}
```

@note `ipaddress` defines its type `uint128_t` to work with 128-bit unsigned integers. It is designed to fill the gap in the C++ standard, which does not natively support 128-bit integers across all platforms. Unlike compiler-specific extensions like `__int128`, `uint128_t` ensures compatibility and portability across different compilers and architectures. <br> The implementation is inspired by the algorithms used in the .NET framework's [UInt128](https://source.dot.net/#System.Private.CoreLib/src/libraries/System.Private.CoreLib/src/System/UInt128.cs), providing a reliable foundation for arithmetic operations and other integer-related functionalities.

### Working with bytes

Below is an example of working with bytes and pointers to bytes directly.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    // Creating IP addresses from byte arrays of a fixed size
    constexpr auto ip1 = ipv4_address::from_bytes({ 0xC0, 0xA8, 0x00, 0x01 });
    constexpr auto ip2 = ipv6_address::from_bytes(ipv6_bytes);
    constexpr auto ip3 =   ip_address::from_bytes(ipv6_bytes); // any version address

    // Creation from a pointer to bytes. 
    //
    //    If the number of bytes is less than the target number  of bytes to represent 
    //    the IP address, the missing bytes will be filled with zeros.
    //    
    //    If the number of bytes is greater than the target number of bytes represented 
    //    by the IP address, then the extra bytes will be ignored.
    // 
    constexpr std::uint8_t ipv4_bytes[] = { 0xC0, 0xA8, 0x00, 0x01 };
    constexpr auto iv4 = ipv4_address::from_bytes(ipv4_bytes, 3);
    constexpr auto iv5 = ipv6_address::from_bytes(ipv4_bytes, 3);
    constexpr auto ip6 =   ip_address::from_bytes(ipv4_bytes, 3,  ip_version::V4);
    constexpr auto ip7 =   ip_address::from_bytes(ipv6_bytes.data(), 11, ip_version::V6);

    // bytes() provides read access to fixed-length byte arrays
    // 
    //    A fixed-size byte array is represented by the byte_array type.
    //    It provides full functionality for byte access and iterators.
    //    std::array is not used because before C++17 it was not able to work 
    //    with constant expressions for iterators, etc.
    constexpr auto ip1_byte_count = ip1.bytes().size(); // 4
    constexpr auto ip2_byte_count = ip2.bytes().size(); // 16
    constexpr auto ip3_byte_count = ip3.is_v4() 
        ? ip3.v4().value().bytes().size() 
        : ip3.v6().value().bytes().size();

    // data() provides a constant pointer to a byte array
    constexpr auto test1 = *(ip1.data() + 1);  // 168
    constexpr auto test2 = *(ip2.data() + 10); // 255
    constexpr auto test3 = *(ip3.data() + 11); // 255
    
    return 0;
}
```

### Comparison

For classes for working with IP addresses, comparison operators and spaceship operator (for C++20 and newer) have been redefined.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto ip1 = ipv6_address::parse("fe80::1ff:fe23:4567:890a");
    constexpr auto ip2 = ipv6_address::parse("fe80::1ff:fe23:4567:890b");

    constexpr auto test1 = ip1 == ip2; // false
    constexpr auto test2 = ip1 != ip2; // true
    constexpr auto test3 = ip1 <  ip2; // true
    constexpr auto test4 = ip1 <= ip2; // true
    constexpr auto test5 = ip1 >= ip2; // false
    constexpr auto test6 = ip1 >  ip2; // false

    // If scope id is specified, it will also be taken into account when comparing
    constexpr auto ip3 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth2");
    constexpr auto ip4 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth1");
    
    constexpr auto test7  = ip2 == ip3; // false
    constexpr auto test8  = ip2 != ip3; // true
    constexpr auto test9  = ip2 <  ip3; // false
    constexpr auto test10 = ip2 <= ip3; // false
    constexpr auto test11 = ip2 >= ip3; // true
    constexpr auto test12 = ip2 >  ip3; // true
    constexpr auto test13 = ip3 == ip4; // false
    constexpr auto test14 = ip3 != ip4; // true
    constexpr auto test15 = ip3 <  ip4; // false
    constexpr auto test16 = ip3 <= ip4; // false
    constexpr auto test17 = ip3 >= ip4; // true
    constexpr auto test18 = ip3 >  ip4; // true

    return 0;
}
```

### Properties

The library provides many properties for determining certain features of an IP address. Let's look at the example below (all possible properties are not presented here; for a more complete picture, see the [documentation for the code](classipaddress_1_1ip__address.html#pub-methods)).

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto test1 = ipv6_address::parse("ffff::").is_multicast(); // true
    constexpr auto test2 =   ip_address::parse("fdff::").is_multicast(); // false

    constexpr auto test3 = ipv4_address::parse("240.0.0.1").is_reserved(); // true
    constexpr auto test4 =   ip_address::parse("239.255.255.255").is_reserved(); // false

    constexpr auto ip = ip_address::parse("2002:ac1d:2d64::1");
    constexpr auto sixtofour = ip.sixtofour(); // get sixtofour

    if (sixtofour) { // true
        constexpr auto result = sixtofour.value();
    }
    return 0;
}
```

### Convert to string

There are three different formats for converting IP addresses to strings as you can see below.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    // For IP addresses version 4, the format has no effect
    constexpr auto ip1 = ipv4_address::parse("127.240.0.1");
    auto str1 = ip1.to_string();                   // equivalent to format::compressed
    auto str2 = ip1.to_string(format::full);       // 127.240.0.1
    auto str3 = ip1.to_string(format::compact);    // 127.240.0.1
    auto str4 = ip1.to_string(format::compressed); // 127.240.0.1

    constexpr auto ip2 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth2");
    auto str5 = ip2.to_string();                   // equivalent to format::compressed 
    auto str6 = ip2.to_string(format::full);       // fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2
    auto str7 = ip2.to_string(format::compact);    // fe80:0:0:0:1ff:fe23:4567:890a%eth2
    auto str8 = ip2.to_string(format::compressed); // fe80::1ff:fe23:4567:890a%eth2

    auto reverse_pointer = ip2.reverse_pointer(); // a.0.9.8.7.6.5.4.3.2.e.f.f.f.1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.e.f.ip6.arpa
 
    return 0;
}
```

## IP networks

- **ipv4_network** — A class for working with IPv4 networks.
- **ipv6_network** — A class for working with IPv6 networks.
- **ip_network** — Combines the `ipv4_network` and `ipv6_network` classes via a union. This ensures version-independent IP address manipulation. It has implicit constructors for converting from `ipv4_network` and `ipv6_network`. The instance size is the same as that of `ipv6_network`.

Networks store two IP addresses (network address and netmask), as well as the prefix length. This means that the size of a network instance will be `2 * sizeof(<ip_type>) + sizeof(size_t)`.

Working with networks is similar to working with addresses. So let's get straight to the examples.

### Parsing ip network strings

Let's look at examples of obtaining networks from string literals and strings.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    // Parsing string literals for IPv4 and IPv6
    //
    // For ipv4: 
    //    if the mask (portion after the / in the argument) is provided in dotted 
    //    quad form, it is considered a netmask if it begins with a non-zero field 
    //    (e.g., /255.0.0.0 corresponds to a /8 netmask). Conversely, if it starts 
    //    with a zero field (e.g., 0.255.255.255), it is treated as a hostmask for 
    //    the /8 prefix. The only exception is an all-zero mask, which is treated 
    //    as a netmask equivalent to /0. If no mask is specified, the default is /32
    constexpr auto net1 = ipv4_network::parse("1.2.3.0/24");
    constexpr auto net2 = ipv6_network::parse("2001:db8::/32");
    constexpr auto net3 =   ip_network::parse("1.2.3.4/255.255.255.255"); // any version address

    // Parsing string literals for IPv4 and IPv6 with the new syntax starting from C++20
    // (Literal Classes as Non-type Template Parameters)
    constexpr auto net4 = ipv4_network::parse<"1.2.3.0/24">();
    constexpr auto net5 = ipv6_network::parse<"2001:db8::/32">();
    constexpr auto net6 =   ip_network::parse<"1.2.3.4/255.255.255.255">(); // any version address

    // Parsing with User-defined literals
    constexpr auto net7 = "1.2.3.0/24"_ipv4_net;
    constexpr auto net8 = "2001:db8::/32"_ipv6_net;
    constexpr auto net9 = "1.2.3.4/255.255.255.255"_net; // any version address

    // Parsing at runtime. For example, when a string comes from an input 
    // field during execution or from any other source.
    const char*       str1 = "1.2.3.0/24";
    const std::string str2 = "2001:db8::/32";
    auto net10 = ipv4_network::parse(str1);
    auto net11 = ipv6_network::parse(str2);
    auto net12 =   ip_network::parse(str2); // any version address

    return 0;
}
```

@note If the prefixlen is not present in the input string, then the length of the prefix will be equal to the maximum length (for ipv4 it is 32, and for ipv6 it is 128)

We can request various network parameters.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto net1 = ip_network::parse("2001:db8::/32");
    constexpr auto network_address1 = net1.network_address(); // 2001:db8::
    constexpr auto broadcast_address1 = net1.broadcast_address(); // 2001:db8:ffff:ffff:ffff:ffff:ffff:ffff
    constexpr auto netmask1 = net1.netmask(); // ffff:ffff::
    constexpr auto hostmask1 = net1.hostmask(); // ::ffff:ffff:ffff:ffff:ffff:ffff
    constexpr auto prefixlen1 = net1.prefixlen(); // 32

    constexpr auto net2 = ip_network::parse("1.2.3.0/255.255.255.0");
    constexpr auto network_address2 = net2.network_address(); // 1.2.3.0
    constexpr auto broadcast_address2 = net2.broadcast_address(); // 1.2.3.255
    constexpr auto netmask2 = net2.netmask(); // 255.255.255.0
    constexpr auto hostmask2 = net2.hostmask(); // 0.0.0.255
    constexpr auto prefixlen2 = net2.prefixlen(); // 24

    // Non strict mode sample
    constexpr auto net3 = ip_network::parse("1.2.3.0/16", false);
    constexpr auto network_address3 = net3.network_address(); // 1.2.0.0
    constexpr auto broadcast_address3 = net3.broadcast_address(); // 1.2.255.255
    constexpr auto netmask3 = net3.netmask(); // 255.255.0.0
    constexpr auto hostmask3 = net3.hostmask(); // 0.0.255.255
    constexpr auto prefixlen3 = net3.prefixlen(); // 16

    return 0;
}
```

### Create from address

A network can be created from an IP address and a prefixlen.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto ip4 = ipv4_address::parse("192.168.0.1");
    constexpr auto ip6 = ipv6_address::parse("2002:ac1d:2d64::");
    constexpr auto ip_any = ip_address::parse("2002:ac1d:2d64::");

    constexpr ipv4_network net1 = ipv4_network::from_address(ip4, 24, false); // non strict mode sample
    constexpr ipv6_network net2 = ipv6_network::from_address(ip6, 64);
    constexpr   ip_network net3 =   ip_network::from_address(ip4, 32);
    constexpr   ip_network net4 =   ip_network::from_address(ip6, 64);
    constexpr   ip_network net5 =   ip_network::from_address(ip_any, 64);
    
    return 0;
}
```

### Comparison

For classes for working with IP networks, comparison operators and spaceship operator (for C++20 and newer) have been redefined.

```cpp
#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto net1 = ip_network::parse("2002:ac1d:2d64::/64");
    constexpr auto net2 = ip_network::parse("2002:ac1d:2d64::"); // this is the same as 2002:ac1d:2d64::/128

    constexpr auto test1 = net1 == net2; // false
    constexpr auto test2 = net1 != net2; // true
    constexpr auto test3 = net1 <  net2; // true
    constexpr auto test4 = net1 <= net2; // true
    constexpr auto test5 = net1 >= net2; // false
    constexpr auto test6 = net1 >  net2; // false

    constexpr auto net3 = ip_network::parse("2002:ac1d:2d64::/64");
    constexpr auto net4 = ip_network::parse("2002:ac1d:2d64::%1/64");
    
    constexpr auto test7  = net2 == net3; // false
    constexpr auto test8  = net2 != net3; // true
    constexpr auto test9  = net2 <  net3; // false
    constexpr auto test10 = net2 <= net3; // false
    constexpr auto test11 = net2 >= net3; // true
    constexpr auto test12 = net2 >  net3; // true
    constexpr auto test13 = net3 == net4; // false
    constexpr auto test14 = net3 != net4; // true
    constexpr auto test15 = net3 <  net4; // true
    constexpr auto test16 = net3 <= net4; // true
    constexpr auto test17 = net3 >= net4; // false
    constexpr auto test18 = net3 >  net4; // false

    return 0;
}
```

### Properties

The library provides many properties for determining certain features of an IP network. Let's look at the example below (all possible properties are not presented here; for a more complete picture, see the [documentation for the code](classipaddress_1_1ip__network.html#pub-methods)).

As you can see, getting network properties is not much different from getting address properties.

```cpp
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    constexpr auto test1 = ipv6_network::parse("ffff::").is_multicast(); // true
    constexpr auto test2 =   ip_network::parse("fdff::").is_multicast(); // false
 
    constexpr auto test3 = ipv4_network::parse("240.0.0.1").is_reserved(); // true
    constexpr auto test4 =   ip_network::parse("239.255.255.255").is_reserved(); // false
 
    return 0;
}
```

### Convert to string

There are three different formats for converting IP networks to strings as you can see below.

```cpp
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    // For IP addresses version 4, the format has no effect
    constexpr auto net1 = ip_network::parse("1.2.3.0/0.0.0.255");
    auto str1 = net1.to_string(); // 1.2.3.0/24

    constexpr auto net2 = ip_network::parse("2001:db8::/32");
    auto str2 = net2.to_string();                   // equivalent to format::compressed
    auto str3 = net2.to_string(format::full);       // 2001:0db8:0000:0000:0000:0000:0000:0000/32
    auto str4 = net2.to_string(format::compact);    // 2001:db8:0:0:0:0:0:0/32
    auto str5 = net2.to_string(format::compressed); // 2001:db8::/32
 
    return 0;
}
```

## Scope Id

The library supports **Scope Id** both as numeric values and as strings.

By default, **Scope Id** are enabled, and their maximum size is 16 characters (defined by `IPADDRESS_IPV6_SCOPE_MAX_LENGTH` during compilation). Yes, the **scope id** is internally represented as a fixed-size array, and its maximum size cannot be changed dynamically. This means that you cannot store a **scope id** longer than `IPADDRESS_IPV6_SCOPE_MAX_LENGTH`. There are several reasons for this design choice: simplicity in storing **scope id**, avoiding additional allocations (all data for the IP address will be as local as possible in memory), and enabling working with **scope id** in a **constexpr** manner.

@note If you are certain that you don't need **Scope Id**, you can completely disable them to avoid any overhead related to their maintenance. This can be done by defining `IPADDRESS_NO_IPV6_SCOPE` during compilation. In this case, the instance size of `ipv6_address` and `ip_address` will be 16 bytes, and all operations related to retrieving or modifying the **scope id** will have no effect.

For IP addresses `ipv6_address` and `ip_address`, functionality is supported for obtaining and changing scope id.

```cpp
#include <iostream>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto ip1 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth2");
    constexpr auto scope_id = ip1.get_scope_id();

    if (scope_id) {
        constexpr auto is_integer = scope_id.has_uint32(); // false
        constexpr auto is_string = scope_id.has_string(); // true
        std::cout << "scope id is integer for ip1: " << std::boolalpha << is_integer << std::endl;
        std::cout << "scope id is string for ip1: " << std::boolalpha << is_string << std::endl;
        std::cout << "scope id for ip1: " << scope_id.get_string() << std::endl;
    }

    // If IPv4 is stored in ip_address, then methods for working with the scope id have no effect
    auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a");
    std::cout << "has scope id for ip2: " << std::boolalpha << (bool) ip2.get_scope_id() << std::endl;

    ip2.set_scope_id("123");
    auto test_scope_id = ip2.get_scope_id();

    if (test_scope_id) {
        auto is_integer = test_scope_id.has_uint32(); // true
        auto is_string = test_scope_id.has_string(); // true
        std::cout << "scope id is integer for ip2: " << std::boolalpha << is_integer << std::endl;
        std::cout << "scope id is string for ip2: " << std::boolalpha << is_string << std::endl;
        std::cout << "scope id for ip2: " << test_scope_id.get_uint32() << std::endl;
    }

    return 0;
}
```

## Std overrides

@note If, for some reason, you don't want the library to overload standard functions, you can define `IPADDRESS_NO_OVERLOAD_STD` during compilation.

The library overloads some functions from the standard library so that IP addresses and networks (`ipv4_address`, `ipv6_address`, `ip_address`, `ipv4_network`, `ipv6_network` and `ip_network`) can be used in hash tables and other standard operations on `std::swap`, etc.

```cpp
#include <iostream>
#include <unordered_map>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

int main() {
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");
    constexpr auto ip3 = ip_address::parse("2001:db8::1%scope");

    // Added partial specialization for hash calculation
    std::unordered_map<ip_address, int> container;
    container[ip1] = 1;
    container[ip2] = 2;
    container[ip3] = 3;
    for (const auto& [key, value] : container) {
        std::cout << key << ": " << value << std::endl;
    }

    // Overloaded operators for parsing and writing to streams (char type only)
    std::cout << ip2 << std::endl; // 2001:db8::1
    std::cout << full << ip2 << std::endl; // 2001:0db8:0000:0000:0000:0000:0000:0001
    std::cout << compact << ip2 << std::endl; // 2001:db8:0:0:0:0:0:1
    std::cout << compressed << ip2 << std::endl; // 2001:db8::1

    ip_address r1, r2;
    std::istringstream ss1("255.0.42.42 test");
    std::istringstream ss2("2001:db8:0:0:0:0:0:1%scope test");
    ss1 >> r1;
    ss2 >> r2;
    std::cout << "has errors: " << std::boolalpha << (ss1.fail() || ss2.fail()) << std::endl; // has errors: false
    std::cout << r1 << std::endl; // 255.0.42.42
    std::cout << r2 << std::endl; // 2001:db8::1%scope

    ip_network n1, n2;
    std::istringstream ss3("1.2.3.0/16 non strict mode sample");
    std::istringstream ss4("2001:db8::/32 test");
    ss3 >> non_strict >> n1; // non strict mode sample
    ss4 >> n2;
    std::cout << "has errors: " << std::boolalpha << (ss3.fail() || ss4.fail()) << std::endl; // has errors: false
    std::cout << n1 << std::endl; // 1.2.0.0/16
    std::cout << n2 << std::endl; // 2001:db8::/32

    return 0;
}
```

@htmlonly

<div class="cards">

<div class="card">
  <a href="index.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Get Started</h2>
    <p>To come back to the beginning.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="operations.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Network Operations</h2>
    <p>To learn about operations and manipulations on networks and address arithmetic.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
