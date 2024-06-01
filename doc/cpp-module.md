@page module Cpp Module (C++20 and newer)

@tableofcontents

The library can be integrated as a C++ module. C++ modules, introduced in the C++20 standard, significantly speed up compilation and make C++ code more natural.

To take advantage of this, you need to have the necessary set of tools. This requires a modern compiler, a supported build system, and CMake 3.28 (or 3.26 with extensions enabled) or newer.

## Using the module {#import-module}

By default, the assembly of the target for C++ module is disabled. To turn on, it is necessary to set the `-DIPADDRESS_BUILD_MODULE=ON` configuration parameter or set it clearly in the CMake file.

If your compiler supports the **C++20** standard and higher, you can import the **ipaddress** module as follows (there are many restrictions for using C++ modules, read about them [below](#restrictions)):

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

@note At the moment, the goal of `ipaddress::ipaddress-module` is not provided through package-managers like Vcpkg or Conan, because there is still no Best Practice on the integration of C++ modules into packages.

## Restrictions on the use of modules {#restrictions}

Let's look at some restrictions on the use of modules. There are at least the following compilers that support C++ modules:

* Clang 16 and newer;
* MSVC 17.4 and newer;
* GCC 14 and newer.

Requires CMake version 3.28 or later (or 3.26 with extensions enabled). At the same time, CMake supports working with C++ modules only for the following generators:

* Ninja and Ninja Multi-Config;
* Visual Studio.

There are also some features, such as:

* CMake can currently export targets with C++ modules for subsequent imports only Ninja and Ninja Multi-Config generators; 
* On Windows there are some problems when using modules with Clang;
* It's safe to say that most editors do not have full module support. When using them IntelliSense and etc. technology maybe break.

@note In other words, it’s important to understand that C++ modules and build systems are still under development and are just starting to be structured for support. Also, best practices for their integration into various packages have not yet been established.

@warning As you can see, there are numerous limitations to using C++ modules. Therefore, you should decide whether it is feasible to use them in your project. If your product needs to be compiled on as many platforms as possible with various toolsets, including not the most up-to-date versions, then perhaps modules are not the choice for you. However, if it is strictly defined that your product is being developed for a specific platform and you have a certain set of tools that you use, then C++ modules (including the standard library modules) can significantly speed up the compilation of your projects.

## Additional links {#cpp-module-articles}

Additional information on the integration with modules can be found in the following articles:
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
