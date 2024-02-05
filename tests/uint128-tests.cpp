#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

TEST(uint128_t, CompileTime) {
    constexpr uint128_t value1 = { 1, 0 };
    constexpr uint128_t value2 = value1 << 2;
    constexpr uint128_t value3 = value2 >> 1;
    constexpr uint128_t value4 = value3 * 4;
    constexpr uint128_t value5 = value4 / 3;
    ASSERT_EQ(value5.upper(), 2);
    ASSERT_EQ(value5.lower(), 0xAAAAAAAAAAAAAAAA);
}

TEST(uint128_t, Ctors) {
    constexpr uint128_t value1;
    constexpr uint128_t value2{};
    constexpr uint128_t value3 = 1;
    constexpr uint128_t value4(1, 2);
    constexpr uint128_t value5 { 1, 2 };
    constexpr uint128_t value6 = { 1, 2 };
    constexpr uint128_t value7 { 1 };
    constexpr uint128_t value8 = { 1 };

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
}

TEST(uint128_t, Copy) {
    constexpr uint128_t value = 5;

    constexpr uint128_t copy_ctor = value;
    uint128_t copy_operator;
    copy_operator = value;

    ASSERT_EQ(copy_ctor.upper(), 0);
    ASSERT_EQ(copy_ctor.lower(), 5);
    ASSERT_EQ(copy_operator.upper(), 0);
    ASSERT_EQ(copy_operator.lower(), 5);
}

TEST(uint128_t, Move) {
    constexpr uint128_t value = 5;

    constexpr uint128_t move_ctor = std::move(value);
    uint128_t move_operator;
    move_operator = std::move(move_ctor);

    ASSERT_EQ(move_operator.upper(), 0);
    ASSERT_EQ(move_operator.lower(), 5);
}

TEST(uint128_t, OperatorBool) {
    constexpr uint128_t value1 = 0;
    constexpr uint128_t value2 = 5;
    
    constexpr auto actual1 = (bool) value1;
    constexpr auto actual2 = (bool) value2;

    ASSERT_FALSE(actual1);
    ASSERT_TRUE(actual2);
}

TEST(uint128_t, OperatorT) {
    constexpr uint128_t value { 4, 140185576636287 };
    
    constexpr auto actual1 = (char) value;
    constexpr auto actual2 = (signed char) value;
    constexpr auto actual3 = (unsigned char) value;
    constexpr auto actual4 = (short) value;
    constexpr auto actual5 = (unsigned short) value;
    constexpr auto actual6 = (int) value;
    constexpr auto actual7 = (unsigned int) value;
    constexpr auto actual8 = (long) value;
    constexpr auto actual9 = (unsigned long) value;
    constexpr auto actual10 = (long long) value;
    constexpr auto actual11 = (unsigned long long) value;

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
}

TEST(uint128_t, Arithmetic) {
    constexpr uint128_t value1 { 4, 5 };
    constexpr uint128_t value2 { 4, 0xFFFFFFFFFFFFFFFFULL - 1 };
    constexpr uint128_t value3 { 4, 0xFFFFFFFFFFFFFFFFULL };

    constexpr auto plus1 = +value1;
    constexpr auto plus2 = +value2;
    constexpr auto plus3 = +value3;
    ASSERT_EQ(plus1.upper(), 4);
    ASSERT_EQ(plus1.lower(), 5);
    ASSERT_EQ(plus2.upper(), 4);
    ASSERT_EQ(plus2.lower(), 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(plus3.upper(), 4);
    ASSERT_EQ(plus3.lower(), 0xFFFFFFFFFFFFFFFFULL);

    constexpr auto minus1 = -value1;
    constexpr auto minus2 = -value2;
    constexpr auto minus3 = -value3;
    ASSERT_EQ(minus1.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus1.lower(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.lower(), 2);
    ASSERT_EQ(minus3.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus3.lower(), 1);
    
    constexpr auto inv1 = ~value1;
    constexpr auto inv2 = ~value2;
    constexpr auto inv3 = ~value3;
    ASSERT_EQ(inv1.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv1.lower(), 0xFFFFFFFFFFFFFFFFULL - 5);
    ASSERT_EQ(inv2.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv2.lower(), 1);
    ASSERT_EQ(inv3.upper(), 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv3.lower(), 0);
    
    constexpr auto sum1 = value1 + 1;
    constexpr auto sum2 = value2 + 1;
    constexpr auto sum3 = value3 + 1;
    constexpr auto sum4 = value1 + uint128_t(10, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sum5 = 1 + value1;
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
    
    constexpr auto sub1 = value1 - 6;
    constexpr auto sub2 = value2 - 7;
    constexpr auto sub3 = value3 - 1;
    constexpr auto sub4 = value1 - uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sub5 = 10 - uint128_t(7);
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
    
    constexpr auto mul1 = value1 * 6;
    constexpr auto mul2 = value2 * 7;
    constexpr auto mul3 = value3 * 1;
    constexpr auto mul4 = 1000 * uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
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
    uint128_t value1 { 0, 0 };
    uint128_t value2 { 0, 1 };
    uint128_t value3 { 1, 0 };
    uint128_t value4 { 1, 1 };

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
    uint128_t value1 { 0, 0 };
    uint128_t value2 { 0, 1 };
    uint128_t value3 { 1, 0 };
    uint128_t value4 { 1, 1 };

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