#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#ifdef IPADDRESS_TEST_MODULE
import ipaddress;
#else
#include <ipaddress/ipaddress.hpp>
#endif

using namespace testing;
using namespace ipaddress;

TEST(ip_network, DefaultCtor) {
    IPADDRESS_CONSTEXPR ip_network net;

    IPADDRESS_CONSTEXPR auto actual = net.v4();
    IPADDRESS_CONSTEXPR auto version = net.version();
    IPADDRESS_CONSTEXPR auto size = net.size();
    IPADDRESS_CONSTEXPR auto is_v4 = net.is_v4();
    IPADDRESS_CONSTEXPR auto is_v6 = net.is_v6();
    IPADDRESS_CONSTEXPR auto has_value = actual.has_value();

    EXPECT_EQ(version, ip_version::V4);
    EXPECT_EQ(size, 4);
    EXPECT_TRUE(is_v4);
    EXPECT_FALSE(is_v6);
    EXPECT_TRUE(has_value);

    IPADDRESS_CONSTEXPR auto value = actual.value();
    IPADDRESS_CONSTEXPR auto network_address = value.network_address().to_uint();
    IPADDRESS_CONSTEXPR auto broadcast_address = value.broadcast_address().to_uint();
    IPADDRESS_CONSTEXPR auto netmask = value.netmask().to_uint();
    IPADDRESS_CONSTEXPR auto hostmask = value.hostmask().to_uint();
    IPADDRESS_CONSTEXPR auto prefixlen = value.prefixlen();
    IPADDRESS_CONSTEXPR auto ipv4_version = value.version();
    EXPECT_EQ(network_address, 0);
    EXPECT_EQ(broadcast_address, 0);
    EXPECT_EQ(netmask, 0xFFFFFFFF);
    EXPECT_EQ(hostmask, 0);
    EXPECT_EQ(prefixlen, 32);
    EXPECT_EQ(ipv4_version, ip_version::V4);
}

TEST(ip_network, CopyCtor) {
    IPADDRESS_CONSTEXPR auto net = ip_network::parse("127.0.0.0/24");
    IPADDRESS_CONSTEXPR auto net_copy = net;

    IPADDRESS_CONSTEXPR auto actual_net = net.v4();
    IPADDRESS_CONSTEXPR auto actual_net_copy = net_copy.v4();

    EXPECT_EQ(net.version(), ip_version::V4);
    EXPECT_EQ(net.size(), 4);
    EXPECT_TRUE(net.is_v4());
    EXPECT_FALSE(net.is_v6());
    EXPECT_TRUE(actual_net.has_value());

    EXPECT_EQ(net_copy.version(), ip_version::V4);
    EXPECT_EQ(net_copy.size(), 4);
    EXPECT_TRUE(net_copy.is_v4());
    EXPECT_FALSE(net_copy.is_v6());
    EXPECT_TRUE(actual_net_copy.has_value());

    IPADDRESS_CONSTEXPR auto value_net = actual_net.value();
    IPADDRESS_CONSTEXPR auto value_net_copy = actual_net_copy.value();
    EXPECT_EQ(value_net.network_address().to_uint(), 0x7F000000);
    EXPECT_EQ(value_net.broadcast_address().to_uint(), 0x7F0000FF);
    EXPECT_EQ(value_net.netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(value_net.hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(value_net.prefixlen(), 24);
    EXPECT_EQ(value_net.version(), ip_version::V4);

    EXPECT_EQ(value_net_copy.network_address().to_uint(), 0x7F000000);
    EXPECT_EQ(value_net_copy.broadcast_address().to_uint(), 0x7F0000FF);
    EXPECT_EQ(value_net_copy.netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(value_net_copy.hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(value_net_copy.prefixlen(), 24);
    EXPECT_EQ(value_net_copy.version(), ip_version::V4);
}

TEST(ip_network, CopyOperator) {
    IPADDRESS_CONSTEXPR auto net = 
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    ip_network::parse<"::ffff:0:0">();
#else
    ip_network::parse("::ffff:0:0");
#endif
    ip_network net_copy = ip_network::parse("127.0.0.1");

    auto actual_net4_copy = net_copy.v4();
    auto actual_net6_copy = net_copy.v6();
    const auto value_net4_copy = actual_net4_copy.value();
    EXPECT_EQ(net_copy.version(), ip_version::V4);
    EXPECT_EQ(net_copy.size(), 4);
    EXPECT_TRUE(net_copy.is_v4());
    EXPECT_FALSE(net_copy.is_v6());
    EXPECT_TRUE(actual_net4_copy.has_value());
    EXPECT_FALSE(actual_net6_copy.has_value());
    EXPECT_EQ(value_net4_copy.network_address().to_uint(), 0x7F000001);
    
    net_copy = net;
    actual_net4_copy = net_copy.v4();
    actual_net6_copy = net_copy.v6();
    const auto value_net6_copy = actual_net6_copy.value();
    EXPECT_EQ(net_copy.version(), ip_version::V6);
    EXPECT_EQ(net_copy.size(), 16);
    EXPECT_FALSE(net_copy.is_v4());
    EXPECT_TRUE(net_copy.is_v6());
    EXPECT_FALSE(actual_net4_copy.has_value());
    EXPECT_TRUE(actual_net6_copy.has_value());
    EXPECT_EQ(value_net6_copy.network_address().to_uint(), uint128_t::from_string("281470681743360").value());
}

TEST(ip_network, Ctors) {
    IPADDRESS_CONSTEXPR auto net4 = ipv4_network::parse("192.168.0.1");
    IPADDRESS_CONSTEXPR auto net6 = ipv6_network::parse("2002:ac1d:2d64::1");

    IPADDRESS_CONSTEXPR ip_network actual1(net4);
    IPADDRESS_CONSTEXPR ip_network actual2(net6);
    
    auto actual1_net = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_net.has_value());
    EXPECT_EQ(actual1_net.value().network_address().to_uint(), 0xC0A80001);
    EXPECT_EQ(actual1_net.value().broadcast_address().to_uint(), 0xC0A80001);
    EXPECT_EQ(actual1_net.value().netmask().to_uint(), 0xFFFFFFFF);
    EXPECT_EQ(actual1_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual1_net.value().prefixlen(), 32);

    auto actual2_net = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_net.has_value());
    EXPECT_EQ(actual2_net.value().network_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().broadcast_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().netmask().to_uint(), uint128_t::from_string("340282366920938463463374607431768211455").value());
    EXPECT_EQ(actual2_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual2_net.value().prefixlen(), 128);
}

TEST(ip_network, from_address) {
    IPADDRESS_CONSTEXPR auto ip4 = ipv4_address::parse("192.168.0.1");
    IPADDRESS_CONSTEXPR auto ip6 = ip_address::parse("2002:ac1d:2d64::1");

    IPADDRESS_CONSTEXPR ip_network actual1 = ip_network::from_address(ip4, 24, false);
    IPADDRESS_CONSTEXPR ip_network actual2 = ip_network::from_address(ip6, 128);
    
    auto actual1_net = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_net.has_value());
    EXPECT_EQ(actual1_net.value().network_address().to_uint(), 0xC0A80000);
    EXPECT_EQ(actual1_net.value().broadcast_address().to_uint(), 0xC0A800FF);
    EXPECT_EQ(actual1_net.value().netmask().to_uint(), 0xFFFFFF00);
    EXPECT_EQ(actual1_net.value().hostmask().to_uint(), 0x000000FF);
    EXPECT_EQ(actual1_net.value().prefixlen(), 24);

    auto actual2_net = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_net.has_value());
    EXPECT_EQ(actual2_net.value().network_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().broadcast_address().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    EXPECT_EQ(actual2_net.value().netmask().to_uint(), uint128_t::from_string("340282366920938463463374607431768211455").value());
    EXPECT_EQ(actual2_net.value().hostmask().to_uint(), 0);
    EXPECT_EQ(actual2_net.value().prefixlen(), 128);
}

TEST(ip_network, parse_utf) {
    constexpr ipv6_address::base_type ip_bytes { 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

    const auto* str1 = L"127.0.0.1";
    const auto* str2 = L"2001:db8::1";
    const auto* str3 = u"127.0.0.1";
    const auto* str4 = u"2001:db8::1";
    const auto* str5 = U"127.0.0.1";
    const auto* str6 = U"2001:db8::1";

    auto ip_wchar_1 = ip_network::parse(str1);
    auto ip_wchar_2 = ip_network::parse(str2);
    auto ip_char16_3 = ip_network::parse(str3);
    auto ip_char16_4 = ip_network::parse(str4);
    auto ip_char32_5 = ip_network::parse(str5);
    auto ip_char32_6 = ip_network::parse(str6);
    ASSERT_EQ(ip_wchar_1.v4().value().network_address().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_wchar_2.v6().value().network_address().bytes(), ip_bytes);
    ASSERT_EQ(ip_char16_3.v4().value().network_address().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char16_4.v6().value().network_address().bytes(), ip_bytes);
    ASSERT_EQ(ip_char32_5.v4().value().network_address().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char32_6.v6().value().network_address().bytes(), ip_bytes);

#if __cpp_char8_t >= 201811L
    auto str7 = u8"127.0.0.1";
    auto str8 = u8"2001:db8::1";
    auto ip_char7_8 = ip_network::parse(str7);
    auto ip_char8_8 = ip_network::parse(str8);
    ASSERT_EQ(ip_char7_8.v4().value().network_address().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char8_8.v6().value().network_address().bytes(), ip_bytes);
#endif
}

TEST(ip_network, parse) {
    const char* str1 = "100.64.0.0";
    const char* str2 = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("192.168.1.1");
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("fe80::1ff:fe23:4567:890a");
    auto actual3 = ip_network::parse(str1);
    auto actual4 = ip_network::parse(str2);
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5 = ip_network::parse<"127.0.0.1">();
    constexpr auto actual6 = ip_network::parse<"fe80::1ff:fe23:4567:890a%eth2">();
#endif
    
    ip_network actual7;
    ip_network actual8;
    std::string s1;
    std::string s2;
    std::istringstream ss1("test: 255.0.42.42/24 parser");
    std::istringstream ss2("test: 2001:db8:0:0:1:0:0:1 parser");
    ss1 >> s1 >> non_strict >> actual7 >> s2;
    ss2 >> s1 >> actual8 >> s2;

    IPADDRESS_CONSTEXPR auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().network_address().to_uint(), 0xC0A80101);

    IPADDRESS_CONSTEXPR auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().network_address().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().network_address().to_uint(), 0x64400000);
    
    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().network_address().to_uint(), uint128_t::from_string("42540766452641154071740215577757643572").value());
    
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5_ip = actual5.v4();
    EXPECT_EQ(actual5.version(), ip_version::V4);
    EXPECT_EQ(actual5.size(), 4);
    EXPECT_TRUE(actual5.is_v4());
    EXPECT_FALSE(actual5.is_v6());
    EXPECT_TRUE(actual5_ip.has_value());
    EXPECT_EQ(actual5_ip.value().network_address().to_uint(), 0x7F000001);
    
    constexpr auto actual6_ip = actual6.v6();
    EXPECT_EQ(actual6.version(), ip_version::V6);
    EXPECT_EQ(actual6.size(), 16);
    EXPECT_FALSE(actual6.is_v4());
    EXPECT_TRUE(actual6.is_v6());
    EXPECT_TRUE(actual6_ip.has_value());
    EXPECT_EQ(actual6_ip.value().network_address().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
#endif

    auto actual7_ip = actual7.v4();
    EXPECT_EQ(actual7.version(), ip_version::V4);
    EXPECT_EQ(actual7.size(), 4);
    EXPECT_TRUE(actual7.is_v4());
    EXPECT_FALSE(actual7.is_v6());
    EXPECT_TRUE(actual7_ip.has_value());
    EXPECT_EQ(actual7_ip.value().network_address().to_uint(), 0xFF002A00);
    
    auto actual8_ip = actual8.v6();
    EXPECT_EQ(actual8.version(), ip_version::V6);
    EXPECT_EQ(actual8.size(), 16);
    EXPECT_FALSE(actual8.is_v4());
    EXPECT_TRUE(actual8.is_v6());
    EXPECT_TRUE(actual8_ip.has_value());
    EXPECT_EQ(actual8_ip.value().network_address().to_uint(), uint128_t::from_string("42540766411282592856904266426630537217").value());
}

