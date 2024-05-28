@page module Cpp Module (C++20 and newer)

@tableofcontents

The library can be integrated as a C++ module. C++ modules, introduced in the C++20 standard, significantly speed up compilation and make C++ code more natural.

To take advantage of this, you need to have the necessary set of tools. This requires a modern compiler, a supported build system, and CMake 3.28 (or 3.26 with extensions enabled) or newer.

## Using the module {#import-module}

If your compiler supports the **C++20** standard and higher, you can import the **ipaddress** module as follows (there are many restrictions for using C++ modules, read about them [below](#restrictions)):

```cmake
cmake_minimum_required(VERSION 3.28.0)

project(my-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23) # or: set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ipaddress CONFIG REQUIRED)

add_executable(my-project main.cpp)
target_link_libraries(my-project PRIVATE ipaddress::ipaddress-module) # add the module to your target 

# You can also set the minimum required standard 
# for your target, instead of:
#   target_compile_features(my-project PRIVATE cxx_std_20)
# or
#   target_compile_features(my-project PRIVATE cxx_std_23)

# Also depending on your build you may need to 
# add the following properties:
#   set_target_properties(my-project PROPERTIES CXX_SCAN_FOR_MODULES ON)
#   set_target_properties(my-project PROPERTIES CXX_EXTENSIONS OFF)
```

After which you can import the module in your source files:

```cpp
#include <iostream> // or if supported: import std;

import ipaddress;

int main() {
    auto ip = ipaddress::ipv4_address::parse<"127.0.0.1">();
    std::cout << ip << std::endl;
    return 0;
}
```

## Restrictions on the use of modules {#restrictions}

Let's look at some restrictions on the use of modules. There are at least the following compilers that support C++ modules:

* Clang 16 and newer;
* MSVC 17.4 and newer;
* GCC 14 and newer.

Requires CMake version 3.28 or later (or 3.26 with extensions enabled). At the same time, CMake supports working with C++ modules only for the following generators:

* Ninja and Ninja Multi-Config;
* Visual Studio.

There are also some features, such as:

* CMake can currently export targets with C++ modules only for Ninja and Ninja Multi-Config generators; 
* on Windows there are some problems when using modules with Clang;
* It's safe to say that most editors do not have full module support. When using them IntelliSense and etc. technology maybe break.

@note In other words, it’s important to understand that C++ modules and build systems are still under development and are just starting to be structured for support. Also, best practices for their integration into various packages have not yet been established.

@warning As you can see, there are numerous limitations to using C++ modules. Therefore, you should decide whether it is feasible to use them in your project. If your product needs to be compiled on as many platforms as possible with various toolsets, including not the most up-to-date versions, then perhaps modules are not the choice for you. However, if it is strictly defined that your product is being developed for a specific platform and you have a certain set of tools that you use, then C++ modules (including the standard library modules) can significantly speed up the compilation of your projects.

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
