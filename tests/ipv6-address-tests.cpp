#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv6_address, DefaultCtor) {
    ipv6_address::base_type expected_empty {
         0, 0, 0, 0, 
         0, 0, 0, 0,
         0, 0, 0, 0,
         0, 0, 0, 0};

    ipv6_address ip;

    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.size, 16);
    EXPECT_EQ(ip.version, version::V6);
}

TEST(ipv6_address, CopyCtor) {
    ipv6_address::base_type expected_ip { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0};

    auto ip = ipv6_address::parse("::ffff:0:0");
    auto ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy, ipv6_address(expected_ip));
}

TEST(ipv6_address, CopyOperator) {
    ipv6_address::base_type expected_null { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};
    ipv6_address::base_type expected_ip { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0};

    auto ip = ipv6_address::parse("::ffff:0:0");
    ipv6_address ip_copy;

    EXPECT_EQ(ip_copy.bytes(), expected_null);
    ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.bytes(), expected_ip);
}

using FromBytesParams = TestWithParam<std::tuple<ipv6_address::base_type, const char*>>;
TEST_P(FromBytesParams, from_bytes) {
    auto bytes = std::get<0>(GetParam());
    auto scope = std::get<1>(GetParam());

    auto ip1 = ipv6_address::from_bytes(bytes);
    auto ip2 = ipv6_address::from_bytes(bytes.data(), bytes.size());

    EXPECT_EQ(ip1.bytes(), bytes);
    EXPECT_FALSE(ip1.get_scope_id());

    EXPECT_EQ(ip2.bytes(), bytes);
    EXPECT_FALSE(ip2.get_scope_id());

    ip1.set_scope_id(scope);
    ip2.set_scope_id("test");

    EXPECT_EQ(ip1.bytes(), bytes);
    EXPECT_EQ(std::string(ip1.get_scope_id()), scope);

    EXPECT_EQ(ip2.bytes(), bytes);
    EXPECT_EQ(std::string(ip2.get_scope_id()), "test");
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, FromBytesParams,
    testing::Values(
        std::make_tuple(ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, ""),
        std::make_tuple(ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, "eth2")
    ));

