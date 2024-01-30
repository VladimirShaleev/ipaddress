#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv6_network, DefaultCtor) {
    ipv6_network net;
    
    EXPECT_EQ(net.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net.prefixlen(), 128);
    EXPECT_EQ(net.version, version::V6);
}

TEST(ipv6_network, CopyCtor) {
    auto net = ipv6_network::parse("2001:db8::/96");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

TEST(ipv6_network, CopyOperator) {
    auto net = ipv6_network::parse("2001:db8::/96");
    ipv6_network net_copy;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.prefixlen(), 128);
    EXPECT_EQ(net_copy.version, version::V6);
    net_copy = net;

    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

using NetworkParserIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6Params, parse) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()));
    
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6Params,
    testing::Values(
        std::make_tuple("::1", "::1", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("::1%scope", "::1%scope", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("2001:db8::/32", "2001:db8::", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("2001:db8::%scope/32", "2001:db8::%scope", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("::/0", "::", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 0),
        std::make_tuple("::/128", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128)
    ));

using NetworkParserIpv6NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> not_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6NotStrictParams,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001::", "ffff::", "0:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16),
        std::make_tuple("2001:db8::/24", "2001:d00::", "ffff:ff00::", "0:ff:ffff:ffff:ffff:ffff:ffff:ffff", 24)
    ));
