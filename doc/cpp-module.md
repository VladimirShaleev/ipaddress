@page module Cpp Module (C++20 and newer)

@tableofcontents

This library can be integrated as a C++ module. C++ modules, introduced in the C++20 standard, not only expedite the compilation process but also make the use of C++ more natural.

To leverage these benefits, ensure you have the right toolset, which includes a modern compiler, a compatible build system, and CMake version 3.28 or later (or 3.26 with extensions).

## Using the Module {#import-module}

By default, the target assembly for the C++ module is not enabled. To activate it, you need to set the `-DIPADDRESS_BUILD_MODULE=ON` flag in your configuration parameters or explicitly specify it in your CMake file.

If your compiler is compatible with the **C++20** standard or newer, you can import the `ipaddress` module as shown below (there are many restrictions for using C++ modules, read about them [in the next section](#restrictions)):

```cmake
cmake_minimum_required(VERSION 3.28.0)

project(my-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23) # or: set(CMAKE_CXX_STANDARD 20)

option(IPADDRESS_BUILD_MODULE "Build Cpp module" ON)
add_subdirectory(third-party/ipaddress)

add_executable(my-project main.cpp)
target_link_libraries(my-project PRIVATE ipaddress::ipaddress-module) # add the module to your target 
set_target_properties(my-project PROPERTIES CXX_SCAN_FOR_MODULES ON)
set_target_properties(my-project PROPERTIES CXX_EXTENSIONS OFF)
```

After which you can import the module in your source files:

```cpp
#include <iostream>

import ipaddress;

int main() {
    auto ip = ipaddress::ipv4_address::parse<"127.0.0.1">();
    std::cout << ip << std::endl;
    return 0;
}
```

@note Currently, the `ipaddress::ipaddress-module` is not available via package managers such as Vcpkg or Conan. This is primarily due to the absence of established best practices for integrating C++ modules into package distributions.

## Restrictions on the use of modules {#restrictions}

Considerations for the use of C++ modules include compiler support and build system requirements. The following compilers are known to support C++ modules:

* Clang 16 or later;
* MSVC 17.4 or later;
* GCC 14 or later.

CMake version 3.28 or higher is required, although version 3.26 may suffice if extensions are enabled. Currently, CMake offers support for C++ modules with these specific generators:

* Ninja and Ninja Multi-Config;
* Visual Studio.

Additionally, there are notable considerations:

* CMake is presently capable of exporting targets with C++ modules for subsequent imports, but only with the Ninja and Ninja Multi-Config generators;
* On Windows, certain issues have been observed when utilizing modules with Clang;
* It is generally acknowledged that most editors lack comprehensive support for modules. Consequently, when using such editors, functionalities like IntelliSense may not perform reliably.

@note In essence, it is crucial to recognize that the ecosystem for C++ modules and build systems is in a state of ongoing development. The structuring for their support is only beginning to take shape. Furthermore, the best practices for integrating these modules into distributable packages managed by package managers are still being formulating.

@warning The current landscape of C++ modules presents several challenges. It's crucial to assess their suitability for your project. If your product requires compilation across a broad range of platforms and toolsets, including older versions, modules might not be the optimal choice. Conversely, if your product is tailored for a specific platform and you have a consistent toolset, utilizing C++ modules, including those from the standard library, could greatly enhance compilation efficiency.

## Additional links {#cpp-module-articles}

For more insights into integrating modules in your C++ projects, consider exploring these articles:
* https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/
* https://www.kitware.com/import-cmake-the-experiment-is-over/

@htmlonly

<div class="cards">

<div class="card">
  <a href="errors.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Error Handling</h2>
    <p>Back to error handling.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="benchmark.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Benchmark</h2>
    <p>Comparison of IP address parsing performance with library boost asio and sockets API.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