using AddressParserParams = TestWithParam<std::tuple<const char*, ipv6_address::base_type, bool, bool, const char*, uint32_t>>;
TEST_P(AddressParserParams, parse) {
    auto excepted_bytes = get<1>(GetParam());
    auto excepted_scope_has_str = get<2>(GetParam());
    auto excepted_scope_has_int = get<3>(GetParam());
    auto excepted_scope_str = get<4>(GetParam());
    auto excepted_scope_int = get<5>(GetParam());

    auto ip = ipv6_address::parse(get<0>(GetParam()));
    auto actual_bytes = ip.bytes();
    auto actual_scope_id = ip.get_scope_id();

    auto actual_scope_has = (bool) actual_scope_id;
    auto actual_scope_has_str = actual_scope_id.has_string();
    auto actual_scope_has_int = actual_scope_id.has_uint32();
    auto actual_scope_str = actual_scope_id.get_string();
    auto actual_scope_int = actual_scope_id.get_uint32();
    std::string actual_scope_str_2 = actual_scope_id;
    uint32_t actual_scope_int_2 = actual_scope_id;

    ASSERT_EQ(actual_bytes, excepted_bytes);
    ASSERT_EQ(excepted_scope_has_str, actual_scope_has);
    ASSERT_EQ(excepted_scope_has_str, actual_scope_has_str);
    ASSERT_EQ(excepted_scope_has_int, actual_scope_has_int);
    ASSERT_EQ(excepted_scope_str, actual_scope_str);
    ASSERT_EQ(excepted_scope_int, actual_scope_int);
    ASSERT_EQ(excepted_scope_str, actual_scope_str_2);
    ASSERT_EQ(excepted_scope_int, actual_scope_int_2);

    std::string s1;
    ipv6_address addr;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> addr >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    ASSERT_EQ(addr, ip);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, AddressParserParams,
    testing::Values(
        std::make_tuple("2001:db8:0:0:1:0:0:1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:DB8::1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:db8::1", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("2001:0db8:85a3:0000:0000:8a2e:0370:7334", ipv6_address::base_type{ 0x20, 0x01, 0x0D, 0xB8, 0x85, 0xa3, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34 }, false, false, "", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, false, false, "", 0),
        std::make_tuple("::", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::127.0.0.1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("0000::0000", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::c0a8:1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("000::c0a8:0001", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("::1", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, false, false, "", 0),
        std::make_tuple("::ffff:0:0", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("::ffff:0:0:0", ipv6_address::base_type{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("64:ff9b::", ipv6_address::base_type{ 0x00, 0x64, 0xff, 0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("64:ff9b:1::", ipv6_address::base_type{ 0x00, 0x64, 0xff, 0x9b, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("100::", ipv6_address::base_type{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, false, false, "", 0),
        std::make_tuple("ff02::1:3", ipv6_address::base_type{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03 }, false, false, "", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%eth2", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, false, "eth2", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%25eth01234567", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, false, "25eth01234567", 0),
        std::make_tuple("fe80::1ff:fe23:4567:890a%3", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, true, "3", 3),
        std::make_tuple("fe80::1ff:fe23:4567:890a%31", ipv6_address::base_type{ 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x23, 0x45, 0x67, 0x89, 0x0a }, true, true, "31", 31),
        std::make_tuple("1:2:3:4:5:6:42.42.42.1", ipv6_address::base_type{ 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x2A, 0x2A, 0x2A, 0x01 }, false, false, "", 0)
    ));













using InvalidAddressParserParams = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressParserParams, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv6_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    ipv4_address::base_type expected_empty { 0 };
    auto error_ip = ipv6_address::parse(expected_address);

    EXPECT_EQ(error_ip.bytes(), expected_empty);
#else
    EXPECT_THAT(
        [address=expected_address]() { ipv6_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, InvalidAddressParserParams,
    testing::Values(
        // std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),

        // std::make_tuple(":", error_code::LEAST_3_PARTS, "least 3 parts in address :"),
        // std::make_tuple(":1", error_code::LEAST_3_PARTS, "least 3 parts in address :1"),
        // std::make_tuple("FEDC:9878", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878"),
        // std::make_tuple(":%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :%scope"),
        // std::make_tuple(":1%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :1%scope"),
        // std::make_tuple("FEDC:9878%scope", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878%scope"),

        // std::make_tuple("1:2:3:4:5:6:7:8:", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 1:2:3:4:5:6:7:8:"),
        // std::make_tuple("10:9:8:7:6:5:4:3:2:1", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1"),
        // std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42"),
        // std::make_tuple("1:2:3:4:5:6:7:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 1:2:3:4:5:6:7:42.42.42.42"),
        // std::make_tuple("10:9:8:7:6:5:4:3:2:1%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1%scope"),
        // std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),

        // std::make_tuple("3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1"),
        // std::make_tuple("1::2::3::4:5", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5"),
        // std::make_tuple("2001::db:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1"),
        // std::make_tuple("3ffe::1::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::"),
        // std::make_tuple("::3ffe::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1"),
        // std::make_tuple(":3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1"),
        // std::make_tuple("3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:"),
        // std::make_tuple(":3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:"),
        // std::make_tuple(":::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::"),
        // std::make_tuple("2001:db8:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1"),
        // std::make_tuple("3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1%scope"),
        // std::make_tuple("1::2::3::4:5%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5%scope"),
        // std::make_tuple("2001::db:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1%scope"),
        // std::make_tuple("3ffe::1::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::%scope"),
        // std::make_tuple("::3ffe::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1%scope"),
        // std::make_tuple(":3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1%scope"),
        // std::make_tuple("3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:%scope"),
        // std::make_tuple(":3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:%scope"),
        // std::make_tuple(":::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::%scope"),
        // std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),

        // std::make_tuple(":2001:db8::1", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1"),
        // std::make_tuple(":1:2:3:4:5:6:7", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7"),
        // std::make_tuple(":1:2:3:4:5:6:", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:"),
        // std::make_tuple(":6:5:4:3:2:1::", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::"),
        // std::make_tuple(":2001:db8::1%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1%scope"),
        // std::make_tuple(":1:2:3:4:5:6:7%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7%scope"),
        // std::make_tuple(":1:2:3:4:5:6:%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:%scope"),
        // std::make_tuple(":6:5:4:3:2:1::%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),

        // std::make_tuple("2001:db8::1:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:"),
        // std::make_tuple("1:2:3:4:5:6:7:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:"),
        std::make_tuple("::1.2.3.4:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::1.2.3.4:"),
        //std::make_tuple("::7:6:5:4:3:2:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:"),
        //std::make_tuple("2001:db8::1:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:%scope"),
        //std::make_tuple("1:2:3:4:5:6:7:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:%scope"),
        std::make_tuple("::1.2.3.4:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::1.2.3.4:%scope"),
        //std::make_tuple("::7:6:5:4:3:2:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:%scope"),

        // std::make_tuple("9:8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1"),
        // std::make_tuple("::8:7:6:5:4:3:2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1"),
        // std::make_tuple("8:7:6:5:4:3:2:1::", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3:2:1::"),
        // std::make_tuple("8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3::2:1"),
        // std::make_tuple("9:8:7:6:5:4:3::2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1%scope"),
        // std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3:2:1::%scope"),
        // std::make_tuple("::8:7:6:5:4:3:2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1%scope"),
        // std::make_tuple("1:2:3:4:5::6:7:8", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8"),
        // std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),

        // std::make_tuple("7:6:5:4:3:2:1", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1"),
        // std::make_tuple("7:6:5:4:3:2:1%scope", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        
        // assertBadSplit("::1/24")
        // assertBadSplit("::1%scope_id/24")
/*



    PART_IS_MORE_4_CHARS,
    PART_HAS_INVALID_SYMBOL,
    MOST_ONE_DOUBLE_COLON_PERMITTED,
    LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON,
    TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON,
    ,
    ,

*/
        std::make_tuple("::1%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%"),
        std::make_tuple("::%123456789abcdefgh", error_code::SCOPE_ID_IS_TOO_LONG, "scope id is too long in address ::%123456789abcdefgh"),
        std::make_tuple("::1%scope%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope%")
    ));
