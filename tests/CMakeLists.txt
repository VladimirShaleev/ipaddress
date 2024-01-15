include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(ipaddress-fixed-string-20-tests "fixed-string-tests.cpp")
target_link_libraries(ipaddress-fixed-string-20-tests ipaddress GTest::gtest GTest::gtest_main)
target_compile_features(ipaddress-fixed-string-20-tests PRIVATE cxx_std_20)

enable_testing()
add_executable(ipaddress-fixed-string-17-tests "fixed-string-tests.cpp")
target_link_libraries(ipaddress-fixed-string-17-tests ipaddress GTest::gtest GTest::gtest_main)
target_compile_features(ipaddress-fixed-string-17-tests PRIVATE cxx_std_17)

enable_testing()
add_executable(ipaddress-fixed-string-14-tests "fixed-string-tests.cpp")
target_link_libraries(ipaddress-fixed-string-14-tests ipaddress GTest::gtest GTest::gtest_main)
target_compile_features(ipaddress-fixed-string-14-tests PRIVATE cxx_std_14)

enable_testing()
add_executable(ipaddress-fixed-string-11-tests "fixed-string-tests.cpp")
target_link_libraries(ipaddress-fixed-string-11-tests ipaddress GTest::gtest GTest::gtest_main)
target_compile_features(ipaddress-fixed-string-11-tests PRIVATE cxx_std_11)

enable_testing()
add_executable(ipaddress-ipv4-address-20-tests "ipv4-address-tests.cpp")
target_link_libraries(ipaddress-ipv4-address-20-tests ipaddress GTest::gtest GTest::gtest_main)
target_compile_features(ipaddress-ipv4-address-20-tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(ipaddress-fixed-string-20-tests)
gtest_discover_tests(ipaddress-fixed-string-17-tests)
gtest_discover_tests(ipaddress-fixed-string-14-tests)
gtest_discover_tests(ipaddress-fixed-string-11-tests)
gtest_discover_tests(ipaddress-ipv4-address-20-tests)
