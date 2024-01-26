#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv4_network, Test) {
    error_code err = error_code::NO_ERROR;
    ipv4_network::parse("192.0.2.0/255.255.255.0", err);   
}
