include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

if(IPADDRESS_ENABLE_CLANG_TIDY AND IPADDRESS_CLANG_TIDY)
  set_target_properties(ipaddress-tests PROPERTIES CXX_CLANG_TIDY ${IPADDRESS_CLANG_TIDY})
endif()

include(GoogleTest)
gtest_discover_tests(ipaddress-tests)
