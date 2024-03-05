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
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z
    URL_MD5 5b8e7be1a6454fef772423800c98674d
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    GIT_PROGRESS TRUE   
)
set(BOOST_INCLUDE_LIBRARIES asio)
FetchContent_MakeAvailable(Boost)

if(IPADDRESS_CLANG_TIDY)
  set_target_properties(benchmark PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(benchmark_main PROPERTIES CXX_CLANG_TIDY "")
endif()

add_executable(ipaddress-benchmark benchmark.cpp)
target_link_libraries(ipaddress-benchmark PRIVATE ipaddress Boost::asio benchmark::benchmark benchmark::benchmark_main)
