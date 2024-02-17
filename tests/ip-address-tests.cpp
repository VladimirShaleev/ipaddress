#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ip_address, DefaultCtor) {
    constexpr ip_address ip;

    constexpr auto actual = ip.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual.has_value());

    constexpr auto value = actual.value();
    EXPECT_EQ(value.to_uint(), 0);
}

TEST(ip_address, CopyCtor) {
    constexpr auto ip = ip_address::parse("127.0.0.1");
    constexpr auto ip_copy = ip;

    constexpr auto actual_ip = ip.v4();
    constexpr auto actual_ip_copy = ip_copy.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual_ip.has_value());

    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip_copy.has_value());

    constexpr auto value_ip = actual_ip.value();
    constexpr auto value_ip_copy = actual_ip_copy.value();
    EXPECT_EQ(value_ip.to_uint(), 0x7F000001);
    EXPECT_EQ(value_ip_copy.to_uint(), 0x7F000001);
}

TEST(ip_address, CopyOperator) {
    constexpr auto ip = ip_address::parse("::ffff:0:0");
    ip_address ip_copy = ip_address::parse("127.0.0.1");

    auto actual_ip4_copy = ip_copy.v4();
    auto actual_ip6_copy = ip_copy.v6();
    const auto value_ip4_copy = actual_ip4_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip4_copy.has_value());
    EXPECT_FALSE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip4_copy.to_uint(), 0x7F000001);
    
    ip_copy = ip;
    actual_ip4_copy = ip_copy.v4();
    actual_ip6_copy = ip_copy.v6();
    const auto value_ip6_copy = actual_ip6_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V6);
    EXPECT_EQ(ip_copy.size(), 16);
    EXPECT_FALSE(ip_copy.is_v4());
    EXPECT_TRUE(ip_copy.is_v6());
    EXPECT_FALSE(actual_ip4_copy.has_value());
    EXPECT_TRUE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip6_copy.to_uint(), uint128_t::from_string("281470681743360").value());
}

TEST(ip_address, Ctors) {
    constexpr auto ipv4 = ipv4_address::parse("192.168.0.1");
    constexpr auto ipv6 = ipv6_address::parse("2002:ac1d:2d64::1");
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    constexpr ip_address actual1(ipv4);
    constexpr ip_address actual2(ipv6);
    constexpr ip_address actual3(ipv4_bytes);
    constexpr ip_address actual4(ipv6_bytes);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80001);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 281470681743360ULL);
}

TEST(ip_address, from_bytes) {
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    constexpr auto actual1 = ip_address::from_bytes(ipv4_bytes);
    constexpr auto actual2 = ip_address::from_bytes(ipv6_bytes);
    constexpr auto actual3 = ip_address::from_bytes(ipv4_bytes.data(), 3, ip_version::V4);
    constexpr auto actual4 = ip_address::from_bytes(ipv6_bytes.data(), 11, ip_version::V6);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80000);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 280375465082880ULL);
}

TEST(ip_address, from_uint) {
    constexpr ipv4_address::uint_type ip1 = 0xC0A80001;
    constexpr ipv6_address::uint_type ip2 = 281470681743360ULL;

    constexpr auto actual1 = ip_address::from_uint(ip1);
    constexpr auto actual2 = ip_address::from_uint(ip2);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
}

TEST(ip_address, parse) {
    auto str1 = "100.64.0.0";
    auto str2 = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    constexpr auto actual1 = ip_address::parse("192.168.1.1");
    constexpr auto actual2 = ip_address::parse("fe80::1ff:fe23:4567:890a");
    auto actual3 = ip_address::parse(str1);
    auto actual4 = ip_address::parse(str2);
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5 = ip_address::parse<"127.0.0.1">();
    constexpr auto actual6 = ip_address::parse<"fe80::1ff:fe23:4567:890a%eth2">();
#endif
    
    ip_address actual7;
    ip_address actual8;
    std::string s1;
    std::string s2;
    std::istringstream ss1("test: 255.0.42.42 parser");
    std::istringstream ss2("test: 2001:db8:0:0:1:0:0:1 parser");
    ss1 >> s1 >> actual7 >> s2;
    ss2 >> s1 >> actual8 >> s2;

    constexpr auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80101);

    constexpr auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0x64400000);
    
    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), uint128_t::from_string("42540766452641154071740215577757643572").value());
    
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5_ip = actual5.v4();
    EXPECT_EQ(actual5.version(), ip_version::V4);
    EXPECT_EQ(actual5.size(), 4);
    EXPECT_TRUE(actual5.is_v4());
    EXPECT_FALSE(actual5.is_v6());
    EXPECT_TRUE(actual5_ip.has_value());
    EXPECT_EQ(actual5_ip.value().to_uint(), 0x7F000001);
    
    constexpr auto actual6_ip = actual6.v6();
    EXPECT_EQ(actual6.version(), ip_version::V6);
    EXPECT_EQ(actual6.size(), 16);
    EXPECT_FALSE(actual6.is_v4());
    EXPECT_TRUE(actual6.is_v6());
    EXPECT_TRUE(actual6_ip.has_value());
    EXPECT_EQ(actual6_ip.value().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
#endif

    auto actual7_ip = actual7.v4();
    EXPECT_EQ(actual7.version(), ip_version::V4);
    EXPECT_EQ(actual7.size(), 4);
    EXPECT_TRUE(actual7.is_v4());
    EXPECT_FALSE(actual7.is_v6());
    EXPECT_TRUE(actual7_ip.has_value());
    EXPECT_EQ(actual7_ip.value().to_uint(), 0xFF002A2A);
    
    auto actual8_ip = actual8.v6();
    EXPECT_EQ(actual8.version(), ip_version::V6);
    EXPECT_EQ(actual8.size(), 16);
    EXPECT_FALSE(actual8.is_v4());
    EXPECT_TRUE(actual8.is_v6());
    EXPECT_TRUE(actual8_ip.has_value());
    EXPECT_EQ(actual8_ip.value().to_uint(), uint128_t::from_string("42540766411282592856904266426630537217").value());
}

