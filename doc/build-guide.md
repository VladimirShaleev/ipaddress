@page build Build Guide

@tableofcontents

Here's how to build the library locally. Technically, this is a header-only library and does not need to be compiled, but for development you will have to collect unit tests and documentation.

To work you will need:

* A compatible operating system (e.g. Linux, macOS, Windows);
* Compatible C++ compiler supporting C++11 or later (if you cannot test on all versions of the standard, then it’s okay, this [project runs tests](https://github.com/VladimirShaleev/ipaddress/actions/workflows/tests.yml) for all versions of the C++ standard and various versions of compilers);
* [CMake](https://cmake.org/) and a compatible build tool for building the project;
* <b>(optional)</b> clang-tidy 14+ for static analysis during build.

## Set up a project {#build-tests}

```bash
git clone https://github.com/VladimirShaleev/ipaddress.git
cd ipaddress
```

Once the repository is clone, you can configure it using CMake.

```bash
cmake -B build -S . \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DIPADDRESS_ENABLE_CLANG_TIDY=OFF \
  -DIPADDRESS_BUILD_TESTS=ON
```

Where:

* `CMAKE_CXX_STANDARD` — The selected standard with which unit tests will be collected. If not specified, the default one in the current build tools will be used.

Once configured, you can build tests.

```bash
cmake --build build --config Debug --target ipaddress-tests -j4
```

Running unit tests.

```bash
cd build
ctest --output-on-failure
```

Below is a list of all parameters supported by the configuration:

* `IPADDRESS_ENABLE_CLANG_TIDY` — Enable clang-tidy checks (`OFF` by default).
* `IPADDRESS_BUILD_TESTS` — Build unit tests (`ON` by default).
* `IPADDRESS_BUILD_BENCHMARK` — Build benchmark (`OFF` by default).
* `IPADDRESS_BUILD_DOC` — Build doc (`ON` by default).
* `IPADDRESS_BUILD_PACKAGES` — Create targets for building packages deb, rpm, etc. (`ON` by default).
* `IPADDRESS_BUILD_MODULE` — Build cpp module (`OFF` by default).
* `IPADDRESS_TEST_MODULE` — Will use the C++ module to build tests if available (`OFF` by default).
* `IPADDRESS_INCLUDE_NATVIS` — Includes a natvis file in the targets for debugging. (`ON` by default).
* `IPADDRESS_NO_EXCEPTIONS` — Disable exceptions throwing (`OFF` by default).
* `IPADDRESS_NO_IPV6_SCOPE` — Disable scope id for ipv6 (`OFF` by default).
* `IPADDRESS_IPV6_SCOPE_MAX_LENGTH` — scope id max length (`16` by default).

## Build a Documentation {#build-doc}

To assemble the documentation, you need to install the following tools on the system:

* [Doxygen](https://www.doxygen.nl) version 1.12.0 for documentation assembly;
* [graphviz](https://graphviz.org/) for creating charts.

After which you can enable the documentation build in the configuration and run build.

```bash
cmake -B build -S . -DIPADDRESS_BUILD_DOC=ON
cmake --build build --target doc
```

## Build a Benchmark {#build-benchmark}

To build the benchmark, follow these steps:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DIPADDRESS_BUILD_BENCHMARK=ON
cmake --build build --config Release --target ipaddress-benchmark
```

@htmlonly

<div class="cards">

<div class="card">
  <a href="benchmark.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Benchmark</h2>
    <p>Back to benchmark.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="namespaceipaddress.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Namespace Reference</h2>
    <p>Review the code documentation.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
