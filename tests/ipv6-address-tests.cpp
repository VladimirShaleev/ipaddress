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
        std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),

        std::make_tuple(":", error_code::LEAST_3_PARTS, "least 3 parts in address :"),
        std::make_tuple(":1", error_code::LEAST_3_PARTS, "least 3 parts in address :1"),
        std::make_tuple("FEDC:9878", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878"),
        std::make_tuple(":%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :%scope"),
        std::make_tuple(":1%scope", error_code::LEAST_3_PARTS, "least 3 parts in address :1%scope"),
        std::make_tuple("FEDC:9878%scope", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878%scope"),

        std::make_tuple("10:9:8:7:6:5:4:3:2:1", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42"),
        std::make_tuple("1:2:3:4:5:6:7:42.42.42.42", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 1:2:3:4:5:6:7:42.42.42.42"),
        std::make_tuple("10:9:8:7:6:5:4:3:2:1%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:2:1%scope"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),

        std::make_tuple("3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1"),
        std::make_tuple("1::2::3::4:5", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5"),
        std::make_tuple("2001::db:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1"),
        std::make_tuple("3ffe::1::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::"),
        std::make_tuple("::3ffe::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1"),
        std::make_tuple(":3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1"),
        std::make_tuple("3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:"),
        std::make_tuple(":3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:"),
        std::make_tuple(":::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::"),
        std::make_tuple("2001:db8:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1"),
        std::make_tuple("3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1%scope"),
        std::make_tuple("1::2::3::4:5%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5%scope"),
        std::make_tuple("2001::db:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1%scope"),
        std::make_tuple("3ffe::1::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::%scope"),
        std::make_tuple("::3ffe::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1%scope"),
        std::make_tuple(":3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1%scope"),
        std::make_tuple("3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:%scope"),
        std::make_tuple(":3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:%scope"),
        std::make_tuple(":::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),

        std::make_tuple("::00000", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::00000 more 4 characters"),
        std::make_tuple("3ffe::10000", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address 3ffe::10000 more 4 characters"),
        std::make_tuple("02001:db8::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 02001:db8:: more 4 characters"),
        std::make_tuple("2001:888888::1", error_code::PART_IS_MORE_4_CHARS, "in part 1 of address 2001:888888::1 more 4 characters"),
        std::make_tuple("::00000%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::00000%scope more 4 characters"),
        std::make_tuple("3ffe::10000%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address 3ffe::10000%scope more 4 characters"),
        std::make_tuple("02001:db8::%scope", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 02001:db8::%scope more 4 characters"),
        std::make_tuple("2001:888888::1%scope", error_code::PART_IS_MORE_4_CHARS, "in part 1 of address 2001:888888::1%scope more 4 characters"),
        std::make_tuple("1.2.3.4::%scope", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 1.2.3.4::%scope more 4 characters"),
        std::make_tuple("1.2.3.4::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 1.2.3.4:: more 4 characters"),
        std::make_tuple("::1.2.3.4:%scope", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::1.2.3.4:%scope more 4 characters"),
        std::make_tuple("::1.2.3.4.5.6.7.8.9", error_code::PART_IS_MORE_4_CHARS, "in part 2 of address ::1.2.3.4.5.6.7.8.9 more 4 characters"),
        std::make_tuple("123456789abcfeh::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfeh:: more 4 characters"),
        std::make_tuple("123456789abcfe::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfe:: more 4 characters"),

        std::make_tuple("3ffe::1.1.1.net", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 2 of address 3ffe::1.1.1.net has invalid symbol"),
        std::make_tuple("3ffe::1.1.1.net%scope", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 2 of address 3ffe::1.1.1.net%scope has invalid symbol"),
        std::make_tuple("3ffe::goog", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::goog has invalid symbols"),
        std::make_tuple("3ffe::-0", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-0 has invalid symbols"),
        std::make_tuple("3ffe::+0", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::+0 has invalid symbols"),
        std::make_tuple("3ffe::-1", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-1 has invalid symbols"),
        std::make_tuple("1234:axy::b", error_code::PART_HAS_INVALID_SYMBOL, "in part 4 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("3ffe::goog%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::goog%scope has invalid symbols"),
        std::make_tuple("3ffe::-0%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-0%scope has invalid symbols"),
        std::make_tuple("3ffe::+0%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::+0%scope has invalid symbols"),
        std::make_tuple("3ffe::-1%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address 3ffe::-1%scope has invalid symbols"),
        std::make_tuple("1234:axy::b%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 4 of address 1234:axy::b%scope has invalid symbols"),
        std::make_tuple("::1/24", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address ::1/24 has invalid symbols"),

        std::make_tuple("3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1"),
        std::make_tuple("1::2::3::4:5", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5"),
        std::make_tuple("2001::db:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1"),
        std::make_tuple("3ffe::1::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::"),
        std::make_tuple("::3ffe::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1"),
        std::make_tuple(":3ffe::1::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1"),
        std::make_tuple("3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:"),
        std::make_tuple(":3ffe::1::1:", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:"),
        std::make_tuple(":::", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::"),
        std::make_tuple("2001:db8:::1", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1"),
        std::make_tuple("3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1%scope"),
        std::make_tuple("1::2::3::4:5%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 1::2::3::4:5%scope"),
        std::make_tuple("2001::db:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001::db:::1%scope"),
        std::make_tuple("3ffe::1::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::%scope"),
        std::make_tuple("::3ffe::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address ::3ffe::1%scope"),
        std::make_tuple(":3ffe::1::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1%scope"),
        std::make_tuple("3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 3ffe::1::1:%scope"),
        std::make_tuple(":3ffe::1::1:%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :3ffe::1::1:%scope"),
        std::make_tuple(":::%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address :::%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),

        std::make_tuple(":2001:db8::1", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1"),
        std::make_tuple(":1:2:3:4:5:6:7", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7"),
        std::make_tuple(":1:2:3:4:5:6:", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:"),
        std::make_tuple(":6:5:4:3:2:1::", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::"),
        std::make_tuple(":2001:db8::1%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :2001:db8::1%scope"),
        std::make_tuple(":1:2:3:4:5:6:7%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:7%scope"),
        std::make_tuple(":1:2:3:4:5:6:%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :1:2:3:4:5:6:%scope"),
        std::make_tuple(":6:5:4:3:2:1::%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),

        std::make_tuple("2001:db8::1:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:"),
        std::make_tuple("1:2:3:4:5:6:7:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:"),
        std::make_tuple("::7:6:5:4:3:2:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:"),
        std::make_tuple("2001:db8::1:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 2001:db8::1:%scope"),
        std::make_tuple("1:2:3:4:5:6:7:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:%scope"),
        std::make_tuple("::7:6:5:4:3:2:%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address ::7:6:5:4:3:2:%scope"),
        std::make_tuple("1:2:3:4:5:6:7:8:", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 1:2:3:4:5:6:7:8:"),

        std::make_tuple("9:8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1"),
        std::make_tuple("::8:7:6:5:4:3:2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1"),
        std::make_tuple("8:7:6:5:4:3:2:1::", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3:2:1::"),
        std::make_tuple("8:7:6:5:4:3::2:1", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3::2:1"),
        std::make_tuple("9:8:7:6:5:4:3::2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 9:8:7:6:5:4:3::2:1%scope"),
        std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 8:7:6:5:4:3:2:1::%scope"),
        std::make_tuple("::8:7:6:5:4:3:2:1%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address ::8:7:6:5:4:3:2:1%scope"),
        std::make_tuple("1:2:3:4:5::6:7:8", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8"),
        std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),

        std::make_tuple("7:6:5:4:3:2:1", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1"),
        std::make_tuple("7:6:5:4:3:2:1%scope", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        
        std::make_tuple("::%123456789abcdefgh", error_code::SCOPE_ID_IS_TOO_LONG, "scope id is too long in address ::%123456789abcdefgh"),

        std::make_tuple("::1%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%"),
        std::make_tuple("::1%scope%", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope%"),
        std::make_tuple("::1%scope_id/24", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope_id/24") 
    ));

TEST(ipv6_address, Comparison) {
    auto ip1 = ipv6_address::parse("2001:db8::1");
    auto ip2 = ipv6_address::parse("2001:db8::2");
    auto ip3 = ipv6_address::parse("2001:db8::2");
    auto ip4 = ipv6_address::parse("2001:db8::2%abc");
    auto ip5 = ipv6_address::parse("2001:db8::2%abc");
    auto ip6 = ipv6_address::parse("2001:db8::2%acc");

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
    
    ASSERT_FALSE(ip2 < ip3);
    ASSERT_TRUE(ip2 <= ip3);
    ASSERT_FALSE(ip2 > ip3);
    ASSERT_TRUE(ip2 >= ip3);
    ASSERT_TRUE(ip2 == ip3);
    ASSERT_FALSE(ip2 != ip3);
    
    ASSERT_FALSE(ip3 < ip2);
    ASSERT_TRUE(ip3 <= ip2);
    ASSERT_FALSE(ip3 > ip2);
    ASSERT_TRUE(ip3 >= ip2);
    ASSERT_TRUE(ip3 == ip2);
    ASSERT_FALSE(ip3 != ip2);

    ASSERT_TRUE(ip3 < ip4);
    ASSERT_TRUE(ip3 <= ip4);
    ASSERT_FALSE(ip3 > ip4);
    ASSERT_FALSE(ip3 >= ip4);
    ASSERT_FALSE(ip3 == ip4);
    ASSERT_TRUE(ip3 != ip4);
    
    ASSERT_FALSE(ip4 < ip3);
    ASSERT_FALSE(ip4 <= ip3);
    ASSERT_TRUE(ip4 > ip3);
    ASSERT_TRUE(ip4 >= ip3);
    ASSERT_FALSE(ip4 == ip3);
    ASSERT_TRUE(ip4 != ip3);

    ASSERT_FALSE(ip4 < ip5);
    ASSERT_TRUE(ip4 <= ip5);
    ASSERT_FALSE(ip4 > ip5);
    ASSERT_TRUE(ip4 >= ip5);
    ASSERT_TRUE(ip4 == ip5);
    ASSERT_FALSE(ip4 != ip5);
    
    ASSERT_FALSE(ip5 < ip4);
    ASSERT_TRUE(ip5 <= ip4);
    ASSERT_FALSE(ip5 > ip4);
    ASSERT_TRUE(ip5 >= ip4);
    ASSERT_TRUE(ip5 == ip4);
    ASSERT_FALSE(ip5 != ip4);

    ASSERT_TRUE(ip5 < ip6);
    ASSERT_TRUE(ip5 <= ip6);
    ASSERT_FALSE(ip5 > ip6);
    ASSERT_FALSE(ip5 >= ip6);
    ASSERT_FALSE(ip5 == ip6);
    ASSERT_TRUE(ip5 != ip6);
    
    ASSERT_FALSE(ip6 < ip5);
    ASSERT_FALSE(ip6 <= ip5);
    ASSERT_TRUE(ip6 > ip5);
    ASSERT_TRUE(ip6 >= ip5);
    ASSERT_FALSE(ip6 == ip5);
    ASSERT_TRUE(ip6 != ip5);
}

using ToStringParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*>>;
TEST_P(ToStringParams, to_string) {
    const auto address = std::get<0>(GetParam());
    const auto full = std::get<1>(GetParam());
    const auto compact = std::get<2>(GetParam());
    const auto compressed = std::get<3>(GetParam());

    const auto actual = ipv6_address::parse(address);

    std::ostringstream ss;
    ss << actual;

    ASSERT_EQ(actual.to_string(format::full), std::string(full));
    ASSERT_EQ(actual.to_string(format::compact), std::string(compact));
    ASSERT_EQ(actual.to_string(format::compressed), std::string(compressed));
    ASSERT_EQ(std::to_string(actual), std::string(compressed));
    ASSERT_EQ(ss.str(), std::string(compressed));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_address, ToStringParams,
    testing::Values(
        std::make_tuple("2001:db8:0:0:1:0:0:1", "2001:0db8:0000:0000:0001:0000:0000:0001", "2001:db8:0:0:1:0:0:1", "2001:db8::1:0:0:1"),
        std::make_tuple("2001:DB8::1", "2001:0db8:0000:0000:0000:0000:0000:0001", "2001:db8:0:0:0:0:0:1", "2001:db8::1"),
        std::make_tuple("2001:db8::1", "2001:0db8:0000:0000:0000:0000:0000:0001", "2001:db8:0:0:0:0:0:1", "2001:db8::1"),
        std::make_tuple("2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:0db8:85a3:0000:0000:8a2e:0370:7334", "2001:db8:85a3:0:0:8a2e:370:7334", "2001:db8:85a3::8a2e:370:7334"),
        std::make_tuple("fe80::1ff:fe23:4567:890a", "fe80:0000:0000:0000:01ff:fe23:4567:890a", "fe80:0:0:0:1ff:fe23:4567:890a", "fe80::1ff:fe23:4567:890a"),
        std::make_tuple("::", "0000:0000:0000:0000:0000:0000:0000:0000", "0:0:0:0:0:0:0:0", "::"),
        std::make_tuple("::127.0.0.1", "0000:0000:0000:0000:0000:0000:7f00:0001", "0:0:0:0:0:0:7f00:1", "::7f00:1"),
        std::make_tuple("0000::0000", "0000:0000:0000:0000:0000:0000:0000:0000", "0:0:0:0:0:0:0:0", "::"),
        std::make_tuple("::c0a8:1", "0000:0000:0000:0000:0000:0000:c0a8:0001", "0:0:0:0:0:0:c0a8:1", "::c0a8:1"),
        std::make_tuple("000::c0a8:0001", "0000:0000:0000:0000:0000:0000:c0a8:0001", "0:0:0:0:0:0:c0a8:1", "::c0a8:1"),
        std::make_tuple("::1", "0000:0000:0000:0000:0000:0000:0000:0001", "0:0:0:0:0:0:0:1", "::1"),
        std::make_tuple("::ffff:0:0", "0000:0000:0000:0000:0000:ffff:0000:0000", "0:0:0:0:0:ffff:0:0", "::ffff:0:0"),
        std::make_tuple("::ffff:0:0:0", "0000:0000:0000:0000:ffff:0000:0000:0000", "0:0:0:0:ffff:0:0:0", "::ffff:0:0:0"),
        std::make_tuple("64:ff9b::", "0064:ff9b:0000:0000:0000:0000:0000:0000", "64:ff9b:0:0:0:0:0:0", "64:ff9b::"),
        std::make_tuple("64:ff9b:1::", "0064:ff9b:0001:0000:0000:0000:0000:0000", "64:ff9b:1:0:0:0:0:0", "64:ff9b:1::"),
        std::make_tuple("100::", "0100:0000:0000:0000:0000:0000:0000:0000", "100:0:0:0:0:0:0:0", "100::"),
        std::make_tuple("ff02::1:3", "ff02:0000:0000:0000:0000:0000:0001:0003", "ff02:0:0:0:0:0:1:3", "ff02::1:3"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%eth2", "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2", "fe80:0:0:0:1ff:fe23:4567:890a%eth2", "fe80::1ff:fe23:4567:890a%eth2"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%25eth01234567", "fe80:0000:0000:0000:01ff:fe23:4567:890a%25eth01234567", "fe80:0:0:0:1ff:fe23:4567:890a%25eth01234567", "fe80::1ff:fe23:4567:890a%25eth01234567"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%3", "fe80:0000:0000:0000:01ff:fe23:4567:890a%3", "fe80:0:0:0:1ff:fe23:4567:890a%3", "fe80::1ff:fe23:4567:890a%3"),
        std::make_tuple("fe80::1ff:fe23:4567:890a%31", "fe80:0000:0000:0000:01ff:fe23:4567:890a%31", "fe80:0:0:0:1ff:fe23:4567:890a%31", "fe80::1ff:fe23:4567:890a%31"),
        std::make_tuple("1:2:3:4:5:6:42.42.42.1", "0001:0002:0003:0004:0005:0006:2a2a:2a01", "1:2:3:4:5:6:2a2a:2a01", "1:2:3:4:5:6:2a2a:2a01"),
        std::make_tuple("0:0:0:4:5:6:42.42.42.1%test", "0000:0000:0000:0004:0005:0006:2a2a:2a01%test", "0:0:0:4:5:6:2a2a:2a01%test", "::4:5:6:2a2a:2a01%test")
    ));
