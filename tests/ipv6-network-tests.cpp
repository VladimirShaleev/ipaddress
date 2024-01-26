#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv6_network, Test) {
    error_code err = error_code::NO_ERROR;
    auto result = ipv6_network::parse("2001:db8::/96", err);   
}
