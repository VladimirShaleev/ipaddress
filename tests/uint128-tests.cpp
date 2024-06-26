#include <gtest/gtest.h>

#ifdef IPADDRESS_TEST_MODULE
import ipaddress;
#else
#include <ipaddress/ipaddress.hpp>
#endif

using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 14

TEST(uint128_t, CompileTime) {
    constexpr uint128_t value1 = { 1, 0 };
    constexpr uint128_t value2 = value1 << 2;
    constexpr uint128_t value3 = value2 >> 1;
    constexpr uint128_t value4 = value3 * 4;
    constexpr uint128_t value5 = value4 / 3;
    ASSERT_EQ(value5.upper(), 2);
    ASSERT_EQ(value5.lower(), 0xAAAAAAAAAAAAAAAA);
}

#endif

TEST(uint128_t, Ctors) {
    IPADDRESS_CONSTEXPR uint128_t value1;
    IPADDRESS_CONSTEXPR uint128_t value2{};
    IPADDRESS_CONSTEXPR uint128_t value3 = 1;
    IPADDRESS_CONSTEXPR uint128_t value4(1, 2);
    IPADDRESS_CONSTEXPR uint128_t value5 { 1, 2 };
    IPADDRESS_CONSTEXPR uint128_t value6 = { 1, 2 };
    IPADDRESS_CONSTEXPR uint128_t value7 { 1 };
    IPADDRESS_CONSTEXPR uint128_t value8 = { 1 };
    auto value9 = (uint128_t) 457.3f;
    auto value10 = (uint128_t) 4.32e+20;

    ASSERT_EQ(value1.upper(), 0);
    ASSERT_EQ(value1.lower(), 0);
    ASSERT_EQ(value2.upper(), 0);
    ASSERT_EQ(value2.lower(), 0);
    ASSERT_EQ(value3.upper(), 0);
    ASSERT_EQ(value3.lower(), 1);
    ASSERT_EQ(value4.upper(), 1);
    ASSERT_EQ(value4.lower(), 2);
    ASSERT_EQ(value5.upper(), 1);
    ASSERT_EQ(value5.lower(), 2);
    ASSERT_EQ(value6.upper(), 1);
    ASSERT_EQ(value6.lower(), 2);
    ASSERT_EQ(value7.upper(), 0);
    ASSERT_EQ(value7.lower(), 1);
    ASSERT_EQ(value8.upper(), 0);
    ASSERT_EQ(value8.lower(), 1);
    ASSERT_EQ(value9.upper(), 0);
    ASSERT_EQ(value9.lower(), 457);
    ASSERT_EQ(value10.upper(), 23);
    ASSERT_EQ(value10.lower(), 0x6B344F2A78C00000);
}

TEST(uint128_t, Copy) {
    IPADDRESS_CONSTEXPR uint128_t value = 5;

    IPADDRESS_CONSTEXPR uint128_t copy_ctor = value;
    uint128_t copy_operator;
    copy_operator = value;

    ASSERT_EQ(copy_ctor.upper(), 0);
    ASSERT_EQ(copy_ctor.lower(), 5);
    ASSERT_EQ(copy_operator.upper(), 0);
    ASSERT_EQ(copy_operator.lower(), 5);
}

TEST(uint128_t, Move) {
    uint128_t value = 5;

    uint128_t move_ctor = std::move(value); // NOLINT(performance-move-const-arg)
    uint128_t move_operator;
    move_operator = std::move(move_ctor); // NOLINT(performance-move-const-arg)

    ASSERT_EQ(move_operator.upper(), 0);
    ASSERT_EQ(move_operator.lower(), 5);
}

TEST(uint128_t, OperatorBool) {
    IPADDRESS_CONSTEXPR uint128_t value1 = 0;
    IPADDRESS_CONSTEXPR uint128_t value2 = 5;
    
    IPADDRESS_CONSTEXPR auto actual1 = (bool) value1;
    IPADDRESS_CONSTEXPR auto actual2 = (bool) value2;

    ASSERT_FALSE(actual1);
    ASSERT_TRUE(actual2);
}

