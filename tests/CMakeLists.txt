include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(NOT IPADDRESS_CXX_STANDARD)
  if(CXX_STANDARD)
    set(IPADDRESS_CXX_STANDARD ${CXX_STANDARD})
  else()
    set(IPADDRESS_CXX_STANDARD 11)
  endif()
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
target_compile_features(ipaddress-tests PRIVATE cxx_std_${IPADDRESS_CXX_STANDARD})

include(GoogleTest)
gtest_discover_tests(ipaddress-tests)
