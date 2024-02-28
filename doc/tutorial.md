@page tutorial Tutorial

@tableofcontents

## IP addresses

For working with IP addresses, the library provides three classes:

- **ipv4_address** — A class for working with IPv4 addresses, where the instance size is always 4 bytes.
- **ipv6_address** — A class for working with IPv6 addresses, where the instance size includes both the space allocated for the IPv6 address itself and the scope id (zone index). As a result, the instance size will be 16 bytes plus the maximum length of the scope id. Read about scope ids below.
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
#include <iostream>

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
    constexpr auto ip2 = ipv6_address::parse("fe80::1ff:fe23:4567:890a%eth2");
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

### From uint/To uint

Below is code demonstrating how to create IP addresses from unsigned integers and convert them back to unsigned integers.

```cpp
#include <iostream>

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

### Comparison

## Scope Id

The library supports **Scope Id** both as numeric values and as strings.

By default, **Scope Id** are enabled, and their maximum size is 16 characters (defined by `IPADDRESS_IPV6_SCOPE_MAX_LENGTH` during compilation). Yes, the **scope id** is internally represented as a fixed-size array, and its maximum size cannot be changed dynamically. This means that you cannot store a **scope id** longer than `IPADDRESS_IPV6_SCOPE_MAX_LENGTH`. There are several reasons for this design choice: simplicity in storing **scope id**, avoiding additional allocations (all data for the IP address will be as local as possible in memory), and enabling working with **scope id** in a **constexpr** manner.

@note If you are certain that you don't need **Scope Id**, you can completely disable them to avoid any overhead related to their maintenance. This can be done by defining `IPADDRESS_NO_IPV6_SCOPE` during compilation. In this case, the instance size of `ipv6_address` and `ip_address` will be 16 bytes, and all operations related to retrieving or modifying the **scope id** will have no effect.

## Std overrides

TODO

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
