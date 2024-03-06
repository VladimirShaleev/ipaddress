include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark
  GIT_TAG 344117638c8ff7e239044fd0fa7085839fc03021 # v1.8.3
)
set(BENCHMARK_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
    URL_MD5 893b5203b862eb9bbd08553e24ff146a
    GIT_PROGRESS TRUE
)
set(BOOST_INCLUDE_LIBRARIES asio)
FetchContent_MakeAvailable(Boost)

add_executable(ipaddress-benchmark benchmark.cpp)
target_link_libraries(ipaddress-benchmark PRIVATE ipaddress Boost::asio benchmark::benchmark benchmark::benchmark_main)
