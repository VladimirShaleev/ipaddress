#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

template <size_t N1, size_t N2>
static constexpr ipv6_network test_swap(const char(&str1)[N1], const char(&str2)[N2]) {
    auto net1 = ipv6_network::parse(str1);
    auto net2 = ipv6_network::parse(str2);
    net1.swap(net2);
    return net1;
}

template <size_t N>
static constexpr error_code test_error(const char(&str)[N]) noexcept {
    error_code err = error_code::NO_ERROR;
    ipv6_network::parse(str, err);
    return err;
}

TEST(ipv6_network, CompileTime) {
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto net1 = ipv6_network::parse<"2001:db8::/96">();
    ASSERT_EQ(net1.network_address(), ipv6_address::parse("2001:db8::"));
    ASSERT_EQ(net1.broadcast_address(), ipv6_address::parse("2001:db8::ffff:ffff"));
    ASSERT_EQ(net1.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    ASSERT_EQ(net1.hostmask(), ipv6_address::parse("::ffff:ffff"));
    ASSERT_EQ(net1.prefixlen(), 96);

    constexpr auto net2 = ipv6_network::parse<"2001:db8::1/128">();
    constexpr auto net2_address = net2.network_address();
    constexpr auto net2_broadcast = net2.broadcast_address();
    constexpr auto net2_netmask = net2.netmask();
    constexpr auto net2_hostmask = net2.hostmask();
    constexpr auto net2_prefixlen = net2.prefixlen();
    ASSERT_EQ(net2_address, ipv6_address::parse("2001:db8::1"));
    ASSERT_EQ(net2_broadcast, ipv6_address::parse("2001:db8::1"));
    ASSERT_EQ(net2_netmask, ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net2_hostmask, ipv6_address::parse("::"));
    ASSERT_EQ(net2_prefixlen, 128);
#endif

    constexpr auto net3 = ipv6_network::parse("2001:db8::%scope/32");
    constexpr auto net3_address = net3.network_address();
    constexpr auto net3_broadcast = net3.broadcast_address();
    constexpr auto net3_netmask = net3.netmask();
    constexpr auto net3_hostmask = net3.hostmask();
    constexpr auto net3_prefixlen = net3.prefixlen();
    constexpr auto net3_hash = net3.hash();
    ASSERT_EQ(net3_address, ipv6_address::parse("2001:db8::%scope"));
    ASSERT_EQ(net3_broadcast, ipv6_address::parse("2001:db8:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net3_netmask, ipv6_address::parse("ffff:ffff::"));
    ASSERT_EQ(net3_hostmask, ipv6_address::parse("::ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net3_prefixlen, 32);
    ASSERT_GT(net3_hash, size_t(0));

    constexpr auto net4 = test_swap("::/128", "2001:db8::%test/64");
    constexpr auto net4_address = net4.network_address();
    constexpr auto net4_broadcast = net4.broadcast_address();
    constexpr auto net4_netmask = net4.netmask();
    constexpr auto net4_hostmask = net4.hostmask();
    constexpr auto net4_prefixlen = net4.prefixlen();
    constexpr auto net4_hash = net4.hash();
    ASSERT_EQ(net4_address, ipv6_address::parse("2001:db8::%test"));
    ASSERT_EQ(net4_broadcast, ipv6_address::parse("2001:db8::ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net4_netmask, ipv6_address::parse("ffff:ffff:ffff:ffff::"));
    ASSERT_EQ(net4_hostmask, ipv6_address::parse("::ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net4_prefixlen, 64);
    ASSERT_GT(net4_hash, size_t(0));

    constexpr auto net5_error = test_error("2001:db8::/8");
    ASSERT_EQ(net5_error, error_code::HAS_HOST_BITS_SET);
    
    constexpr auto b1 = net3 < net4;
    constexpr auto b2 = net3 > net4;
    constexpr auto b3 = net3 <= net4;
    constexpr auto b4 = net3 >= net4;
    constexpr auto b5 = net3 == net4;
    constexpr auto b6 = net3 != net4;
    ASSERT_TRUE(b1);
    ASSERT_FALSE(b2);
    ASSERT_TRUE(b3);
    ASSERT_FALSE(b4);
    ASSERT_FALSE(b5);
    ASSERT_TRUE(b6);

    constexpr auto net6 = "2001:db8::/32"_ipv6_net;
    constexpr auto net7 = "2001:db8::"_ipv6_net;
    ASSERT_EQ(net6, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net7, ipv6_network::parse("2001:db8::/128"));

    constexpr auto net8 = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 32);
    constexpr auto net9 = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 32, false);
    ASSERT_EQ(net8, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net9, ipv6_network::parse("2001:db8::/32"));

    constexpr auto net10 = ipv6_network::parse("ff00::").is_multicast();
    constexpr auto net11 = ipv6_network::parse("fdff::").is_multicast();
    ASSERT_TRUE(net10);
    ASSERT_FALSE(net11);

    constexpr auto net12 = ipv6_network::parse("2001::1/128").is_private();
    constexpr auto net13 = ipv6_network::parse("::ff/128").is_private();
    ASSERT_TRUE(net12);
    ASSERT_FALSE(net13);
    
    constexpr auto net14 = ipv6_network::parse("200::1/128").is_global();
    constexpr auto net15 = ipv6_network::parse("2001::1/128").is_global();
    ASSERT_TRUE(net14);
    ASSERT_FALSE(net15);

    constexpr auto net16 = ipv6_network::parse("4000::1/128").is_reserved();
    constexpr auto net17 = ipv6_network::parse("febf:ffff::").is_reserved();
    ASSERT_TRUE(net16);
    ASSERT_FALSE(net17);
    
    constexpr auto net18 = ipv6_network::parse("::1").is_loopback();
    constexpr auto net19 = ipv6_network::parse("::2").is_loopback();
    ASSERT_TRUE(net18);
    ASSERT_FALSE(net19);

    constexpr auto net20 = ipv6_network::parse("febf:ffff::").is_link_local();
    constexpr auto net21 = ipv6_network::parse("fe7f:ffff::").is_link_local();
    ASSERT_TRUE(net20);
    ASSERT_FALSE(net21);

    constexpr auto net22 = ipv6_network::parse("feff:ffff:ffff:ffff::").is_site_local();
    constexpr auto net23 = ipv6_network::parse("ff00::").is_site_local();
    ASSERT_TRUE(net22);
    ASSERT_FALSE(net23);

    constexpr auto net24 = ipv6_network::parse("::").is_unspecified();
    constexpr auto net25 = ipv6_network::parse("::/127").is_unspecified();
    ASSERT_TRUE(net24);
    ASSERT_FALSE(net25);
}

TEST(ipv6_network, DefaultCtor) {
    ipv6_network net;
    
    EXPECT_EQ(net.network_address(), ipv6_address::parse("::"));
    EXPECT_EQ(net.broadcast_address(), ipv6_address::parse("::"));
    EXPECT_EQ(net.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net.prefixlen(), 128);
    EXPECT_EQ(net.version(), ip_version::V6);
}

TEST(ipv6_network, CopyCtor) {
    auto net = ipv6_network::parse("2001:db8::/96");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.network_address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.broadcast_address(), ipv6_address::parse("2001:db8::ffff:ffff"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

TEST(ipv6_network, CopyOperator) {
    auto net = ipv6_network::parse("2001:db8::/96");
    ipv6_network net_copy;
    
    EXPECT_EQ(net_copy.network_address(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net_copy.broadcast_address(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.prefixlen(), 128);
    EXPECT_EQ(net_copy.version(), ip_version::V6);
    net_copy = net;

    EXPECT_EQ(net_copy.network_address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.broadcast_address(), ipv6_address::parse("2001:db8::ffff:ffff"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

using NetworkParserIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6Params, parse) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()));
    
    EXPECT_EQ(net.network_address(), excepted_address);
    EXPECT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    EXPECT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6Params,
    testing::Values(
        std::make_tuple("::1", "::1", "::1", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("::1%scope", "::1%scope", "::1", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("2001:db8::/32", "2001:db8::", "2001:db8:ffff:ffff:ffff:ffff:ffff:ffff", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("2001:db8::%scope/32", "2001:db8::%scope", "2001:db8:ffff:ffff:ffff:ffff:ffff:ffff", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("::/0", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 0),
        std::make_tuple("::/128", "::", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128)
    ));

using NetworkParserIpv6NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.network_address(), excepted_address);
    EXPECT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> not_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    EXPECT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6NotStrictParams,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001::", "2001:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16),
        std::make_tuple("2001:db8::/24", "2001:d00::", "2001:dff:ffff:ffff:ffff:ffff:ffff:ffff", "ffff:ff00::", "0:ff:ffff:ffff:ffff:ffff:ffff:ffff", 24)
    ));

using NetworkFromAddressIpv6Params = TestWithParam<std::tuple<const char*, const char*, size_t, bool>>;
TEST_P(NetworkFromAddressIpv6Params, from_address) {
    auto address = ipv6_address::parse(get<1>(GetParam()));
    auto prefixlen = get<2>(GetParam());
    auto strict = get<3>(GetParam());

    auto expected = ipv6_network::parse(get<0>(GetParam()), strict);

    error_code err = error_code::NO_ERROR;
    auto actual = ipv6_network::from_address(address, err, prefixlen, strict);
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(actual.network_address(), expected.network_address());
    ASSERT_EQ(actual.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual.netmask(), expected.netmask());
    ASSERT_EQ(actual.hostmask(), expected.hostmask());
    ASSERT_EQ(actual.prefixlen(), expected.prefixlen());

    auto actual2 = ipv6_network::from_address(address, prefixlen, strict);
    ASSERT_EQ(actual2, expected);
    ASSERT_EQ(actual2.network_address(), expected.network_address());
    ASSERT_EQ(actual2.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual2.netmask(), expected.netmask());
    ASSERT_EQ(actual2.hostmask(), expected.hostmask());
    ASSERT_EQ(actual2.prefixlen(), expected.prefixlen());
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkFromAddressIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001:db8::", 16, false),
        std::make_tuple("2001:db8::/32", "2001:db8::", 32, true)
    ));

TEST(ipv6_network, from_address_error) {
    error_code err = error_code::NO_ERROR;
    auto actual = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), err, 16);
    ASSERT_EQ(err, error_code::HAS_HOST_BITS_SET);
    ASSERT_EQ(actual.network_address(), ipv6_address::parse("::"));
    ASSERT_EQ(actual.broadcast_address(), ipv6_address::parse("::"));
    ASSERT_EQ(actual.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(actual.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(actual.prefixlen(), 128);
    
#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 16);
    ASSERT_EQ(error_network.network_address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        []() { ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 16); },
        ThrowsMessage<parse_error>(StrEq("has host bits set in address 2001:db8::")));
#endif
}

using InvalidNetworkIpv6Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv6Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv6_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv6_network::parse(expected_network);

    EXPECT_EQ(error_network.network_address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, InvalidNetworkIpv6Params,
    testing::Values(
        std::make_tuple("/", error_code::EMPTY_NETMASK, "empty mask in address /"),
        std::make_tuple("2001:db8::/", error_code::EMPTY_NETMASK, "empty mask in address 2001:db8::/"),
        std::make_tuple("2001:db8::/129", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/129"),
        std::make_tuple("2001:db8::/255.255.255.255", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/255.255.255.255"),
        std::make_tuple("/%scope", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope"),
        std::make_tuple("/%scope8", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope8"),
        std::make_tuple("::1/::1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::1"),
        std::make_tuple("::1/1::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/1::"),
        std::make_tuple("::1/-1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/-1"),
        std::make_tuple("::1/+1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/+1"),
        std::make_tuple("::1/ 1 ", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/ 1 "),
        std::make_tuple("::1/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/word"),
        std::make_tuple("::1/::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::"),
        std::make_tuple("::1%scope/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1%scope/word"),
        
        std::make_tuple("2001:db8::/24", error_code::HAS_HOST_BITS_SET, "has host bits set in address 2001:db8::/24"),
        std::make_tuple("2001:db8:://", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://"),
        std::make_tuple("2001:db8:://128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://128"),
        std::make_tuple("2001:db8::/128/128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8::/128/128"),

        std::make_tuple("/8", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("google.com", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com has invalid symbol"),
        std::make_tuple("1.2.3.4", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4"),
        std::make_tuple("10/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10/8"),
        std::make_tuple("1234:axy::b", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("google.com%scope", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com%scope has invalid symbol"),
        std::make_tuple("1.2.3.4%scope", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4%scope"),
        std::make_tuple("10%scope/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10%scope/8"),
        std::make_tuple("1234:axy::b%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b%scope has invalid symbols")
    ));

TEST(ipv6_network, Comparison) {
    auto net1 = ipv6_network::parse("2001:db8::/96");
    auto net2 = ipv6_network::parse("2001:dc8::");
    auto net3 = ipv6_network::parse("2001:dc8::/128");
    
    ASSERT_TRUE(net1 < net2);
    ASSERT_TRUE(net1 <= net2);
    ASSERT_FALSE(net1 > net2);
    ASSERT_FALSE(net1 >= net2);
    ASSERT_FALSE(net1 == net2);
    ASSERT_TRUE(net1 != net2);
    
    ASSERT_FALSE(net2 < net1);
    ASSERT_FALSE(net2 <= net1);
    ASSERT_TRUE(net2 > net1);
    ASSERT_TRUE(net2 >= net1);
    ASSERT_FALSE(net2 == net1);
    ASSERT_TRUE(net2 != net1);
    
    ASSERT_FALSE(net2 < net3);
    ASSERT_TRUE(net2 <= net3);
    ASSERT_FALSE(net2 > net3);
    ASSERT_TRUE(net2 >= net3);
    ASSERT_TRUE(net2 == net3);
    ASSERT_FALSE(net2 != net3);
    
    ASSERT_FALSE(net3 < net2);
    ASSERT_TRUE(net3 <= net2);
    ASSERT_FALSE(net3 > net2);
    ASSERT_TRUE(net3 >= net2);
    ASSERT_TRUE(net3 == net2);
    ASSERT_FALSE(net3 != net2);
}

using ToStringNetworkIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*>>;
TEST_P(ToStringNetworkIpv6Params, to_string) {
    const auto expected_full = std::get<1>(GetParam());
    const auto expected_compact = std::get<2>(GetParam());
    const auto expected_compressed = std::get<3>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam()));

    std::ostringstream ss_full; ss_full << full << actual;
    std::ostringstream ss_default; ss_default << actual;
    std::ostringstream ss_compact; ss_compact << compact << actual;
    std::ostringstream ss_compressed; ss_compressed << compressed << actual;
    
    ASSERT_EQ(actual.to_string(format::full), std::string(expected_full));
    ASSERT_EQ(actual.to_string(format::compact), std::string(expected_compact));
    ASSERT_EQ(actual.to_string(format::compressed), std::string(expected_compressed));
    ASSERT_EQ(actual.to_string(), std::string(expected_compressed));
    ASSERT_EQ((std::string) actual, std::string(expected_compressed));
    ASSERT_EQ(std::to_string(actual), std::string(expected_compressed));
    ASSERT_EQ(ss_full.str(), std::string(expected_full));
    ASSERT_EQ(ss_default.str(), std::string(expected_compressed));
    ASSERT_EQ(ss_compact.str(), std::string(expected_compact));
    ASSERT_EQ(ss_compressed.str(), std::string(expected_compressed));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, ToStringNetworkIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8::", "2001:0db8:0000:0000:0000:0000:0000:0000/128", "2001:db8:0:0:0:0:0:0/128", "2001:db8::/128"),
        std::make_tuple("2001:db8::/32", "2001:0db8:0000:0000:0000:0000:0000:0000/32", "2001:db8:0:0:0:0:0:0/32", "2001:db8::/32"),
        std::make_tuple("2001:db8::%scope/32", "2001:0db8:0000:0000:0000:0000:0000:0000%scope/32", "2001:db8:0:0:0:0:0:0%scope/32", "2001:db8::%scope/32")
    ));

TEST(ipv6_network, Hash) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/128");
    auto net3 = ipv6_network::parse("2001:db8::/32");
    auto net4 = ipv6_network::parse("2001:db8::%scope/32");
    auto hash_functor = std::hash<ipv6_network>{};

    ASSERT_EQ(net1.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net2.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net3.hash(), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(net4.hash(), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);

    ASSERT_EQ(hash_functor(net1), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net2), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net3), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(hash_functor(net4), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);
}

TEST(ipv6_network, Containers) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/64");
    auto net3 = ipv6_network::parse("2001:db8::/32");

    std::vector<ipv6_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ipv6_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net3);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net1);
    
    auto net3_with_scope = ipv6_network::parse("2001:db8::%scope/32");
    std::unordered_map<ipv6_network, int> unordered_map;
    unordered_map[net2] = 2;
    unordered_map[net1] = 1;
    unordered_map[net3] = 3;
    unordered_map[net3] = 4;
    unordered_map[net3_with_scope] = 0;
    ASSERT_EQ(unordered_map.size(), 4);
    ASSERT_EQ(unordered_map[net1], 1);
    ASSERT_EQ(unordered_map[net2], 2);
    ASSERT_EQ(unordered_map[net3], 4);
    ASSERT_EQ(unordered_map[net3_with_scope], 0);
}

TEST(ipv6_network, Swap) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::%scope/32");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv6_network::parse("2001:db8::%scope/32"));
    ASSERT_EQ(net2, ipv6_network::parse("2001:db8::"));
}

TEST(ipv6_network, literals) {
    auto net1 = "2001:db8::/32"_ipv6_net;
    auto net2 = "0001:0002:0003:0004:0005:0006:0007:0008%123456789abcdefg/128"_ipv6_net;
    
    ASSERT_EQ(net1, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net2, ipv6_network::parse("1:2:3:4:5:6:7:8%123456789abcdefg"));
}

using IsMulticastIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsMulticastIpv6NetworkParams, is_multicast) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_multicast();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsMulticastIpv6NetworkParams,
    Values(
        std::make_tuple("ffff::", true),
        std::make_tuple("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", true),
        std::make_tuple("ff00::", true),
        std::make_tuple("fdff::", false)
    ));

using IsPrivateIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsPrivateIpv6NetworkParams, is_private) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_private();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsPrivateIpv6NetworkParams,
    Values(
        std::make_tuple("fbff:ffff::", false),
        std::make_tuple("fe00::%123", false),
        std::make_tuple("::/0", false),
        std::make_tuple("::ff/128", false),
        std::make_tuple("2001::1/128", true),
        std::make_tuple("fc00::", true),
        std::make_tuple("fc00:ffff:ffff:ffff::", true),
        std::make_tuple("::/128", true),
        std::make_tuple("::1/128", true),
        std::make_tuple("::ffff:0:0/96", true),
        std::make_tuple("100::/64", true),
        std::make_tuple("2001::/23", true),
        std::make_tuple("2001:10::/28", true),
        std::make_tuple("2001:2::/48", true),
        std::make_tuple("2001:db8::/32", true),
        std::make_tuple("fc00::%test/7", true),
        std::make_tuple("fe80::/10", true)
    ));

using IsGlobalIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsGlobalIpv6NetworkParams, is_global) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_global();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsGlobalIpv6NetworkParams,
    Values(
        std::make_tuple("200::1/128", true)
    ));

using IsReservedIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsReservedIpv6NetworkParams, is_reserved) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_reserved();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsReservedIpv6NetworkParams,
    Values(
        std::make_tuple("4000::1/128", true)
    ));

using IsLoopbackIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLoopbackIpv6NetworkParams, is_loopback) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_loopback();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsLoopbackIpv6NetworkParams,
    Values(
        std::make_tuple("0:0::0:01", true),
        std::make_tuple("::1", true),
        std::make_tuple("::2", false)
    ));

using IsLinkLocalIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLinkLocalIpv6NetworkParams, is_link_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_link_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsLinkLocalIpv6NetworkParams,
    Values(
        std::make_tuple("fea0::", true),
        std::make_tuple("febf:ffff::", true),
        std::make_tuple("fe7f:ffff::", false),
        std::make_tuple("fec0::", false)
    ));

using IsSiteLocalIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsSiteLocalIpv6NetworkParams, is_site_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_site_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsSiteLocalIpv6NetworkParams,
    Values(
        std::make_tuple("fecf::", true),
        std::make_tuple("feff:ffff:ffff:ffff::", true),
        std::make_tuple("fbf:ffff::", false),
        std::make_tuple("ff00::", false)
    ));

