@mainpage Get Started

@tableofcontents

[![GitHub Release](https://img.shields.io/github/v/release/vladimirshaleev/ipaddress?sort=semver&display_name=tag)](https://github.com/VladimirShaleev/ipaddress/releases)
[![GitHub License](https://img.shields.io/github/license/vladimirshaleev/ipaddress)](https://github.com/VladimirShaleev/ipaddress/blob/main/LICENSE)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/vladimirshaleev/ipaddress/tests.yml?branch=main&logo=github&label=tests)
](https://github.com/VladimirShaleev/ipaddress/actions/workflows/tests.yml)

## Introduction

This cross-platfrom **header-only** library is inspired by the [ipaddress API in Python](https://docs.python.org/3.12/library/ipaddress.html), 
from which it derives its name. It aims to be simpler to use due to its familiar interface. However, the C++ implementation takes 
a different approach: it uses **static polymorphism** through the strategy pattern instead of dynamic polymorphism to handle 
differences between IP versions (IPv4 and IPv6). This design choice eliminates the overhead of dynamic calls and virtual tables. 
For instance, an instance of the **ipv4_address** class will be represented by 4 bytes.

The library leverages modern C++ features, ensuring that all IP address and network operations support constant expressions. 
For more details on constant expressions, refer to the relevant section **TODO**.

![Constexpr](img/constexpr.png "Errors are known at Compile Time")

## Compatibility

The library has been tested on the following compilers:

* Clang 6.0.1 and newer;
* Apple Clang 13.0.0 and newer;
* GCC 7.5.0 and newer;
* MSVC 14.29 (Visual Studio 16.11) and newer

## Licensing

```
MIT License

Copyright (c) 2024 Vladimir Shaleev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
