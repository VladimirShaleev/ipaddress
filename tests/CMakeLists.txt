include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(IPADDRESS_CLANG_TIDY)
  set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")
endif()

if(CMAKE_CXX_STANDARD)
  message(STATUS "Select standatd C++${CMAKE_CXX_STANDARD}")
endif()

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