using IsUnspecifiedIpv6NetworkParams = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsUnspecifiedIpv6NetworkParams, is_unspecified) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).is_unspecified();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, IsUnspecifiedIpv6NetworkParams,
    Values(
        std::make_tuple("0::0", true),
        std::make_tuple("::1", false),
        std::make_tuple("::/127", false)
    ));

using ContainsIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(ContainsIpv6NetworkParams, contains) {
    const auto expected = std::get<2>(GetParam());

    const auto network = ipv6_network::parse(std::get<0>(GetParam()));
    const auto address = ipv6_address::parse(std::get<1>(GetParam()));

    const auto actual = network.contains(address);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, ContainsIpv6NetworkParams,
    Values(
        std::make_tuple("2001:db8::/32", "2001:db8::1", true),
        std::make_tuple("2001:db8::/32", "2001:dbc::", false)
    ));

using OverlapsIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(OverlapsIpv6NetworkParams, overlaps) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv6_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv6_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.overlaps(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, OverlapsIpv6NetworkParams,
    Values(
        std::make_tuple("2001:db8::/32", "2001:db8::/128", true),
        std::make_tuple("2001:dbc::/32", "2001:db8::/32", false),
        std::make_tuple("2001:db8::/32", "2001:db8::/32", true)
    ));

using SubnetOfIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(SubnetOfIpv6NetworkParams, subnet_of) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv6_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv6_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.subnet_of(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SubnetOfIpv6NetworkParams,
    Values(
        std::make_tuple("2000:999::/56", "2000:aaa::/48", false),
        std::make_tuple("2000:aaa::/56", "2000:aaa::/48", true),
        std::make_tuple("2000:bbb::/56", "2000:aaa::/48", false),
        std::make_tuple("2000:aaa::/48", "2000:aaa::/56", false),
        std::make_tuple("2000:999::%scope/56", "2000:aaa::%scope/48", false),
        std::make_tuple("2000:aaa::%scope/56", "2000:aaa::%scope/48", true)
    ));

using SupernetOfIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, bool>>;
TEST_P(SupernetOfIpv6NetworkParams, supernet_of) {
    const auto expected = std::get<2>(GetParam());

    const auto net1 = ipv6_network::parse(std::get<0>(GetParam()));
    const auto net2 = ipv6_network::parse(std::get<1>(GetParam()));

    const auto actual = net1.supernet_of(net2);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SupernetOfIpv6NetworkParams,
    Values(
        std::make_tuple("2000:999::/56", "2000:aaa::/48", false),
        std::make_tuple("2000:aaa::/56", "2000:aaa::/48", false),
        std::make_tuple("2000:bbb::/56", "2000:aaa::/48", false),
        std::make_tuple("2000:aaa::/48", "2000:aaa::/56", true)
    ));