using InvalidAddressParams = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressParams, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ip_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    ip_address::base_type expected_empty { 0 };
    auto error_ip = ip_address::parse(expected_address);

    EXPECT_EQ(error_ip.bytes(), expected_empty);
#else
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ip_address, InvalidAddressParams,
    testing::Values(
        std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("127", error_code::LEAST_3_PARTS, "least 3 parts in address 127"),
        std::make_tuple("127.0.0.", error_code::EMPTY_OCTET, "empty octet 0 in address 127.0.0."),
        std::make_tuple("127.0.0.1/24", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 127.0.0.1/24 has invalid symbol"),
        std::make_tuple("127.0.0.1271", error_code::OCTET_MORE_3_CHARACTERS, "in octet 0 of address 127.0.0.1271 more 3 characters"),
        std::make_tuple("192.168.0.999", error_code::OCTET_EXCEEDED_255, "octet 0 of address 192.168.0.999 exceeded 255"),
        std::make_tuple("1.2.3.040", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 1.2.3.040"),
        std::make_tuple("FEDC:9878%scope", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878%scope"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple("123456789abcfe::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfe:: more 4 characters"),
        std::make_tuple("::1/24", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address ::1/24 has invalid symbols"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple(":6:5:4:3:2:1::%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),
        std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 8:7:6:5:4:3:2:1::%scope"),
        std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),
        std::make_tuple("7:6:5:4:3:2:1%scope", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        std::make_tuple("::%123456789abcdefgh", error_code::SCOPE_ID_IS_TOO_LONG, "scope id is too long in address ::%123456789abcdefgh"),
        std::make_tuple("::1%scope_id/24", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope_id/24") 
    ));

TEST(ip_address, Comparison) {
    constexpr auto ip1 = ip_address::parse("127.240.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");
    constexpr auto ip3 = ip_address::parse("2001:db8::2");

    ASSERT_TRUE(ip1 < ip2);
    ASSERT_TRUE(ip1 <= ip2);
    ASSERT_FALSE(ip1 > ip2);
    ASSERT_FALSE(ip1 >= ip2);
    ASSERT_FALSE(ip1 == ip2);
    ASSERT_TRUE(ip1 != ip2);
    
    ASSERT_FALSE(ip2 < ip1);
    ASSERT_FALSE(ip2 <= ip1);
    ASSERT_TRUE(ip2 > ip1);
    ASSERT_TRUE(ip2 >= ip1);
    ASSERT_FALSE(ip2 == ip1);
    ASSERT_TRUE(ip2 != ip1);

    ASSERT_FALSE(ip3 < ip2);
    ASSERT_TRUE(ip3 <= ip2);
    ASSERT_FALSE(ip3 > ip2);
    ASSERT_TRUE(ip3 >= ip2);
    ASSERT_TRUE(ip3 == ip2);
    ASSERT_FALSE(ip3 != ip2);
}

TEST(ip_address, to_string) {
    constexpr auto ip1 = ip_address::parse("127.240.0.1");
    constexpr auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::string expected_address = "127.240.0.1";
    const auto expected_full_2 = "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const auto expected_compact_2 = "fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const auto expected_compressed_2 = "fe80::1ff:fe23:4567:890a%eth2";
    const auto expected_compressed_upper_2 = "FE80::1FF:FE23:4567:890A%eth2";

    std::ostringstream ss_full; ss_full << full << ip1;
    std::ostringstream ss_default; ss_default << ip1;
    std::ostringstream ss_compact; ss_compact << compact << ip1;
    std::ostringstream ss_compressed; ss_compressed << compressed << ip1;
    std::ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    std::ostringstream ss_full_2; ss_full_2 << full << ip2;
    std::ostringstream ss_default_2; ss_default_2 << ip2;
    std::ostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    std::ostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    std::ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_string(format::full), expected_address);
    ASSERT_EQ(ip1.to_string(format::compact), expected_address);
    ASSERT_EQ(ip1.to_string(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_string(), expected_address);
    ASSERT_EQ((std::string) ip1, expected_address);
    ASSERT_EQ(std::to_string(ip1), expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_string(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_string(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_string(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_string(), expected_compressed_2);
    ASSERT_EQ((std::string) ip2, expected_compressed_2);
    ASSERT_EQ(std::to_string(ip2), expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}
