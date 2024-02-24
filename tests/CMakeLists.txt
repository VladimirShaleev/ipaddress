include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(ipaddress-tests 
  "uint128-tests.cpp" 
  "fixed-string-tests.cpp" 
  "ipv4-address-tests.cpp" 
  "ipv6-address-tests.cpp" 
  "ipv4-network-tests.cpp" 
  "ipv6-network-tests.cpp" 
  "ip-address-tests.cpp" 
  "ip-network-tests.cpp")
target_link_libraries(ipaddress-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(ipaddress-tests)