using AddressesCountIpv6NetworkParams = TestWithParam<std::tuple<const char*, uint128_t>>;
TEST_P(AddressesCountIpv6NetworkParams, addresses_count) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).addresses_count();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, AddressesCountIpv6NetworkParams,
    Values(
        std::make_tuple("2001:658:22a:cafe::/64", uint128_t::from_string("18446744073709551616").value()),
        std::make_tuple("2001:658:22a:cafe::%scope/64", uint128_t::from_string("18446744073709551616").value())
    ));

using HostsIpv6NetworkParams = TestWithParam<std::tuple<const char*, std::vector<const char*>>>;
TEST_P(HostsIpv6NetworkParams, hosts) {
    std::vector<ipv6_address> expected;
    for (const auto& addr : std::get<1>(GetParam())) {
        expected.push_back(ipv6_address::parse(addr));
    }
    const auto actual = ipv6_network::parse(std::get<0>(GetParam())).hosts();

    ASSERT_FALSE(actual.empty());
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.front(), expected.front());
    ASSERT_EQ(actual.back(), expected.back());

    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    auto expected_reverse_it = expected.rbegin();
    for (auto it = actual.rbegin(); it != actual.rend(); ++it) {
        ASSERT_EQ(*it, *expected_reverse_it++);
    }

    auto expected_const_reverse_it = expected.crbegin();
    auto actual_const_reverse_it = actual.crbegin();
    for (; actual_const_reverse_it != actual.crend(); ++actual_const_reverse_it) {
        ASSERT_EQ(*actual_const_reverse_it, *expected_const_reverse_it++);
    }

    expected_const_it = expected.cbegin();
    for (auto it = actual_const_reverse_it.base(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(actual[i], expected[i]);
        ASSERT_EQ(actual.at(i), expected[i]);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, HostsIpv6NetworkParams,
    Values(
        std::make_tuple("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128", std::vector<const char*>{"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"}),
        std::make_tuple("2001:658:22a:cafe::1/128", std::vector<const char*>{"2001:658:22a:cafe::1"}),
        std::make_tuple("2001:658:22a:cafe::/127", std::vector<const char*>{"2001:658:22a:cafe::", "2001:658:22a:cafe::1"}),
        std::make_tuple("2001:658:22a:cafe::/120", std::vector<const char*>{
            "2001:658:22a:cafe::1", "2001:658:22a:cafe::2", "2001:658:22a:cafe::3", "2001:658:22a:cafe::4", "2001:658:22a:cafe::5", "2001:658:22a:cafe::6", "2001:658:22a:cafe::7", "2001:658:22a:cafe::8", "2001:658:22a:cafe::9", "2001:658:22a:cafe::a", "2001:658:22a:cafe::b", "2001:658:22a:cafe::c", "2001:658:22a:cafe::d", "2001:658:22a:cafe::e", "2001:658:22a:cafe::f", 
            "2001:658:22a:cafe::10", "2001:658:22a:cafe::11", "2001:658:22a:cafe::12", "2001:658:22a:cafe::13", "2001:658:22a:cafe::14", "2001:658:22a:cafe::15", "2001:658:22a:cafe::16", "2001:658:22a:cafe::17", "2001:658:22a:cafe::18", "2001:658:22a:cafe::19", "2001:658:22a:cafe::1a", "2001:658:22a:cafe::1b", "2001:658:22a:cafe::1c", "2001:658:22a:cafe::1d", "2001:658:22a:cafe::1e", "2001:658:22a:cafe::1f", 
            "2001:658:22a:cafe::20", "2001:658:22a:cafe::21", "2001:658:22a:cafe::22", "2001:658:22a:cafe::23", "2001:658:22a:cafe::24", "2001:658:22a:cafe::25", "2001:658:22a:cafe::26", "2001:658:22a:cafe::27", "2001:658:22a:cafe::28", "2001:658:22a:cafe::29", "2001:658:22a:cafe::2a", "2001:658:22a:cafe::2b", "2001:658:22a:cafe::2c", "2001:658:22a:cafe::2d", "2001:658:22a:cafe::2e", "2001:658:22a:cafe::2f", 
            "2001:658:22a:cafe::30", "2001:658:22a:cafe::31", "2001:658:22a:cafe::32", "2001:658:22a:cafe::33", "2001:658:22a:cafe::34", "2001:658:22a:cafe::35", "2001:658:22a:cafe::36", "2001:658:22a:cafe::37", "2001:658:22a:cafe::38", "2001:658:22a:cafe::39", "2001:658:22a:cafe::3a", "2001:658:22a:cafe::3b", "2001:658:22a:cafe::3c", "2001:658:22a:cafe::3d", "2001:658:22a:cafe::3e", "2001:658:22a:cafe::3f", 
            "2001:658:22a:cafe::40", "2001:658:22a:cafe::41", "2001:658:22a:cafe::42", "2001:658:22a:cafe::43", "2001:658:22a:cafe::44", "2001:658:22a:cafe::45", "2001:658:22a:cafe::46", "2001:658:22a:cafe::47", "2001:658:22a:cafe::48", "2001:658:22a:cafe::49", "2001:658:22a:cafe::4a", "2001:658:22a:cafe::4b", "2001:658:22a:cafe::4c", "2001:658:22a:cafe::4d", "2001:658:22a:cafe::4e", "2001:658:22a:cafe::4f", 
            "2001:658:22a:cafe::50", "2001:658:22a:cafe::51", "2001:658:22a:cafe::52", "2001:658:22a:cafe::53", "2001:658:22a:cafe::54", "2001:658:22a:cafe::55", "2001:658:22a:cafe::56", "2001:658:22a:cafe::57", "2001:658:22a:cafe::58", "2001:658:22a:cafe::59", "2001:658:22a:cafe::5a", "2001:658:22a:cafe::5b", "2001:658:22a:cafe::5c", "2001:658:22a:cafe::5d", "2001:658:22a:cafe::5e", "2001:658:22a:cafe::5f", 
            "2001:658:22a:cafe::60", "2001:658:22a:cafe::61", "2001:658:22a:cafe::62", "2001:658:22a:cafe::63", "2001:658:22a:cafe::64", "2001:658:22a:cafe::65", "2001:658:22a:cafe::66", "2001:658:22a:cafe::67", "2001:658:22a:cafe::68", "2001:658:22a:cafe::69", "2001:658:22a:cafe::6a", "2001:658:22a:cafe::6b", "2001:658:22a:cafe::6c", "2001:658:22a:cafe::6d", "2001:658:22a:cafe::6e", "2001:658:22a:cafe::6f", 
            "2001:658:22a:cafe::70", "2001:658:22a:cafe::71", "2001:658:22a:cafe::72", "2001:658:22a:cafe::73", "2001:658:22a:cafe::74", "2001:658:22a:cafe::75", "2001:658:22a:cafe::76", "2001:658:22a:cafe::77", "2001:658:22a:cafe::78", "2001:658:22a:cafe::79", "2001:658:22a:cafe::7a", "2001:658:22a:cafe::7b", "2001:658:22a:cafe::7c", "2001:658:22a:cafe::7d", "2001:658:22a:cafe::7e", "2001:658:22a:cafe::7f", 
            "2001:658:22a:cafe::80", "2001:658:22a:cafe::81", "2001:658:22a:cafe::82", "2001:658:22a:cafe::83", "2001:658:22a:cafe::84", "2001:658:22a:cafe::85", "2001:658:22a:cafe::86", "2001:658:22a:cafe::87", "2001:658:22a:cafe::88", "2001:658:22a:cafe::89", "2001:658:22a:cafe::8a", "2001:658:22a:cafe::8b", "2001:658:22a:cafe::8c", "2001:658:22a:cafe::8d", "2001:658:22a:cafe::8e", "2001:658:22a:cafe::8f", 
            "2001:658:22a:cafe::90", "2001:658:22a:cafe::91", "2001:658:22a:cafe::92", "2001:658:22a:cafe::93", "2001:658:22a:cafe::94", "2001:658:22a:cafe::95", "2001:658:22a:cafe::96", "2001:658:22a:cafe::97", "2001:658:22a:cafe::98", "2001:658:22a:cafe::99", "2001:658:22a:cafe::9a", "2001:658:22a:cafe::9b", "2001:658:22a:cafe::9c", "2001:658:22a:cafe::9d", "2001:658:22a:cafe::9e", "2001:658:22a:cafe::9f", 
            "2001:658:22a:cafe::a0", "2001:658:22a:cafe::a1", "2001:658:22a:cafe::a2", "2001:658:22a:cafe::a3", "2001:658:22a:cafe::a4", "2001:658:22a:cafe::a5", "2001:658:22a:cafe::a6", "2001:658:22a:cafe::a7", "2001:658:22a:cafe::a8", "2001:658:22a:cafe::a9", "2001:658:22a:cafe::aa", "2001:658:22a:cafe::ab", "2001:658:22a:cafe::ac", "2001:658:22a:cafe::ad", "2001:658:22a:cafe::ae", "2001:658:22a:cafe::af", 
            "2001:658:22a:cafe::b0", "2001:658:22a:cafe::b1", "2001:658:22a:cafe::b2", "2001:658:22a:cafe::b3", "2001:658:22a:cafe::b4", "2001:658:22a:cafe::b5", "2001:658:22a:cafe::b6", "2001:658:22a:cafe::b7", "2001:658:22a:cafe::b8", "2001:658:22a:cafe::b9", "2001:658:22a:cafe::ba", "2001:658:22a:cafe::bb", "2001:658:22a:cafe::bc", "2001:658:22a:cafe::bd", "2001:658:22a:cafe::be", "2001:658:22a:cafe::bf", 
            "2001:658:22a:cafe::c0", "2001:658:22a:cafe::c1", "2001:658:22a:cafe::c2", "2001:658:22a:cafe::c3", "2001:658:22a:cafe::c4", "2001:658:22a:cafe::c5", "2001:658:22a:cafe::c6", "2001:658:22a:cafe::c7", "2001:658:22a:cafe::c8", "2001:658:22a:cafe::c9", "2001:658:22a:cafe::ca", "2001:658:22a:cafe::cb", "2001:658:22a:cafe::cc", "2001:658:22a:cafe::cd", "2001:658:22a:cafe::ce", "2001:658:22a:cafe::cf", 
            "2001:658:22a:cafe::d0", "2001:658:22a:cafe::d1", "2001:658:22a:cafe::d2", "2001:658:22a:cafe::d3", "2001:658:22a:cafe::d4", "2001:658:22a:cafe::d5", "2001:658:22a:cafe::d6", "2001:658:22a:cafe::d7", "2001:658:22a:cafe::d8", "2001:658:22a:cafe::d9", "2001:658:22a:cafe::da", "2001:658:22a:cafe::db", "2001:658:22a:cafe::dc", "2001:658:22a:cafe::dd", "2001:658:22a:cafe::de", "2001:658:22a:cafe::df", 
            "2001:658:22a:cafe::e0", "2001:658:22a:cafe::e1", "2001:658:22a:cafe::e2", "2001:658:22a:cafe::e3", "2001:658:22a:cafe::e4", "2001:658:22a:cafe::e5", "2001:658:22a:cafe::e6", "2001:658:22a:cafe::e7", "2001:658:22a:cafe::e8", "2001:658:22a:cafe::e9", "2001:658:22a:cafe::ea", "2001:658:22a:cafe::eb", "2001:658:22a:cafe::ec", "2001:658:22a:cafe::ed", "2001:658:22a:cafe::ee", "2001:658:22a:cafe::ef", 
            "2001:658:22a:cafe::f0", "2001:658:22a:cafe::f1", "2001:658:22a:cafe::f2", "2001:658:22a:cafe::f3", "2001:658:22a:cafe::f4", "2001:658:22a:cafe::f5", "2001:658:22a:cafe::f6", "2001:658:22a:cafe::f7", "2001:658:22a:cafe::f8", "2001:658:22a:cafe::f9", "2001:658:22a:cafe::fa", "2001:658:22a:cafe::fb", "2001:658:22a:cafe::fc", "2001:658:22a:cafe::fd", "2001:658:22a:cafe::fe", "2001:658:22a:cafe::ff"
        })
    ));

using SupernetIpv6NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, const char*>>;
TEST_P(SupernetIpv6NetworkParams, supernet) {
    const auto expected = ipv6_network::parse(std::get<3>(GetParam()));

    const auto network = ipv6_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    const auto actual = network.supernet(prefixlen_diff, new_prefix);

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SupernetIpv6NetworkParams,
    Values(
        std::make_tuple("2001:658:22a:cafe::/127", 1, nullptr, "2001:658:22a:cafe::/126"),
        std::make_tuple("2001:658:22a:cafe::/127", 2, nullptr, "2001:658:22a:cafe::/125"),
        std::make_tuple("2001:658:22a:cafe::/127", 1, 64, "2001:658:22a:cafe::/64"),
        std::make_tuple("2001:658:22a:cafe::/64", 1, nullptr, "2001:658:22a:cafe::/63"),
        std::make_tuple("2001:658:22a:cafe::/63", 1, nullptr, "2001:658:22a:cafc::/62"),
        std::make_tuple("::/0", 1, nullptr, "::/0")
    ));

using SupernetErrorIpv6NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, error_code, const char*>>;
TEST_P(SupernetErrorIpv6NetworkParams, supernet) {
    const auto expected_error = std::get<3>(GetParam());
    const auto expected_error_str = std::get<4>(GetParam());

    const auto network = ipv6_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    error_code err;
    auto actural = network.supernet(err, prefixlen_diff, new_prefix);
    ASSERT_EQ(err, expected_error);
    ASSERT_EQ(actural.network_address(), ipv6_address::parse("::"));
    ASSERT_EQ(actural.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(actural.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(actural.prefixlen(), 128);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = network.supernet(prefixlen_diff, new_prefix);
    ASSERT_EQ(error_network.network_address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        ([&network, prefixlen_diff, new_prefix]() { network.supernet(prefixlen_diff, new_prefix); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SupernetErrorIpv6NetworkParams,
    Values(
        std::make_tuple("2001:658:22a:cafe::/64", 1, 65, error_code::NEW_PREFIX_MUST_BE_SHORTER, "new prefix must be shorter"),
        std::make_tuple("2001:658:22a:cafe::/64", 2, 63, error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX, "cannot set prefixlen_diff and new_prefix"),
        std::make_tuple("2001:658:22a:cafe::/64", 65, nullptr, error_code::INVALID_PREFIXLEN_DIFF, "invalid prefixlen_diff")
    ));

using SubnetsIpv6NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, std::vector<const char*>>>;
TEST_P(SubnetsIpv6NetworkParams, subnets) {
    std::vector<ipv6_network> expected;
    for (const auto& addr : std::get<3>(GetParam())) {
        expected.push_back(ipv6_network::parse(addr));
    }

    const auto network = ipv6_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    const auto actual = network.subnets(prefixlen_diff, new_prefix);

    ASSERT_FALSE(actual.empty());
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.front(), expected.front());
    ASSERT_EQ(actual.back(), expected.back());
    
    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    auto expected_reverse_it = expected.rbegin();
    for (auto it = actual.rbegin(); it != actual.rend(); ++it) {
        ASSERT_EQ(*it, *expected_reverse_it++);
    }

    auto expected_const_reverse_it = expected.crbegin();
    auto actual_const_reverse_it = actual.crbegin();
    for (; actual_const_reverse_it != actual.crend(); ++actual_const_reverse_it) {
        ASSERT_EQ(*actual_const_reverse_it, *expected_const_reverse_it++);
    }

    expected_const_it = expected.cbegin();
    for (auto it = actual_const_reverse_it.base(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(actual[i], expected[i]);
        ASSERT_EQ(actual.at(i), expected[i]);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SubnetsIpv6NetworkParams,
    Values(
        std::make_tuple("::/0", 1, nullptr, std::vector<const char*>{"::/1", "8000::/1"}),
        std::make_tuple("2001:658:22a:cafe::/120", 1, nullptr, std::vector<const char*>{"2001:658:22a:cafe::/121", "2001:658:22a:cafe::80/121" }),
        std::make_tuple("2001:658:22a:cafe::/120", 2, nullptr, std::vector<const char*>{"2001:658:22a:cafe::/122", "2001:658:22a:cafe::40/122", "2001:658:22a:cafe::80/122", "2001:658:22a:cafe::c0/122" }),
        std::make_tuple("2001:658:22a:cafe::/120", 1, 123, std::vector<const char*>{"2001:658:22a:cafe::/123", "2001:658:22a:cafe::20/123", "2001:658:22a:cafe::40/123", "2001:658:22a:cafe::60/123", "2001:658:22a:cafe::80/123", "2001:658:22a:cafe::a0/123", "2001:658:22a:cafe::c0/123", "2001:658:22a:cafe::e0/123" }),
        std::make_tuple("2001:db8::1/128", 1, nullptr, std::vector<const char*>{"2001:db8::1/128"}),
        std::make_tuple("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128", 1, nullptr, std::vector<const char*>{"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128"})
    ));
/*
using SubnetsErrorIpv6NetworkParams = TestWithParam<std::tuple<const char*, size_t, optional<size_t>, error_code, const char*>>;
TEST_P(SubnetsErrorIpv6NetworkParams, subnets) {
    const auto expected_error = std::get<3>(GetParam());
    const auto expected_error_str = std::get<4>(GetParam());

    const auto network = ipv6_network::parse(std::get<0>(GetParam()));
    const auto prefixlen_diff = std::get<1>(GetParam());
    const auto new_prefix = std::get<2>(GetParam());

    error_code err;
    const auto actual = network.subnets(err, prefixlen_diff, new_prefix);
    ASSERT_EQ(err, expected_error);
    ASSERT_TRUE(actual.empty());

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_subnets = network.subnets(prefixlen_diff, new_prefix);
    ASSERT_TRUE(error_subnets.empty());
#else
    EXPECT_THAT(
        ([&network, prefixlen_diff, new_prefix]() { network.subnets(prefixlen_diff, new_prefix); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, SubnetsErrorIpv6NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/24", 1, 23, error_code::NEW_PREFIX_MUST_BE_LONGER, "new prefix must be longer"),
        std::make_tuple("192.0.2.0/24", 2, 25, error_code::CANNOT_SET_PREFIXLEN_DIFF_AND_NEW_PREFIX, "cannot set prefixlen_diff and new_prefix"),
        std::make_tuple("192.0.2.0/24", 1, 33, error_code::INVALID_PREFIXLEN_DIFF, "invalid prefixlen_diff")
    ));

using AddressExcludeIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, std::vector<const char*>>>;
TEST_P(AddressExcludeIpv6NetworkParams, address_exclude) {
    std::vector<ipv6_network> expected;
    for (const auto& addr : std::get<2>(GetParam())) {
        expected.push_back(ipv6_network::parse(addr));
    }
    
    const auto actual = ipv6_network::parse(std::get<0>(GetParam()))
        .address_exclude(ipv6_network::parse(std::get<1>(GetParam())));

    ASSERT_EQ(actual.empty(), expected.empty());
    
    auto expected_it = expected.begin();
    for (const auto& address : actual) {
        ASSERT_EQ(address, *expected_it++);
    }

    auto expected_const_it = expected.cbegin();
    for (auto it = actual.cbegin(); it != actual.cend(); ++it) {
        ASSERT_EQ(*it, *expected_const_it++);
    }
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, AddressExcludeIpv6NetworkParams,
    Values(
        std::make_tuple("192.0.2.0/28", "192.0.2.1/32", std::vector<const char*>{"192.0.2.8/29", "192.0.2.4/30", "192.0.2.2/31", "192.0.2.0/32" })
    ));

using AddressExcludeErrorIpv6NetworkParams = TestWithParam<std::tuple<const char*, const char*, error_code, const char*>>;
TEST_P(AddressExcludeErrorIpv6NetworkParams, address_exclude) {
    const auto expected_error = std::get<2>(GetParam());
    const auto expected_error_str = std::get<3>(GetParam());

    const auto network1 = ipv6_network::parse(std::get<0>(GetParam()));
    const auto network2 = ipv6_network::parse(std::get<1>(GetParam()));
    
    error_code err;
    const auto actual = network1.address_exclude(network2, err);
    ASSERT_EQ(err, expected_error);
    ASSERT_TRUE(actual.empty());

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_address_exclude = network1.address_exclude(network2);
    ASSERT_TRUE(error_address_exclude.empty());
#else
    EXPECT_THAT(
        ([&network1, &network2]() { network1.address_exclude(network2); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, AddressExcludeErrorIpv6NetworkParams,
    Values(
        std::make_tuple("192.168.1.128/30", "192.168.1.0/24", error_code::NOT_CONTAINED_NETWORK, "network is not a subnet of other")
    ));
*/