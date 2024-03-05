@page errors Error Handling

@tableofcontents

The library provides two ways to handle errors: through exceptions and through returned error codes. Which method to choose depends on the task and policy of your project (not everywhere it is possible to use exceptions; moreover, they may even be disabled in your build).

## Exception Handling

All methods not marked `noexcept` can throw exceptions; there are only a few such methods. The documentation labels them all, and also indicates the types of exceptions that can be thrown.

These are mainly methods for parsing from a string and some operations with networks from the previous section. 

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    try {
        ipv4_address::parse("192.257.2.0");
    } catch (const parse_error& exc) {
        std::cout << exc.what() << std::endl; // octet 1 of address 192.257.2.0 exceeded 255
    }
    return 0;
}
```

That's basically all you need to know about exception handling. The only thing I’ll clarify is that universal classes for addresses and networks (with union IPv4 and IPv6) first attempt to parse for IPv4, and then IPv6, which is why the error will contain information about the non-corect IPv6 (in case of exception).

## Handling Error Codes

If you do not use exceptions, then for all methods that can throw exceptions there are alternative methods that return an error code.

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {

    error_code err;
    auto ip = ipv4_address::parse("192.257.2.0", err);

    if (err != error_code::NoError) {
        std::cout << "invalid ip" << std::endl;
        return 1;
    }
    std::cout << "ip " << ip << std::endl;

    return 0;
}
```

## Disable Exceptions

Also, your compiler may have exceptions disabled. In this case, you can also disable exceptions in the library.

@note The library supports the work without any exceptions, if necessary. To disable code that may throw exceptions, pass the `IPADDRESS_NO_EXCEPTIONS` definition when building.

After this, the library will not throw exceptions when errors occur. It is recommended in this case to use error codes to handle errors.

This is what the behavior will be if you pass this flag.

```cpp
#include <iostream>
 
#include <ipaddress/ipaddress.hpp>
 
using namespace ipaddress;
 
int main() {
    //try {
        auto empty = ipv4_address::parse("192.257.2.0");
        std::cout << empty << std::endl;
    // } catch (const parse_error& exc) { // exception will not be thrown
    //     std::cout << exc.what() << std::endl;
    // }

    error_code err;
    auto ip = ipv4_address::parse("192.257.2.0", err);

    if (err != error_code::NoError) { // this will work as expected
        std::cout << "invalid ip" << std::endl;
        return 1;
    }
    std::cout << "ip " << ip << std::endl;

    return 0;
}
```

@note Keep in mind that if the `IPADDRESS_NO_EXCEPTIONS` flag is defined, then not all compilers can produce compile-time errors for constexpr **before** C++20 for parsing expressions like `constexpr auto ip = ipv4_address::parse("127.0.0.257");`

@htmlonly

<div class="cards">

<div class="card">
  <a href="operations.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Network Operations</h2>
    <p>Back to the section with operations and manipulations on networks.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="build.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Build Guide</h2>
    <p>Learn how to build the library, unit tests and documentation locally for development purposes.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