TEST(uint128_t, OperatorT) {
    IPADDRESS_CONSTEXPR uint128_t value { 4, 140185576636287 };
    
    IPADDRESS_CONSTEXPR auto actual1 = (char) value;
    IPADDRESS_CONSTEXPR auto actual2 = (signed char) value;
    IPADDRESS_CONSTEXPR auto actual3 = (unsigned char) value;
    IPADDRESS_CONSTEXPR auto actual4 = (short) value;
    IPADDRESS_CONSTEXPR auto actual5 = (unsigned short) value;
    IPADDRESS_CONSTEXPR auto actual6 = (int) value;
    IPADDRESS_CONSTEXPR auto actual7 = (unsigned int) value;
    IPADDRESS_CONSTEXPR auto actual8 = (long) value;
    IPADDRESS_CONSTEXPR auto actual9 = (unsigned long) value;
    IPADDRESS_CONSTEXPR auto actual10 = (long long) value;
    IPADDRESS_CONSTEXPR auto actual11 = (unsigned long long) value;
    auto actual12 = (float) value;
    auto actual13 = (double) value;
    auto actual14 = (long double) value;

    ASSERT_EQ(actual1, (char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual2, (signed char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual3, (unsigned char)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual4, (short)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual5, (unsigned short)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual6, (int)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual7, (unsigned int)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual8, (long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual9, (unsigned long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual10, (long long)(0x7F7F7F7F7F7F));
    ASSERT_EQ(actual11, (unsigned long long)(0x7F7F7F7F7F7F));

    std::ostringstream str12; str12 << std::setprecision(4) << (actual12 * 1.0e-19);
    std::ostringstream str13; str13 << std::setprecision(4) << (actual13 * 1.0e-19);
    std::ostringstream str14; str14 << std::setprecision(4) << (actual14 * 1.0e-19);
    ASSERT_EQ(str12.str(), "7.379");
    ASSERT_EQ(str13.str(), "7.379");
    ASSERT_EQ(str14.str(), "7.379");
}

TEST(uint128_t, Arithmetic) {
    IPADDRESS_CONSTEXPR uint128_t value1 { 4, 5 };
    IPADDRESS_CONSTEXPR uint128_t value2 { 4, 0xFFFFFFFFFFFFFFFFULL - 1 };
    IPADDRESS_CONSTEXPR uint128_t value3 { 4, 0xFFFFFFFFFFFFFFFFULL };

    IPADDRESS_CONSTEXPR auto plus1 = +value1;
    IPADDRESS_CONSTEXPR auto plus2 = +value2;
    IPADDRESS_CONSTEXPR auto plus3 = +value3;
    ASSERT_EQ(plus1.upper(), 4);
    ASSERT_EQ(plus1.lower(), 5);
    ASSERT_EQ(plus2.upper(), 4);
    ASSERT_EQ(plus2.lower(), 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(plus3.upper(), 4);
    ASSERT_EQ(plus3.lower(), 0xFFFFFFFFFFFFFFFFULL);

    IPADDRESS_CONSTEXPR auto minus1 = -value1;
    IPADDRESS_CONSTEXPR auto minus2 = -value2;
    IPADDRESS_CONSTEXPR auto minus3 = -value3;
    ASSERT_EQ(minus1.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus1.lower(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.lower(), 2);
    ASSERT_EQ(minus3.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus3.lower(), 1);
    
    IPADDRESS_CONSTEXPR auto inv1 = ~value1;
    IPADDRESS_CONSTEXPR auto inv2 = ~value2;
    IPADDRESS_CONSTEXPR auto inv3 = ~value3;
    ASSERT_EQ(inv1.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv1.lower(), 0xFFFFFFFFFFFFFFFFULL - 5);
    ASSERT_EQ(inv2.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv2.lower(), 1);
    ASSERT_EQ(inv3.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv3.lower(), 0);
    
    IPADDRESS_CONSTEXPR auto sum1 = value1 + 1;
    IPADDRESS_CONSTEXPR auto sum2 = value2 + 1;
    IPADDRESS_CONSTEXPR auto sum3 = value3 + 1;
    IPADDRESS_CONSTEXPR auto sum4 = value1 + uint128_t(10, 0xFFFFFFFFFFFFFFFFULL - 2);
    IPADDRESS_CONSTEXPR auto sum5 = 1 + value1;
    ASSERT_EQ(sum1.upper(), 4);
    ASSERT_EQ(sum1.lower(), 6);
    ASSERT_EQ(sum2.upper(), 4);
    ASSERT_EQ(sum2.lower(), 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sum3.upper(), 5);
    ASSERT_EQ(sum3.lower(), 0);
    ASSERT_EQ(sum4.upper(), 15);
    ASSERT_EQ(sum4.lower(), 2);
    ASSERT_EQ(sum5.upper(), 4);
    ASSERT_EQ(sum5.lower(), 6);
    
    IPADDRESS_CONSTEXPR auto sub1 = value1 - 6;
    IPADDRESS_CONSTEXPR auto sub2 = value2 - 7;
    IPADDRESS_CONSTEXPR auto sub3 = value3 - 1;
    IPADDRESS_CONSTEXPR auto sub4 = value1 - uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    IPADDRESS_CONSTEXPR auto sub5 = 10 - uint128_t(7);
    ASSERT_EQ(sub1.upper(), 3);
    ASSERT_EQ(sub1.lower(), 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sub2.upper(), 4);
    ASSERT_EQ(sub2.lower(), 0xFFFFFFFFFFFFFFFFULL - 8);
    ASSERT_EQ(sub3.upper(), 4);
    ASSERT_EQ(sub3.lower(), 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(sub4.upper(), 1);
    ASSERT_EQ(sub4.lower(), 8);
    ASSERT_EQ(sub5.upper(), 0);
    ASSERT_EQ(sub5.lower(), 3);
    
    IPADDRESS_CONSTEXPR auto mul1 = value1 * 6;
    IPADDRESS_CONSTEXPR auto mul2 = value2 * 7;
    IPADDRESS_CONSTEXPR auto mul3 = value3 * 1;
    IPADDRESS_CONSTEXPR auto mul4 = 1000 * uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    ASSERT_EQ(mul1.upper(), 24);
    ASSERT_EQ(mul1.lower(), 30);
    ASSERT_EQ(mul2.upper(), 34);
    ASSERT_EQ(mul2.lower(), 0XFFFFFFFFFFFFFFF2ULL);
    ASSERT_EQ(mul3.upper(), 4);
    ASSERT_EQ(mul3.lower(), 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(mul4.upper(), 0xBB7);
    ASSERT_EQ(mul4.lower(), 0xFFFFFFFFFFFFF448);
    
    auto div1 = value1 / 25;
    auto div2 = value2 / 2;
    auto div3 = value3 / 1;
    auto div4 = 1000 / uint128_t(0, 2);
    auto div5 = 0 / uint128_t(0, 2);
    auto div6 = 0 / uint128_t(0, 0);
    auto div7 = 1000 / uint128_t(0, 1000);
    auto div8 = 1000 / uint128_t(0, 2000);
    auto div9 = uint128_t(2, 1000) / uint128_t(2, 1000);
    auto div10 = uint128_t(2, 1000) / uint128_t(2, 2000);
    ASSERT_EQ(div1.upper(), 0);
    ASSERT_EQ(div1.lower(), 0x28F5C28F5C28F5C2);
    ASSERT_EQ(div2.upper(), 2);
    ASSERT_EQ(div2.lower(), 0x7fffffffffffffffULL);
    ASSERT_EQ(div3.upper(), 4);
    ASSERT_EQ(div3.lower(), 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(div4.upper(), 0);
    ASSERT_EQ(div4.lower(), 0x1F4);
    ASSERT_EQ(div5.upper(), 0);
    ASSERT_EQ(div5.lower(), 0);
    ASSERT_EQ(div6.upper(), 0);
    ASSERT_EQ(div6.lower(), 0);
    ASSERT_EQ(div7.upper(), 0);
    ASSERT_EQ(div7.lower(), 1);
    ASSERT_EQ(div8.upper(), 0);
    ASSERT_EQ(div8.lower(), 0);
    ASSERT_EQ(div9.upper(), 0);
    ASSERT_EQ(div9.lower(), 1);
    ASSERT_EQ(div10.upper(), 0);
    ASSERT_EQ(div10.lower(), 0);

    auto rem1 = value1 % 25;
    auto rem2 = value2 % 2;
    auto rem3 = value3 % 1;
    auto rem4 = 15 % value1;
    ASSERT_EQ(rem1.upper(), 0);
    ASSERT_EQ(rem1.lower(), 19);
    ASSERT_EQ(rem2.upper(), 0);
    ASSERT_EQ(rem2.lower(), 0);
    ASSERT_EQ(rem3.upper(), 0);
    ASSERT_EQ(rem3.lower(), 0);
    ASSERT_EQ(rem4.upper(), 0);
    ASSERT_EQ(rem4.lower(), 15);

    auto and1 = uint128_t(1, 1) & uint128_t(3, 3);
    auto and2 = uint128_t(1, 1) & 1;
    auto and3 = 1 & uint128_t(1, 1);
    ASSERT_EQ(and1.upper(), 1);
    ASSERT_EQ(and1.lower(), 1);
    ASSERT_EQ(and2.upper(), 0);
    ASSERT_EQ(and2.lower(), 1);
    ASSERT_EQ(and3.upper(), 0);
    ASSERT_EQ(and3.lower(), 1);

    auto or1 = uint128_t(1, 1) | uint128_t(2, 2);
    auto or2 = uint128_t(1, 1) | 2;
    auto or3 = 2 | uint128_t(1, 1);
    ASSERT_EQ(or1.upper(), 3);
    ASSERT_EQ(or1.lower(), 3);
    ASSERT_EQ(or2.upper(), 1);
    ASSERT_EQ(or2.lower(), 3);
    ASSERT_EQ(or3.upper(), 1);
    ASSERT_EQ(or3.lower(), 3);

    auto xor1 = uint128_t(1, 1) ^ uint128_t(3, 3);
    auto xor2 = uint128_t(1, 1) ^ 3;
    auto xor3 = 3 ^ uint128_t(1, 1);
    ASSERT_EQ(xor1.upper(), 2);
    ASSERT_EQ(xor1.lower(), 2);
    ASSERT_EQ(xor2.upper(), 1);
    ASSERT_EQ(xor2.lower(), 2);
    ASSERT_EQ(xor3.upper(), 1);
    ASSERT_EQ(xor3.lower(), 2);
    
    auto lshift1 = uint128_t(0, 0xFFFFFFFFFFFFFFFFULL) << 1;
    ASSERT_EQ(lshift1.upper(), 1);
    ASSERT_EQ(lshift1.lower(), 0xFFFFFFFFFFFFFFFEULL);
}

TEST(uint128_t, Assignment) {
    uint128_t value { 4, 5 };

    value += 3;
    ASSERT_EQ(value.upper(), 4);
    ASSERT_EQ(value.lower(), 8);

    value -= 2;
    ASSERT_EQ(value.upper(), 4);
    ASSERT_EQ(value.lower(), 6);

    value *= 2;
    ASSERT_EQ(value.upper(), 8);
    ASSERT_EQ(value.lower(), 12);

    value /= 2;
    ASSERT_EQ(value.upper(), 4);
    ASSERT_EQ(value.lower(), 6);

    value %= 3;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 1);

    value &= 0;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 0);

    value |= 1;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 1);

    value ^= 3;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 2);

    value <<= 1;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 4);
    
    value >>= 1;
    ASSERT_EQ(value.upper(), 0);
    ASSERT_EQ(value.lower(), 2);
}

TEST(uint128_t, IncDec) {
    uint128_t value1 { 4, 5 };
    uint128_t value2 { 4, 0xFFFFFFFFFFFFFFFFULL - 1 };
    uint128_t value3 { 4, 0xFFFFFFFFFFFFFFFFULL };

    ASSERT_EQ(++value1, uint128_t(4, 6));
    ASSERT_EQ(++value2, uint128_t(4, 0xFFFFFFFFFFFFFFFFULL));
    ASSERT_EQ(++value3, uint128_t(5, 0));
    ASSERT_EQ(value1++, uint128_t(4, 6));
    ASSERT_EQ(value2++, uint128_t(4, 0xFFFFFFFFFFFFFFFFULL));
    ASSERT_EQ(value3++, uint128_t(5, 0));
    ASSERT_EQ(value1, uint128_t(4, 7));
    ASSERT_EQ(value2, uint128_t(5, 0));
    ASSERT_EQ(value3, uint128_t(5, 1));
    
    ASSERT_EQ(value1--, uint128_t(4, 7));
    ASSERT_EQ(value2--, uint128_t(5, 0));
    ASSERT_EQ(value3--, uint128_t(5, 1));
    ASSERT_EQ(value1, uint128_t(4, 6));
    ASSERT_EQ(value2, uint128_t(4, 0xFFFFFFFFFFFFFFFFULL));
    ASSERT_EQ(value3, uint128_t(5, 0));
    ASSERT_EQ(--value1, uint128_t(4, 5));
    ASSERT_EQ(--value2, uint128_t(4, 0xFFFFFFFFFFFFFFFFULL - 1));
    ASSERT_EQ(--value3, uint128_t(4, 0xFFFFFFFFFFFFFFFFULL));
}

TEST(uint128_t, Logical) {
    const uint128_t value1 { 0, 0 };
    const uint128_t value2 { 0, 1 };
    const uint128_t value3 { 1, 0 };
    const uint128_t value4 { 1, 1 };

    ASSERT_FALSE((bool) value1);
    ASSERT_TRUE((bool) value2);
    ASSERT_TRUE((bool) value3);
    ASSERT_TRUE((bool) value4);
    
    ASSERT_TRUE(!value1);
    ASSERT_FALSE(!value2);
    ASSERT_FALSE(!value3);
    ASSERT_FALSE(!value4);
    
    ASSERT_FALSE(value1 && value1);
    ASSERT_FALSE(value1 && value2);
    ASSERT_TRUE(value2 && value3);
    ASSERT_TRUE(value3 && value4);
    ASSERT_FALSE(value4 && value1);
    ASSERT_TRUE(value4 && value4);
    
    ASSERT_FALSE(value1 || value1);
    ASSERT_TRUE(value1 || value2);
    ASSERT_TRUE(value2 || value3);
    ASSERT_TRUE(value3 || value4);
    ASSERT_TRUE(value4 || value1);
    ASSERT_TRUE(value4 || value4);
}

TEST(uint128_t, Comparison) {
    const uint128_t value1 { 0, 0 };
    const uint128_t value2 { 0, 1 };
    const uint128_t value3 { 1, 0 };
    const uint128_t value4 { 1, 1 };

    ASSERT_FALSE(value1 == value2);
    ASSERT_TRUE(value1 != value2);
    ASSERT_TRUE(value1 < value2);
    ASSERT_FALSE(value1 > value2);
    ASSERT_TRUE(value1 <= value2);
    ASSERT_FALSE(value1 >= value2);

    ASSERT_FALSE(value1 == value3);
    ASSERT_TRUE(value1 != value3);
    ASSERT_TRUE(value1 < value3);
    ASSERT_FALSE(value1 > value3);
    ASSERT_TRUE(value1 <= value3);
    ASSERT_FALSE(value1 >= value3);

    ASSERT_TRUE(value2 == 1);
    ASSERT_FALSE(value2 != 1);
    ASSERT_FALSE(value2 < 1);
    ASSERT_FALSE(value2 > 1);
    ASSERT_TRUE(value2 <= 1);
    ASSERT_TRUE(value2 >= 1);

    ASSERT_TRUE(1 == value2);
    ASSERT_FALSE(1 != value2);
    ASSERT_FALSE(1 < value2);
    ASSERT_FALSE(1 > value2);
    ASSERT_TRUE(1 <= value2);
    ASSERT_TRUE(1 >= value2);

    ASSERT_TRUE(value3 == value3);
    ASSERT_FALSE(value3 != value3);
    ASSERT_FALSE(value3 < value3);
    ASSERT_FALSE(value3 > value3);
    ASSERT_TRUE(value3 <= value3);
    ASSERT_TRUE(value3 >= value3);

    ASSERT_FALSE(value4 == value2);
    ASSERT_TRUE(value4 != value2);
    ASSERT_FALSE(value4 < value2);
    ASSERT_TRUE(value4 > value2);
    ASSERT_FALSE(value4 <= value2);
    ASSERT_TRUE(value4 >= value2);
}

TEST(uint128_t, Hash) {
    const std::hash<uint128_t> hasher;

    const auto hash1 = hasher(uint128_t(0, 0));
    const auto hash2 = hasher(uint128_t(4, 5));

    ASSERT_EQ(hash1, sizeof(size_t) == 8 ? 0xA55DB391E20904C2ULL : 0xE20904C2UL);
    ASSERT_EQ(hash2, sizeof(size_t) == 8 ? 0xFD9B4C34031415A2ULL : 0xBDD51BB8UL);
}

TEST(uint128_t, Swap) {
    uint128_t value1 = 0;
    uint128_t value2 = { 4, 5 };
    
    std::swap(value1, value2);

    ASSERT_EQ(value1.upper(), 4);
    ASSERT_EQ(value1.lower(), 5);
    ASSERT_EQ(value2.upper(), 0);
    ASSERT_EQ(value2.lower(), 0);
}

TEST(uint128_t, ToString) {
    uint128_t value1 = 17852;
    uint128_t value2 = { 4, 17852 };

    std::ostringstream ss1; ss1 << value1;
    std::ostringstream ss2; ss2 << std::dec << value1;
    std::ostringstream ss3; ss3 << std::oct << value1;
    std::ostringstream ss4; ss4 << std::hex << value1;
    std::ostringstream ss5; ss5 << std::hex << std::uppercase << value1;
    std::ostringstream ss6; ss6 << value2;
    std::ostringstream ss7; ss7 << std::dec << value2;
    std::ostringstream ss8; ss8 << std::oct << value2;
    std::ostringstream ss9; ss9 << std::hex << value2;
    std::ostringstream ss10; ss10 << std::hex << std::uppercase << value2;
    
    ASSERT_EQ(ss1.str(), std::string("17852"));
    ASSERT_EQ(ss2.str(), std::string("17852"));
    ASSERT_EQ(ss3.str(), std::string("42674"));
    ASSERT_EQ(ss4.str(), std::string("45bc"));
    ASSERT_EQ(ss5.str(), std::string("45BC"));
    ASSERT_EQ(ss6.str(), std::string("73786976294838224316"));
    ASSERT_EQ(ss7.str(), std::string("73786976294838224316"));
    ASSERT_EQ(ss8.str(), std::string("10000000000000000042674"));
    ASSERT_EQ(ss9.str(), std::string("400000000000045bc"));
    ASSERT_EQ(ss10.str(), std::string("400000000000045BC"));
}

TEST(uint128_t, ToStringUnicode) {
    IPADDRESS_CONSTEXPR auto value = uint128_t::from_string("10000000000000000042674").value();
    ASSERT_EQ(std::to_string(value), "10000000000000000042674");
    ASSERT_EQ(std::to_wstring(value), L"10000000000000000042674");

    std::ostringstream ss1; ss1 << value;
    std::wstringstream ss2; ss2 << value;
    ASSERT_EQ(ss1.str(), "10000000000000000042674");
    ASSERT_EQ(ss2.str(), L"10000000000000000042674");
    ASSERT_EQ(value.to_string(), "10000000000000000042674");
    ASSERT_EQ(value.to_wstring(), L"10000000000000000042674");
    ASSERT_EQ(value.to_u16string(), u"10000000000000000042674");
    ASSERT_EQ(value.to_u32string(), U"10000000000000000042674");
#if __cpp_char8_t >= 201811L
    ASSERT_EQ(value.to_u8string(), u8"10000000000000000042674");
#endif
}

TEST(uint128_t, FromString) {
    const uint128_t expected1 = 17852;
    const uint128_t expected2 = { 4, 17852 };

    // NOLINTNEXTLINE(readability-isolate-declaration)
    uint128_t read1, read2, read3, read4, read5, read6, read7, read8, read9, read10, read11;
    std::istringstream ss1("17852 test"); ss1 >> read1;
    std::istringstream ss2("17852 test"); ss2 >> std::dec >> read2;
    std::istringstream ss3("42674 test"); ss3 >> std::oct >> read3;
    std::istringstream ss4("45bc test"); ss4 >> std::hex >> read4;
    std::istringstream ss5("45BC test"); ss5 >> std::hex >> read5;
    std::istringstream ss6("73786976294838224316 test"); ss6 >> read6;
    std::istringstream ss7("73786976294838224316 test"); ss7 >> std::dec >> read7;
    std::istringstream ss8("10000000000000000042674 test"); ss8 >> std::oct >> read8;
    std::istringstream ss9("400000000000045bc test"); ss9 >> std::hex >> read9;
    std::istringstream ss10("400000000000045BC test"); ss10 >> std::hex >> read10;
    std::istringstream ss11("bad"); ss11 >> read11;

    ASSERT_EQ(read1, expected1);
    ASSERT_EQ(read2, expected1);
    ASSERT_EQ(read3, expected1);
    ASSERT_EQ(read4, expected1);
    ASSERT_EQ(read5, expected1);
    ASSERT_EQ(read6, expected2);
    ASSERT_EQ(read7, expected2);
    ASSERT_EQ(read8, expected2);
    ASSERT_EQ(read9, expected2);
    ASSERT_EQ(read10, expected2);
    ASSERT_TRUE(ss11.fail());
    ASSERT_EQ(read11, uint128_t(0));
}

TEST(uint128_t, FromStringWideChar) {
    IPADDRESS_CONSTEXPR auto value1 = uint128_t::from_string(L"10000000000000000042674");
    IPADDRESS_CONSTEXPR auto has_value1 = value1.has_value();
    IPADDRESS_CONSTEXPR auto lower_value1 = value1.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value1 = value1.value().upper();
    ASSERT_TRUE(has_value1);
    ASSERT_EQ(lower_value1, 1864712049423066802ULL);
    ASSERT_EQ(upper_value1, 542ULL);

    IPADDRESS_CONSTEXPR auto value2 = uint128_t::from_string(L"100000000000a00000042674");
    IPADDRESS_CONSTEXPR auto has_value2 = value2.has_value();
    IPADDRESS_CONSTEXPR auto lower_value2 = value2.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value2 = value2.value().upper();
    ASSERT_FALSE(has_value2);
    ASSERT_EQ(lower_value2, 0);
    ASSERT_EQ(upper_value2, 0);

    const auto str = L"10000000000000000042674";
    const auto value3 = uint128_t::from_string(str);
    ASSERT_TRUE(value3.has_value());
    ASSERT_EQ(value3.value().lower(), 1864712049423066802ULL);
    ASSERT_EQ(value3.value().upper(), 542ULL);

    uint128_t value4;
    std::wistringstream ss1(L"10000000000000000042674");
    ss1 >> value4;
    ASSERT_FALSE(ss1.fail());
    ASSERT_EQ(value4.lower(), 1864712049423066802ULL);
    ASSERT_EQ(value4.upper(), 542ULL);
    
    uint128_t value5;
    std::wistringstream ss2(L"1000c0000000000000042674");
    ss2 >> value5;
    ASSERT_TRUE(ss2.fail());
    ASSERT_EQ(value5.lower(), 0);
    ASSERT_EQ(value5.upper(), 0);
}

TEST(uint128_t, FromStringUtf16) {
    IPADDRESS_CONSTEXPR auto value1 = uint128_t::from_string(u"10000000000000000042674");
    IPADDRESS_CONSTEXPR auto has_value1 = value1.has_value();
    IPADDRESS_CONSTEXPR auto lower_value1 = value1.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value1 = value1.value().upper();
    ASSERT_TRUE(has_value1);
    ASSERT_EQ(lower_value1, 1864712049423066802ULL);
    ASSERT_EQ(upper_value1, 542ULL);

    IPADDRESS_CONSTEXPR auto value2 = uint128_t::from_string(u"100000000000a00000042674");
    IPADDRESS_CONSTEXPR auto has_value2 = value2.has_value();
    IPADDRESS_CONSTEXPR auto lower_value2 = value2.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value2 = value2.value().upper();
    ASSERT_FALSE(has_value2);
    ASSERT_EQ(lower_value2, 0);
    ASSERT_EQ(upper_value2, 0);

    const auto str = u"10000000000000000042674";
    const auto value3 = uint128_t::from_string(str);
    ASSERT_TRUE(value3.has_value());
    ASSERT_EQ(value3.value().lower(), 1864712049423066802ULL);
    ASSERT_EQ(value3.value().upper(), 542ULL);
}

TEST(uint128_t, FromStringUtf32) {
    IPADDRESS_CONSTEXPR auto value1 = uint128_t::from_string(U"10000000000000000042674");
    IPADDRESS_CONSTEXPR auto has_value1 = value1.has_value();
    IPADDRESS_CONSTEXPR auto lower_value1 = value1.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value1 = value1.value().upper();
    ASSERT_TRUE(has_value1);
    ASSERT_EQ(lower_value1, 1864712049423066802ULL);
    ASSERT_EQ(upper_value1, 542ULL);

    IPADDRESS_CONSTEXPR auto value2 = uint128_t::from_string(U"100000000000a00000042674");
    IPADDRESS_CONSTEXPR auto has_value2 = value2.has_value();
    IPADDRESS_CONSTEXPR auto lower_value2 = value2.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value2 = value2.value().upper();
    ASSERT_FALSE(has_value2);
    ASSERT_EQ(lower_value2, 0);
    ASSERT_EQ(upper_value2, 0);

    const auto str = U"10000000000000000042674";
    const auto value3 = uint128_t::from_string(str);
    ASSERT_TRUE(value3.has_value());
    ASSERT_EQ(value3.value().lower(), 1864712049423066802ULL);
    ASSERT_EQ(value3.value().upper(), 542ULL);
}

#if __cpp_char8_t >= 201811L
TEST(uint128_t, FromStringUtf8) {
    IPADDRESS_CONSTEXPR auto value1 = uint128_t::from_string(u8"10000000000000000042674");
    IPADDRESS_CONSTEXPR auto has_value1 = value1.has_value();
    IPADDRESS_CONSTEXPR auto lower_value1 = value1.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value1 = value1.value().upper();
    ASSERT_TRUE(has_value1);
    ASSERT_EQ(lower_value1, 1864712049423066802ULL);
    ASSERT_EQ(upper_value1, 542ULL);

    IPADDRESS_CONSTEXPR auto value2 = uint128_t::from_string(u8"100000000000a00000042674");
    IPADDRESS_CONSTEXPR auto has_value2 = value2.has_value();
    IPADDRESS_CONSTEXPR auto lower_value2 = value2.value().lower();
    IPADDRESS_CONSTEXPR auto upper_value2 = value2.value().upper();
    ASSERT_FALSE(has_value2);
    ASSERT_EQ(lower_value2, 0);
    ASSERT_EQ(upper_value2, 0);

    const auto str = u8"10000000000000000042674";
    const auto value3 = uint128_t::from_string(str);
    ASSERT_TRUE(value3.has_value());
    ASSERT_EQ(value3.value().lower(), 1864712049423066802ULL);
    ASSERT_EQ(value3.value().upper(), 542ULL);
}
#endif

TEST(uint128_t, NumericLimits) {
    ASSERT_TRUE(std::numeric_limits<uint128_t>::is_integer);
    ASSERT_EQ(std::numeric_limits<uint128_t>::digits, 128);
    ASSERT_EQ(std::numeric_limits<uint128_t>::lowest(), uint128_t());
    ASSERT_EQ(std::numeric_limits<uint128_t>::min(), uint128_t());
    ASSERT_EQ(std::numeric_limits<uint128_t>::max(), uint128_t(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF));
}

TEST(uint128_t, Abs) {
    IPADDRESS_CONSTEXPR auto actual1 = std::abs(uint128_t(5));

    ASSERT_EQ(actual1, uint128_t(5));
}