using InvalidNetworkParams = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkParams, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err = error_code::no_error;
    ip_network::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_net = ip_network::parse(expected_address);

    EXPECT_EQ(error_net.network_address().to_uint128(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ip_network::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ip_network::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#else
    ASSERT_THROW(ip_network::parse(expected_address), parse_error);
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ip_network, InvalidNetworkParams,
    testing::Values(
        std::make_tuple("/8", error_code::empty_address, "address cannot be empty"),
        std::make_tuple("1.2.3.4/", error_code::empty_netmask, "empty mask in address 1.2.3.4/"),
        std::make_tuple("1.2.3.4/33", error_code::least_3_parts, "least 3 parts in address 1.2.3.4/33"),
        std::make_tuple("1.2.3.4/255.255.255.127", error_code::invalid_netmask, "is not a valid netmask in address 1.2.3.4/255.255.255.127"),
        std::make_tuple("1.2.3.4/24", error_code::least_3_parts, "least 3 parts in address 1.2.3.4/24"),
        std::make_tuple("1.2.3.4//", error_code::only_one_slash_permitted, "only one '/' permitted in address 1.2.3.4//"),
        std::make_tuple("bogus", error_code::part_is_more_4_chars, "in part 0 of address bogus more 4 characters"),
        std::make_tuple("10/8", error_code::least_3_parts, "least 3 parts in address 10/8"),
        std::make_tuple("1.2.3.256", error_code::octet_exceeded_255, "octet 0 of address 1.2.3.256 exceeded 255"),
        std::make_tuple("/", error_code::empty_netmask, "empty mask in address /"),
        std::make_tuple("2001:db8::/129", error_code::invalid_netmask, "is not a valid netmask in address 2001:db8::/129"),
        std::make_tuple("2001:db8::/24", error_code::has_host_bits_set, "has host bits set in address 2001:db8::/24"),
        std::make_tuple("google.com", error_code::octet_has_invalid_symbol, "in octet 0 of address google.com has invalid symbol"),
        std::make_tuple("10/8", error_code::least_3_parts, "least 3 parts in address 10/8"),
        std::make_tuple("1234:axy::b", error_code::part_has_invalid_symbol, "in part 0 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("google.com%scope", error_code::octet_has_invalid_symbol, "in octet 0 of address google.com%scope has invalid symbol"),
        std::make_tuple("1.2.3.4%scope", error_code::least_3_parts, "least 3 parts in address 1.2.3.4%scope"),
        std::make_tuple("10%scope/8", error_code::least_3_parts, "least 3 parts in address 10%scope/8"),
        std::make_tuple("1234:axy::b%scope", error_code::part_has_invalid_symbol, "in part 0 of address 1234:axy::b%scope has invalid symbols")
    ));

template <typename T, size_t N>
static void parse_unexpected_symbol(const T (&expected_address)[N]) {
    using tstring = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

    error_code err1 = error_code::no_error;
    error_code err2 = error_code::no_error;

    ip_network::parse(expected_address, err1);
    ip_network::parse(tstring(expected_address), err2);
    ASSERT_EQ(err1, error_code::unexpected_symbol);
    ASSERT_EQ(err2, error_code::unexpected_symbol);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_ip1 = ip_network::parse(expected_address);
    auto error_ip2 = ip_network::parse(tstring(expected_address));
    ASSERT_EQ(error_ip1.network_address().to_uint128(), 0);
    ASSERT_EQ(error_ip2.network_address().to_uint128(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ip_network::parse(address); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+d55c} in string 2001:dc8::/1{U+d55c}2{U+d55c}")));
    EXPECT_THAT(
        [address=expected_address]() { ip_network::parse(tstring(address)); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+d55c} in string 2001:dc8::/1{U+d55c}2{U+d55c}")));
    EXPECT_THAT(
        [address=expected_address]() { ip_network::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
#else // googletest EXPECT_THAT is not supported in cpp less than 14
    ASSERT_THROW(ip_network::parse(expected_address), parse_error);
    ASSERT_THROW((ip_network::parse(tstring(expected_address))), parse_error);
#endif
}
#define PARSE_UNEXPECTED_SYMBOL(unicode) parse_unexpected_symbol(unicode##"2001:dc8::/1\ud55c2\ud55c")

#if __cpp_char8_t >= 201811L
TEST(ip_network, ParseUnexpectedUtf8) {
    PARSE_UNEXPECTED_SYMBOL(u8);
}
#endif

TEST(ip_network, ParseUnexpectedUtf16) {
    PARSE_UNEXPECTED_SYMBOL(u);
}

TEST(ip_network, ParseUnexpectedUtf32) {
    PARSE_UNEXPECTED_SYMBOL(U);
}

TEST(ip_network, ParseUnexpectedWideChar) {
    PARSE_UNEXPECTED_SYMBOL(L);
}

TEST(ip_network, Comparison) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/32");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("2001:db8::/64");
    IPADDRESS_CONSTEXPR auto net3 = ip_network::parse("2001:db8::/65");

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

    ASSERT_FALSE(net3 < net2);
    ASSERT_FALSE(net3 <= net2);
    ASSERT_TRUE(net3 > net2);
    ASSERT_TRUE(net3 >= net2);
    ASSERT_FALSE(net3 == net2);
    ASSERT_TRUE(net3 != net2);
}

TEST(ip_network, to_string) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::string expected_address = "127.240.0.0/24";
    const char* expected_full_2 = "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2/128";
    const char* expected_compact_2 = "fe80:0:0:0:1ff:fe23:4567:890a%eth2/128";
    const char* expected_compressed_2 = "fe80::1ff:fe23:4567:890a%eth2/128";
    const char* expected_compressed_upper_2 = "FE80::1FF:FE23:4567:890A%eth2/128";

    std::ostringstream ss_full; ss_full << full << net1;
    std::ostringstream ss_default; ss_default << net1;
    std::ostringstream ss_compact; ss_compact << compact << net1;
    std::ostringstream ss_compressed; ss_compressed << compressed << net1;
    std::ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << net1;

    std::ostringstream ss_full_2; ss_full_2 << full << net2;
    std::ostringstream ss_default_2; ss_default_2 << net2;
    std::ostringstream ss_compact_2; ss_compact_2 << compact << net2;
    std::ostringstream ss_compressed_2; ss_compressed_2 << compressed << net2;
    std::ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << net2;
    
    ASSERT_EQ(net1.to_string(format::full), expected_address);
    ASSERT_EQ(net1.to_string(format::compact), expected_address);
    ASSERT_EQ(net1.to_string(format::compressed),expected_address);
    ASSERT_EQ(net1.to_string(), expected_address);
    ASSERT_EQ((std::string) net1, expected_address);
    ASSERT_EQ(std::to_string(net1), expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(net2.to_string(format::full), expected_full_2);
    ASSERT_EQ(net2.to_string(format::compact), expected_compact_2);
    ASSERT_EQ(net2.to_string(format::compressed), expected_compressed_2);
    ASSERT_EQ(net2.to_string(), expected_compressed_2);
    ASSERT_EQ((std::string) net2, expected_compressed_2);
    ASSERT_EQ(std::to_string(net2), expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

TEST(ip_network, to_wstring) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::wstring expected_address = L"127.240.0.0/24";
    const wchar_t* expected_full_2 = L"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2/128";
    const wchar_t* expected_compact_2 = L"fe80:0:0:0:1ff:fe23:4567:890a%eth2/128";
    const wchar_t* expected_compressed_2 = L"fe80::1ff:fe23:4567:890a%eth2/128";
    const wchar_t* expected_compressed_upper_2 = L"FE80::1FF:FE23:4567:890A%eth2/128";

    std::wostringstream ss_full; ss_full << full << net1;
    std::wostringstream ss_default; ss_default << net1;
    std::wostringstream ss_compact; ss_compact << compact << net1;
    std::wostringstream ss_compressed; ss_compressed << compressed << net1;
    std::wostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << net1;

    std::wostringstream ss_full_2; ss_full_2 << full << net2;
    std::wostringstream ss_default_2; ss_default_2 << net2;
    std::wostringstream ss_compact_2; ss_compact_2 << compact << net2;
    std::wostringstream ss_compressed_2; ss_compressed_2 << compressed << net2;
    std::wostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << net2;
    
    ASSERT_EQ(net1.to_wstring(format::full), expected_address);
    ASSERT_EQ(net1.to_wstring(format::compact), expected_address);
    ASSERT_EQ(net1.to_wstring(format::compressed),expected_address);
    ASSERT_EQ(net1.to_wstring(), expected_address);
    ASSERT_EQ((std::wstring) net1, expected_address);
    ASSERT_EQ(std::to_wstring(net1), expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(net2.to_wstring(format::full), expected_full_2);
    ASSERT_EQ(net2.to_wstring(format::compact), expected_compact_2);
    ASSERT_EQ(net2.to_wstring(format::compressed), expected_compressed_2);
    ASSERT_EQ(net2.to_wstring(), expected_compressed_2);
    ASSERT_EQ((std::wstring) net2, expected_compressed_2);
    ASSERT_EQ(std::to_wstring(net2), expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

TEST(ip_network, to_u16string) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u16string expected_address = u"127.240.0.0/24";
    const char16_t* expected_full_2 = u"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2/128";
    const char16_t* expected_compact_2 = u"fe80:0:0:0:1ff:fe23:4567:890a%eth2/128";
    const char16_t* expected_compressed_2 = u"fe80::1ff:fe23:4567:890a%eth2/128";

    ASSERT_EQ(net1.to_u16string(format::full), expected_address);
    ASSERT_EQ(net1.to_u16string(format::compact), expected_address);
    ASSERT_EQ(net1.to_u16string(format::compressed),expected_address);
    ASSERT_EQ(net1.to_u16string(), expected_address);
    ASSERT_EQ((std::u16string) net1, expected_address);

    ASSERT_EQ(net2.to_u16string(format::full), expected_full_2);
    ASSERT_EQ(net2.to_u16string(format::compact), expected_compact_2);
    ASSERT_EQ(net2.to_u16string(format::compressed), expected_compressed_2);
    ASSERT_EQ(net2.to_u16string(), expected_compressed_2);
    ASSERT_EQ((std::u16string) net2, expected_compressed_2);
}

TEST(ip_network, to_u32string) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u32string expected_address = U"127.240.0.0/24";
    const char32_t* expected_full_2 = U"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2/128";
    const char32_t* expected_compact_2 = U"fe80:0:0:0:1ff:fe23:4567:890a%eth2/128";
    const char32_t* expected_compressed_2 = U"fe80::1ff:fe23:4567:890a%eth2/128";

    ASSERT_EQ(net1.to_u32string(format::full), expected_address);
    ASSERT_EQ(net1.to_u32string(format::compact), expected_address);
    ASSERT_EQ(net1.to_u32string(format::compressed),expected_address);
    ASSERT_EQ(net1.to_u32string(), expected_address);
    ASSERT_EQ((std::u32string) net1, expected_address);

    ASSERT_EQ(net2.to_u32string(format::full), expected_full_2);
    ASSERT_EQ(net2.to_u32string(format::compact), expected_compact_2);
    ASSERT_EQ(net2.to_u32string(format::compressed), expected_compressed_2);
    ASSERT_EQ(net2.to_u32string(), expected_compressed_2);
    ASSERT_EQ((std::u32string) net2, expected_compressed_2);
}

#if __cpp_char8_t >= 201811L
TEST(ip_network, to_u8string) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.240.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u8string expected_address = u8"127.240.0.0/24";
    const char8_t* expected_full_2 = u8"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2/128";
    const char8_t* expected_compact_2 = u8"fe80:0:0:0:1ff:fe23:4567:890a%eth2/128";
    const char8_t* expected_compressed_2 = u8"fe80::1ff:fe23:4567:890a%eth2/128";

    ASSERT_EQ(net1.to_u8string(format::full), expected_address);
    ASSERT_EQ(net1.to_u8string(format::compact), expected_address);
    ASSERT_EQ(net1.to_u8string(format::compressed),expected_address);
    ASSERT_EQ(net1.to_u8string(), expected_address);
    ASSERT_EQ((std::u8string) net1, expected_address);

    ASSERT_EQ(net2.to_u8string(format::full), expected_full_2);
    ASSERT_EQ(net2.to_u8string(format::compact), expected_compact_2);
    ASSERT_EQ(net2.to_u8string(format::compressed), expected_compressed_2);
    ASSERT_EQ(net2.to_u8string(), expected_compressed_2);
    ASSERT_EQ((std::u8string) net2, expected_compressed_2);
}
#endif

TEST(ip_network, Hash) {
    IPADDRESS_CONSTEXPR auto hash_functor = std::hash<ip_network>{};

    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.0.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("2001:db8::/64");

    IPADDRESS_CONSTEXPR auto hash1 = net1.hash();
    IPADDRESS_CONSTEXPR auto hash2 = net2.hash();
    IPADDRESS_CONSTEXPR auto hash3 = hash_functor(net1);
    IPADDRESS_CONSTEXPR auto hash4 = hash_functor(net2);

    ASSERT_EQ(hash1, sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(hash2, sizeof(size_t) == 8 ? 9261008770321846004ULL : 3598479863U);
    ASSERT_EQ(hash3, sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(hash4, sizeof(size_t) == 8 ? 9261008770321846004ULL : 3598479863U);
}

TEST(ip_network, Containers) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("127.0.0.0/24");
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("2001:db8::/64");
    IPADDRESS_CONSTEXPR auto net3 = ip_network::parse("2001:db8::/65");

    std::vector<ip_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ip_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net1);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net3);
    
    auto net3_with_scope = ip_network::parse("2001:db8::%scope/65");
    std::unordered_map<ip_network, int> unordered_map;
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

TEST(ip_network, Swap) {
    auto net1 = ip_network::parse("127.0.0.1");
    auto net2 = ip_network::parse("2001:db8::2%test");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ip_network::parse("2001:db8::2%test"));
    ASSERT_EQ(net2, ip_network::parse("127.0.0.1"));
}

TEST(ip_network, is_prop) {
    IPADDRESS_CONSTEXPR auto net1 = ip_network::parse("224.1.1.1").is_multicast();
    IPADDRESS_CONSTEXPR auto net2 = ip_network::parse("240.0.0.0").is_multicast();
    IPADDRESS_CONSTEXPR auto net3 = ip_network::parse("ffff::").is_multicast();
    IPADDRESS_CONSTEXPR auto net4 = ip_network::parse("fdff::").is_multicast();
    IPADDRESS_CONSTEXPR auto net5 = ip_network::parse("192.168.1.1").is_private();
    IPADDRESS_CONSTEXPR auto net6 = ip_network::parse("192.169.0.0").is_private();
    IPADDRESS_CONSTEXPR auto net7 = ip_network::parse("fc00::").is_private();
    IPADDRESS_CONSTEXPR auto net8 = ip_network::parse("fbff:ffff::").is_private();
    IPADDRESS_CONSTEXPR auto net9 = ip_network::parse("100.64.1.0/24").is_private();
    IPADDRESS_CONSTEXPR auto net10 = ip_network::parse("2001:20::/28").is_private();
    IPADDRESS_CONSTEXPR auto net11 = ip_network::parse("192.0.7.1").is_global();
    IPADDRESS_CONSTEXPR auto net12 = ip_network::parse("203.0.113.1").is_global();
    IPADDRESS_CONSTEXPR auto net13 = ip_network::parse("200::1").is_global();
    IPADDRESS_CONSTEXPR auto net14 = ip_network::parse("fc00::").is_global();
    IPADDRESS_CONSTEXPR auto net15 = ip_network::parse("100.64.1.0/24").is_global();
    IPADDRESS_CONSTEXPR auto net16 = ip_network::parse("2606:4700:4700::/48").is_global();
    IPADDRESS_CONSTEXPR auto net17 = ip_network::parse("240.0.0.1").is_reserved();
    IPADDRESS_CONSTEXPR auto net18 = ip_network::parse("239.255.255.255").is_reserved();
    IPADDRESS_CONSTEXPR auto net19 = ip_network::parse("100::").is_reserved();
    IPADDRESS_CONSTEXPR auto net20 = ip_network::parse("ffff::").is_reserved();
    IPADDRESS_CONSTEXPR auto net21 = ip_network::parse("127.42.0.0").is_loopback();
    IPADDRESS_CONSTEXPR auto net22 = ip_network::parse("128.0.0.0").is_loopback();
    IPADDRESS_CONSTEXPR auto net23 = ip_network::parse("::1").is_loopback();
    IPADDRESS_CONSTEXPR auto net24 = ip_network::parse("::2").is_loopback();
    IPADDRESS_CONSTEXPR auto net25 = ip_network::parse("169.254.100.200").is_link_local();
    IPADDRESS_CONSTEXPR auto net26 = ip_network::parse("169.255.100.200").is_link_local();
    IPADDRESS_CONSTEXPR auto net27 = ip_network::parse("fea0::").is_link_local();
    IPADDRESS_CONSTEXPR auto net28 = ip_network::parse("fe7f:ffff::").is_link_local();
    IPADDRESS_CONSTEXPR auto net29 = ip_network::parse("0.0.0.0").is_unspecified();
    IPADDRESS_CONSTEXPR auto net30 = ip_network::parse("127.0.0.1").is_unspecified();
    IPADDRESS_CONSTEXPR auto net31 = ip_network::parse("::").is_unspecified();
    IPADDRESS_CONSTEXPR auto net32 = ip_network::parse("::1").is_unspecified();
    IPADDRESS_CONSTEXPR auto net33 = ip_network::parse("127.0.0.1").is_site_local();
    IPADDRESS_CONSTEXPR auto net34 = ip_network::parse("0.0.0.0").is_site_local();
    IPADDRESS_CONSTEXPR auto net35 = ip_network::parse("fecf::").is_site_local();
    IPADDRESS_CONSTEXPR auto net36 = ip_network::parse("fbf:ffff::").is_site_local();
    
    ASSERT_TRUE(net1);
    ASSERT_FALSE(net2);
    ASSERT_TRUE(net3);
    ASSERT_FALSE(net4);
    ASSERT_TRUE(net5);
    ASSERT_FALSE(net6);
    ASSERT_TRUE(net7);
    ASSERT_FALSE(net8);
    ASSERT_FALSE(net9);
    ASSERT_FALSE(net10);
    ASSERT_TRUE(net11);
    ASSERT_FALSE(net12);
    ASSERT_TRUE(net13);
    ASSERT_FALSE(net14);
    ASSERT_FALSE(net15);
    ASSERT_TRUE(net16);
    ASSERT_TRUE(net17);
    ASSERT_FALSE(net18);
    ASSERT_TRUE(net19);
    ASSERT_FALSE(net20);
    ASSERT_TRUE(net21);
    ASSERT_FALSE(net22);
    ASSERT_TRUE(net23);
    ASSERT_FALSE(net24);
    ASSERT_TRUE(net25);
    ASSERT_FALSE(net26);
    ASSERT_TRUE(net27);
    ASSERT_FALSE(net28);
    ASSERT_TRUE(net29);
    ASSERT_FALSE(net30);
    ASSERT_TRUE(net31);
    ASSERT_FALSE(net32);
    ASSERT_FALSE(net33);
    ASSERT_FALSE(net34);
    ASSERT_TRUE(net35);
    ASSERT_FALSE(net36);
}

TEST(ip_network, addresses_count) {
    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("10.0.0.0/30").addresses_count();
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("2001:658:22a:cafe::/64").addresses_count();

    ASSERT_EQ(actual1, 4);
    ASSERT_EQ(actual2, uint128_t::from_string("18446744073709551616").value());
}

TEST(ip_network, contains) {
    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("192.0.2.0/28").contains(ip_address::parse("192.0.2.6"));
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("192.0.2.0/28").contains(ip_address::parse("192.0.3.6"));
    IPADDRESS_CONSTEXPR auto actual3 = ip_network::parse("2001:db8::/32").contains(ip_address::parse("2001:db8::1"));
    IPADDRESS_CONSTEXPR auto actual4 = ip_network::parse("2001:db8::/32").contains(ip_address::parse("2001:dbc::"));

    ASSERT_TRUE(actual1);
    ASSERT_FALSE(actual2);
    ASSERT_TRUE(actual3);
    ASSERT_FALSE(actual4);
}

TEST(ip_network, overlaps) {
    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("1.2.3.0/24").overlaps(ip_network::parse("1.2.3.0/30"));
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("1.2.3.0/24").overlaps(ip_network::parse("1.2.2.0/24"));
    IPADDRESS_CONSTEXPR auto actual3 = ip_network::parse("2001:db8::/32").overlaps(ip_network::parse("2001:db8::/128"));
    IPADDRESS_CONSTEXPR auto actual4 = ip_network::parse("2001:dbc::/32").overlaps(ip_network::parse("2001:db8::/32"));

    ASSERT_TRUE(actual1);
    ASSERT_FALSE(actual2);
    ASSERT_TRUE(actual3);
    ASSERT_FALSE(actual4);
}

TEST(ip_network, subnet_of) {
    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("10.0.0.0/30").subnet_of(ip_network::parse("10.0.0.0/24"));
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("10.0.0.0/30").subnet_of(ip_network::parse("10.0.1.0/24"));
    IPADDRESS_CONSTEXPR auto actual3 = ip_network::parse("2000:aaa::/56").subnet_of(ip_network::parse("2000:aaa::/48"));
    IPADDRESS_CONSTEXPR auto actual4 = ip_network::parse("2000:bbb::/56").subnet_of(ip_network::parse("2000:aaa::/48"));

    ASSERT_TRUE(actual1);
    ASSERT_FALSE(actual2);
    ASSERT_TRUE(actual3);
    ASSERT_FALSE(actual4);
}

TEST(ip_network, supernet_of) {
    IPADDRESS_CONSTEXPR auto actual1 = ip_network::parse("192.168.1.0/24").supernet_of(ip_network::parse("192.168.1.128/30"));
    IPADDRESS_CONSTEXPR auto actual2 = ip_network::parse("10.0.0.0/30").supernet_of(ip_network::parse("10.0.1.0/24"));
    IPADDRESS_CONSTEXPR auto actual3 = ip_network::parse("2000:aaa::/48").supernet_of(ip_network::parse("2000:aaa::/56"));
    IPADDRESS_CONSTEXPR auto actual4 = ip_network::parse("2000:aaa::/56").supernet_of(ip_network::parse("2000:aaa::/48"));

    ASSERT_TRUE(actual1);
    ASSERT_FALSE(actual2);
    ASSERT_TRUE(actual3);
    ASSERT_FALSE(actual4);
}

TEST(ip_network, supernet) {
    IPADDRESS_CONSTEXPR auto supernet = ip_network::parse("192.0.2.0/24").supernet();
    ASSERT_EQ(supernet, ip_network::parse("192.0.2.0/23"));
}

TEST(ip_network, hosts) {
    IPADDRESS_CONSTEXPR auto hosts_sequence = ip_network::parse("192.0.2.0/29").hosts();
    IPADDRESS_CONSTEXPR auto hosts_empty = hosts_sequence.empty();
    IPADDRESS_CONSTEXPR auto hosts_size = hosts_sequence.size();
    IPADDRESS_CONSTEXPR auto hosts_front = hosts_sequence.front();
    IPADDRESS_CONSTEXPR auto hosts_back = hosts_sequence.back();
    IPADDRESS_CONSTEXPR auto hosts_at_0 = hosts_sequence.at(0);
    IPADDRESS_CONSTEXPR auto hosts_at_1 = hosts_sequence[1];
    IPADDRESS_CONSTEXPR auto hosts_at_2 = hosts_sequence.at(2);
    IPADDRESS_CONSTEXPR auto hosts_at_3 = hosts_sequence[3];
    IPADDRESS_CONSTEXPR auto hosts_begin = hosts_sequence.begin();
    IPADDRESS_CONSTEXPR auto hosts_end = hosts_sequence.end();
    IPADDRESS_CONSTEXPR auto hosts_rbegin = hosts_sequence.rbegin();
    IPADDRESS_CONSTEXPR auto hosts_rend = hosts_sequence.rend();
    IPADDRESS_CONSTEXPR auto hosts_diff = hosts_end.uint_diff(hosts_begin);
    IPADDRESS_CONSTEXPR auto hosts_begin_0 = *hosts_begin;
    IPADDRESS_CONSTEXPR auto hosts_begin_0_uint = hosts_begin->v4().value().to_uint();
    IPADDRESS_CONSTEXPR auto hosts_begin_at_0 = hosts_begin[0U];
    IPADDRESS_CONSTEXPR auto hosts_begin_at_1 = hosts_begin[1U];
    IPADDRESS_CONSTEXPR auto hosts_begin_at_2 = hosts_begin[2U];
    IPADDRESS_CONSTEXPR auto hosts_begin_at_3 = hosts_begin[3U];
    IPADDRESS_CONSTEXPR auto hosts_it = *(--(++hosts_sequence.begin()++ + 2U) -= 1U);
    IPADDRESS_CONSTEXPR auto hosts_it_eq = hosts_begin == hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_it_ne = hosts_begin != hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_it_ls = hosts_begin < hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_it_le = hosts_begin <= hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_it_gt = hosts_begin > hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_it_ge = hosts_begin >= hosts_end;
    IPADDRESS_CONSTEXPR auto hosts_rdiff = hosts_rend.uint_diff(hosts_rbegin);
    IPADDRESS_CONSTEXPR auto hosts_rbegin_0 = *hosts_rbegin;
    IPADDRESS_CONSTEXPR auto hosts_rbegin_0_uint = hosts_rbegin->v4().value().to_uint();
    IPADDRESS_CONSTEXPR auto hosts_rbegin_at_0 = hosts_rbegin[0U];
    IPADDRESS_CONSTEXPR auto hosts_rbegin_at_1 = hosts_rbegin[1U];
    IPADDRESS_CONSTEXPR auto hosts_rbegin_at_2 = hosts_rbegin[2U];
    IPADDRESS_CONSTEXPR auto hosts_rbegin_at_3 = hosts_rbegin[3U];
    IPADDRESS_CONSTEXPR auto hosts_rit = *(--(++hosts_sequence.rbegin()++ + 2U) -= 1U);
    IPADDRESS_CONSTEXPR auto hosts_rit_eq = hosts_rbegin == hosts_rend;
    IPADDRESS_CONSTEXPR auto hosts_rit_ne = hosts_rbegin != hosts_rend;
    IPADDRESS_CONSTEXPR auto hosts_rit_ls = hosts_rbegin < hosts_rend;
    IPADDRESS_CONSTEXPR auto hosts_rit_le = hosts_rbegin <= hosts_rend;
    IPADDRESS_CONSTEXPR auto hosts_rit_gt = hosts_rbegin > hosts_rend;
    IPADDRESS_CONSTEXPR auto hosts_rit_ge = hosts_rbegin >= hosts_rend;
    ASSERT_FALSE(hosts_empty);
    ASSERT_EQ(hosts_size, 6);
    ASSERT_EQ(hosts_front, ip_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_back, ip_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_at_0, ip_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_at_1, ip_address::parse("192.0.2.2"));
    ASSERT_EQ(hosts_at_2, ip_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_at_3, ip_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_diff, 6);
    ASSERT_EQ(hosts_begin_0, ip_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_begin_0_uint, 0xC0000201);
    ASSERT_EQ(hosts_begin_at_0, ip_address::parse("192.0.2.1"));
    ASSERT_EQ(hosts_begin_at_1, ip_address::parse("192.0.2.2"));
    ASSERT_EQ(hosts_begin_at_2, ip_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_begin_at_3, ip_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_it, ip_address::parse("192.0.2.2"));
    ASSERT_FALSE(hosts_it_eq);
    ASSERT_TRUE(hosts_it_ne);
    ASSERT_TRUE(hosts_it_ls);
    ASSERT_TRUE(hosts_it_le);
    ASSERT_FALSE(hosts_it_gt);
    ASSERT_FALSE(hosts_it_ge);
    ASSERT_EQ(hosts_rdiff, 6);
    ASSERT_EQ(hosts_rbegin_0, ip_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_rbegin_0_uint, 0xC0000206);
    ASSERT_EQ(hosts_rbegin_at_0, ip_address::parse("192.0.2.6"));
    ASSERT_EQ(hosts_rbegin_at_1, ip_address::parse("192.0.2.5"));
    ASSERT_EQ(hosts_rbegin_at_2, ip_address::parse("192.0.2.4"));
    ASSERT_EQ(hosts_rbegin_at_3, ip_address::parse("192.0.2.3"));
    ASSERT_EQ(hosts_rit, ip_address::parse("192.0.2.5"));
    ASSERT_FALSE(hosts_rit_eq);
    ASSERT_TRUE(hosts_rit_ne);
    ASSERT_TRUE(hosts_rit_ls);
    ASSERT_TRUE(hosts_rit_le);
    ASSERT_FALSE(hosts_rit_gt);
    ASSERT_FALSE(hosts_rit_ge);
}

TEST(ip_network, subnets) {
    IPADDRESS_CONSTEXPR auto subnets_sequence = ip_network::parse("192.0.2.0/24").subnets(2);
    IPADDRESS_CONSTEXPR auto subnets_empty = subnets_sequence.empty();
    IPADDRESS_CONSTEXPR auto subnets_size = subnets_sequence.size();
    IPADDRESS_CONSTEXPR auto subnets_front = subnets_sequence.front();
    IPADDRESS_CONSTEXPR auto subnets_back = subnets_sequence.back();
    IPADDRESS_CONSTEXPR auto subnets_at_0 = subnets_sequence.at(0);
    IPADDRESS_CONSTEXPR auto subnets_at_1 = subnets_sequence[1];
    IPADDRESS_CONSTEXPR auto subnets_at_2 = subnets_sequence.at(2);
    IPADDRESS_CONSTEXPR auto subnets_at_3 = subnets_sequence[3];
    IPADDRESS_CONSTEXPR auto subnets_begin = subnets_sequence.begin();
    IPADDRESS_CONSTEXPR auto subnets_end = subnets_sequence.end();
    IPADDRESS_CONSTEXPR auto subnets_rbegin = subnets_sequence.rbegin();
    IPADDRESS_CONSTEXPR auto subnets_rend = subnets_sequence.rend();
    IPADDRESS_CONSTEXPR auto subnets_diff = subnets_end.uint_diff(subnets_begin);
    IPADDRESS_CONSTEXPR auto subnets_begin_0 = *subnets_begin;
    IPADDRESS_CONSTEXPR auto subnets_begin_0_uint = subnets_begin->network_address().to_uint32();
    IPADDRESS_CONSTEXPR auto subnets_begin_at_0 = subnets_begin[0U];
    IPADDRESS_CONSTEXPR auto subnets_begin_at_1 = subnets_begin[1U];
    IPADDRESS_CONSTEXPR auto subnets_begin_at_2 = subnets_begin[2U];
    IPADDRESS_CONSTEXPR auto subnets_begin_at_3 = subnets_begin[3U];
    IPADDRESS_CONSTEXPR auto subnets_it = *(--(++subnets_sequence.begin()++ + 2U) -= 1U);
    IPADDRESS_CONSTEXPR auto subnets_it_eq = subnets_begin == subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_it_ne = subnets_begin != subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_it_ls = subnets_begin < subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_it_le = subnets_begin <= subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_it_gt = subnets_begin > subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_it_ge = subnets_begin >= subnets_end;
    IPADDRESS_CONSTEXPR auto subnets_rdiff = subnets_rend.uint_diff(subnets_rbegin);
    IPADDRESS_CONSTEXPR auto subnets_rbegin_0 = *subnets_rbegin;
    IPADDRESS_CONSTEXPR auto subnets_rbegin_0_uint = subnets_rbegin->network_address().to_uint32();
    IPADDRESS_CONSTEXPR auto subnets_rbegin_at_0 = subnets_rbegin[0U];
    IPADDRESS_CONSTEXPR auto subnets_rbegin_at_1 = subnets_rbegin[1U];
    IPADDRESS_CONSTEXPR auto subnets_rbegin_at_2 = subnets_rbegin[2U];
    IPADDRESS_CONSTEXPR auto subnets_rbegin_at_3 = subnets_rbegin[3U];
    IPADDRESS_CONSTEXPR auto subnets_rit = *(--(++subnets_sequence.rbegin()++ + 2U) -= 1U);
    IPADDRESS_CONSTEXPR auto subnets_rit_eq = subnets_rbegin == subnets_rend;
    IPADDRESS_CONSTEXPR auto subnets_rit_ne = subnets_rbegin != subnets_rend;
    IPADDRESS_CONSTEXPR auto subnets_rit_ls = subnets_rbegin < subnets_rend;
    IPADDRESS_CONSTEXPR auto subnets_rit_le = subnets_rbegin <= subnets_rend;
    IPADDRESS_CONSTEXPR auto subnets_rit_gt = subnets_rbegin > subnets_rend;
    IPADDRESS_CONSTEXPR auto subnets_rit_ge = subnets_rbegin >= subnets_rend;
    ASSERT_FALSE(subnets_empty);
    ASSERT_EQ(subnets_size, 4);
    ASSERT_EQ(subnets_front, ip_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_back, ip_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_at_0, ip_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_at_1, ip_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_at_2, ip_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_at_3, ip_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_diff, 4);
    ASSERT_EQ(subnets_begin_0, ip_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_begin_0_uint, 0xC0000200);
    ASSERT_EQ(subnets_begin_at_0, ip_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_begin_at_1, ip_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_begin_at_2, ip_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_begin_at_3, ip_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_it, ip_network::parse("192.0.2.64/26"));
    ASSERT_FALSE(subnets_it_eq);
    ASSERT_TRUE(subnets_it_ne);
    ASSERT_TRUE(subnets_it_ls);
    ASSERT_TRUE(subnets_it_le);
    ASSERT_FALSE(subnets_it_gt);
    ASSERT_FALSE(subnets_it_ge);
    ASSERT_EQ(subnets_rdiff, 4);
    ASSERT_EQ(subnets_rbegin_0, ip_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_rbegin_0_uint, 0xC00002C0);
    ASSERT_EQ(subnets_rbegin_at_0, ip_network::parse("192.0.2.192/26"));
    ASSERT_EQ(subnets_rbegin_at_1, ip_network::parse("192.0.2.128/26"));
    ASSERT_EQ(subnets_rbegin_at_2, ip_network::parse("192.0.2.64/26"));
    ASSERT_EQ(subnets_rbegin_at_3, ip_network::parse("192.0.2.0/26"));
    ASSERT_EQ(subnets_rit, ip_network::parse("192.0.2.128/26"));
    ASSERT_FALSE(subnets_rit_eq);
    ASSERT_TRUE(subnets_rit_ne);
    ASSERT_TRUE(subnets_rit_ls);
    ASSERT_TRUE(subnets_rit_le);
    ASSERT_FALSE(subnets_rit_gt);
    ASSERT_FALSE(subnets_rit_ge);
}

TEST(ip_network, address_exclude) {
    IPADDRESS_CONSTEXPR auto exclude_sequence = ip_network::parse("192.0.2.0/28").address_exclude(ip_network::parse("192.0.2.1/32"));
    IPADDRESS_CONSTEXPR auto exclude_empty = exclude_sequence.empty();
    IPADDRESS_CONSTEXPR auto exclude_begin = exclude_sequence.begin();
    IPADDRESS_CONSTEXPR auto exclude_end = exclude_sequence.end();
    IPADDRESS_CONSTEXPR auto exclude_begin_0 = *exclude_begin;
    IPADDRESS_CONSTEXPR auto exclude_begin_0_uint = exclude_begin->network_address().to_uint32();
    IPADDRESS_CONSTEXPR auto exclude_begin_1 = *++exclude_sequence.begin();
    ASSERT_FALSE(exclude_empty);
    ASSERT_EQ(exclude_begin_0, ip_network::parse("192.0.2.8/29"));
    ASSERT_EQ(exclude_begin_0_uint, 0xC0000208);
    ASSERT_EQ(exclude_begin_1, ip_network::parse("192.0.2.4/30"));

    IPADDRESS_CONSTEXPR auto exclude_it_eq = exclude_begin == exclude_end;
    IPADDRESS_CONSTEXPR auto exclude_it_ne = exclude_begin != exclude_end;
    IPADDRESS_CONSTEXPR auto exclude_it_ls = exclude_begin < exclude_end;
    IPADDRESS_CONSTEXPR auto exclude_it_le = exclude_begin <= exclude_end;
    IPADDRESS_CONSTEXPR auto exclude_it_gt = exclude_begin > exclude_end;
    IPADDRESS_CONSTEXPR auto exclude_it_ge = exclude_begin >= exclude_end;
    ASSERT_FALSE(exclude_it_eq);
    ASSERT_TRUE(exclude_it_ne);
    ASSERT_TRUE(exclude_it_ls);
    ASSERT_TRUE(exclude_it_le);
    ASSERT_FALSE(exclude_it_gt);
    ASSERT_FALSE(exclude_it_ge);
}

TEST(ip_network, collapse_addresses) {
    IPADDRESS_CONSTEXPR std::array<ip_network, 0> arr_empty{};
    IPADDRESS_CONSTEXPR auto collapsed_arr_empty = collapse_addresses(arr_empty);
    IPADDRESS_CONSTEXPR auto collapsed_arr_empty_size = collapsed_arr_empty.size();
    ASSERT_EQ(collapsed_arr_empty_size, 0);

    IPADDRESS_CONSTEXPR std::array<ip_network, 2> arr = { ip_network::parse("2001:db8::1/128"), ip_network::parse("2001:db8::2/128") };
    IPADDRESS_CONSTEXPR auto collapsed_arr = collapse_addresses(arr);
    IPADDRESS_CONSTEXPR auto collapsed_arr_size = collapsed_arr.size();
    IPADDRESS_CONSTEXPR auto collapsed_arr_0 = collapsed_arr[0];
    IPADDRESS_CONSTEXPR auto collapsed_arr_1 = collapsed_arr[1];
    ASSERT_EQ(collapsed_arr_size, 2);
    ASSERT_EQ(collapsed_arr_0, ip_network::parse("2001:db8::1/128"));
    ASSERT_EQ(collapsed_arr_1, ip_network::parse("2001:db8::2/128"));

    IPADDRESS_CONSTEXPR ip_network nets[] = { ip_network::parse("192.168.1.1/32"), ip_network::parse("192.168.1.0/32") };
    IPADDRESS_CONSTEXPR auto collapsed_nets = collapse_addresses(nets);
    IPADDRESS_CONSTEXPR auto collapsed_nets_size = collapsed_nets.size();
    IPADDRESS_CONSTEXPR auto collapsed_nets_0 = collapsed_nets[0];
    ASSERT_EQ(collapsed_nets_size, 1);
    ASSERT_EQ(collapsed_nets_0, ip_network::parse("192.168.1.0/31"));

    IPADDRESS_CONSTEXPR auto collapsed_0 = collapse_addresses(ip_network::parse("2001:db8::1/128"));
    IPADDRESS_CONSTEXPR auto collapsed_0_size = collapsed_0.size();
    IPADDRESS_CONSTEXPR auto collapsed_0_0 = collapsed_0[0];
    ASSERT_EQ(collapsed_0_size, 1);
    ASSERT_EQ(collapsed_0_0, ip_network::parse("2001:db8::1/128"));

    IPADDRESS_CONSTEXPR auto collapsed_1 = collapse_addresses(ipv6_network::parse("2001:db8::1/128"), ip_network::parse("2001:db8::2/128"));
    IPADDRESS_CONSTEXPR auto collapsed_1_size = collapsed_1.size();
    IPADDRESS_CONSTEXPR auto collapsed_1_0 = collapsed_1[0];
    IPADDRESS_CONSTEXPR auto collapsed_1_1 = collapsed_1[1];
    ASSERT_EQ(collapsed_1_size, 2);
    ASSERT_EQ(collapsed_1_0, ip_network::parse("2001:db8::1/128"));
    ASSERT_EQ(collapsed_1_1, ip_network::parse("2001:db8::2/128"));

    IPADDRESS_CONSTEXPR auto collapsed_2 = collapse_addresses(ip_network::parse("192.168.1.3/32"), ipv4_network::parse("192.168.1.0/32"), ip_network::parse("192.168.1.1/32"));
    IPADDRESS_CONSTEXPR auto collapsed_2_size = collapsed_2.size();
    IPADDRESS_CONSTEXPR auto collapsed_2_0 = collapsed_2[0];
    IPADDRESS_CONSTEXPR auto collapsed_2_1 = collapsed_2[1];
    ASSERT_EQ(collapsed_2_size, 2);
    ASSERT_EQ(collapsed_2_0, ip_network::parse("192.168.1.0/31"));
    ASSERT_EQ(collapsed_2_1, ip_network::parse("192.168.1.3/32"));

    std::vector<ip_network> vec = {
        ip_network::parse("2001:db8::19/128"), ip_network::parse("2001:db8::a/128"), ip_network::parse("2001:db8::b/128"), ip_network::parse("2001:db8::16/128"),
        ip_network::parse("2001:db8::c/128"), ip_network::parse("2001:db8::d/128"), ip_network::parse("2001:db8::10/128"), ip_network::parse("2001:db8::11/128"),
        ip_network::parse("2001:db8::12/128"), ip_network::parse("2001:db8::13/128"), ip_network::parse("2001:db8::e/128"), ip_network::parse("2001:db8::f/128"),
        ip_network::parse("2001:db8::14/128"), ip_network::parse("2001:db8::15/128"), ip_network::parse("2001:db8::17/128"), ip_network::parse("2001:db8::18/128") };
    auto collapsed_3 = collapse_addresses(vec.begin(), vec.end());
    auto collapsed_3_size = collapsed_3.size();
    auto collapsed_3_0 = collapsed_3[0];
    auto collapsed_3_1 = collapsed_3[1];
    auto collapsed_3_2 = collapsed_3[2];
    auto collapsed_3_3 = collapsed_3[3];
    ASSERT_EQ(collapsed_3_size, 4);
    ASSERT_EQ(collapsed_3_0, ip_network::parse("2001:db8::a/127"));
    ASSERT_EQ(collapsed_3_1, ip_network::parse("2001:db8::c/126"));
    ASSERT_EQ(collapsed_3_2, ip_network::parse("2001:db8::10/125"));
    ASSERT_EQ(collapsed_3_3, ip_network::parse("2001:db8::18/127"));
}

using CollapseAddressesErrorNetworkParams = TestWithParam<std::tuple<std::vector<const char*>, error_code, const char*>>;
TEST_P(CollapseAddressesErrorNetworkParams, collapse_addresses) {
    std::vector<ip_network> vec;
    for (const auto& addr : get<0>(GetParam())) {
        vec.push_back(ip_network::parse(addr));
    }
    auto expected_error_code = get<1>(GetParam());

    error_code err = error_code::no_error;
    const auto actual = collapse_addresses(vec.begin(), vec.end(), err);
    ASSERT_EQ(err, expected_error_code);
    ASSERT_TRUE(actual.empty());

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_collapse_addresses = collapse_addresses(vec.begin(), vec.end());
    ASSERT_TRUE(error_collapse_addresses.empty());
#elif IPADDRESS_CPP_VERSION >= 14
    const auto expected_error_str = std::get<2>(GetParam());
    EXPECT_THAT(
        ([&vec]() { auto _ = collapse_addresses(vec.begin(), vec.end()); }),
        ThrowsMessage<logic_error>(StrEq(expected_error_str)));
#else
    ASSERT_THROW(collapse_addresses(vec.begin(), vec.end()), logic_error);
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ip_network, CollapseAddressesErrorNetworkParams,
    testing::Values(
        std::make_tuple(std::vector<const char*>{"2001:db8::1/128", "192.0.2.0/28"}, error_code::invalid_version, "versions don't match"),
        std::make_tuple(std::vector<const char*>{"192.0.2.0/28", "2001:db8::1/128"}, error_code::invalid_version, "versions don't match")
    ));

TEST(ip_network, literals) {
    IPADDRESS_CONSTEXPR auto net1 = "127.128.128.255"_net;
    IPADDRESS_CONSTEXPR auto net2 = "2001:db8::1"_net;
    
    ASSERT_TRUE(net1.is_v4());
    ASSERT_TRUE(net2.is_v6());
    ASSERT_EQ(net1, ip_network::parse("127.128.128.255"));
    ASSERT_EQ(net2, ip_network::parse("2001:db8::1"));
}
