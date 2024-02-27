# ipaddress

[![GitHub Release](https://img.shields.io/github/v/release/vladimirshaleev/ipaddress?sort=semver&display_name=tag)](https://github.com/VladimirShaleev/ipaddress/releases)
[![GitHub License](https://img.shields.io/github/license/vladimirshaleev/ipaddress)](https://github.com/VladimirShaleev/ipaddress/blob/main/LICENSE)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/vladimirshaleev/ipaddress/tests.yml?branch=main&logo=github&label=tests)
](https://github.com/VladimirShaleev/ipaddress/actions/workflows/tests.yml)

A library for working and manipulating IPv4/IPv6 addresses and networks

> THE LIBRARY IS CURRENTLY UNDER ACTIVE DEVELOPMENT AND NOT YET READY FOR USE!

This **header-only** library is inspired by the [ipaddress API in Python](https://docs.python.org/3.12/library/ipaddress.html), from which it derives its name. It aims to be simpler to use due to its familiar interface. However, the C++ implementation takes a different approach: it uses **static polymorphism** through the strategy pattern instead of dynamic polymorphism to handle differences between IP versions (IPv4 and IPv6). This design choice eliminates the overhead of dynamic calls and virtual tables. For instance, an instance of the **ipv4_address** class will be represented by 4 bytes.

The library leverages modern C++ features, ensuring that all IP address and network operations support constant expressions. For more details on constant expressions, refer to the relevant section **TODO**.

![Constexpr](doc/img/constexpr.png "Errors are known at Compile Time")
