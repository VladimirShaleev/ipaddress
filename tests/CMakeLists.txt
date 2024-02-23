include(CTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(ipaddress-23-tests "fixed-string-tests.cpp" "uint128-tests.cpp" "ipv4-address-tests.cpp" "ipv6-address-tests.cpp" "ipv4-network-tests.cpp" "ipv6-network-tests.cpp" "ip-address-tests.cpp" "ip-network-tests.cpp")
target_link_libraries(ipaddress-23-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)
target_compile_features(ipaddress-23-tests PRIVATE cxx_std_23)

enable_testing()
add_executable(ipaddress-20-tests "fixed-string-tests.cpp" "uint128-tests.cpp" "ipv4-address-tests.cpp" "ipv6-address-tests.cpp" "ipv4-network-tests.cpp" "ipv6-network-tests.cpp" "ip-address-tests.cpp" "ip-network-tests.cpp")
target_link_libraries(ipaddress-20-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)
target_compile_features(ipaddress-20-tests PRIVATE cxx_std_20)

enable_testing()
add_executable(ipaddress-17-tests "fixed-string-tests.cpp" "uint128-tests.cpp" "ipv4-address-tests.cpp" "ipv6-address-tests.cpp" "ipv4-network-tests.cpp" "ipv6-network-tests.cpp" "ip-address-tests.cpp" "ip-network-tests.cpp")
target_link_libraries(ipaddress-17-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)
target_compile_features(ipaddress-17-tests PRIVATE cxx_std_17)

enable_testing()
add_executable(ipaddress-14-tests "fixed-string-tests.cpp" "uint128-tests.cpp" "ipv4-address-tests.cpp" "ipv6-address-tests.cpp" "ipv4-network-tests.cpp" "ipv6-network-tests.cpp" "ip-address-tests.cpp" "ip-network-tests.cpp")
target_link_libraries(ipaddress-14-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)
target_compile_features(ipaddress-14-tests PRIVATE cxx_std_14)

enable_testing()
add_executable(ipaddress-11-tests "fixed-string-tests.cpp" "uint128-tests.cpp" "ipv4-address-tests.cpp" "ipv6-address-tests.cpp" "ipv4-network-tests.cpp" "ipv6-network-tests.cpp" "ip-address-tests.cpp" "ip-network-tests.cpp")
target_link_libraries(ipaddress-11-tests ipaddress GTest::gtest GTest::gtest_main GTest::gmock_main)
target_compile_features(ipaddress-11-tests PRIVATE cxx_std_11)

include(GoogleTest)
gtest_discover_tests(ipaddress-23-tests)
gtest_discover_tests(ipaddress-20-tests)
gtest_discover_tests(ipaddress-17-tests)
gtest_discover_tests(ipaddress-14-tests)
gtest_discover_tests(ipaddress-11-tests)
