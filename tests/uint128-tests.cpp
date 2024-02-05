#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

TEST(uint128_t, CompileTime) {
    constexpr uint128_t value1 = { 1, 0 };
    constexpr uint128_t value2 = value1 << 2;
    constexpr uint128_t value3 = value2 >> 1;
    constexpr uint128_t value4 = value3 * 4;
    constexpr uint128_t value5 = value4 / 3;
    ASSERT_EQ(value5.high, 2);
    ASSERT_EQ(value5.low, 0xAAAAAAAAAAAAAAAA);
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

    ASSERT_EQ(value1.high, 0);
    ASSERT_EQ(value1.low, 0);
    ASSERT_EQ(value2.high, 0);
    ASSERT_EQ(value2.low, 0);
    ASSERT_EQ(value3.high, 0);
    ASSERT_EQ(value3.low, 1);
    ASSERT_EQ(value4.high, 1);
    ASSERT_EQ(value4.low, 2);
    ASSERT_EQ(value5.high, 1);
    ASSERT_EQ(value5.low, 2);
    ASSERT_EQ(value6.high, 1);
    ASSERT_EQ(value6.low, 2);
    ASSERT_EQ(value7.high, 0);
    ASSERT_EQ(value7.low, 1);
    ASSERT_EQ(value8.high, 0);
    ASSERT_EQ(value8.low, 1);
}

TEST(uint128_t, Copy) {
    constexpr uint128_t value = 5;

    constexpr uint128_t copy_ctor = value;
    uint128_t copy_operator;
    copy_operator = value;

    ASSERT_EQ(copy_ctor.high, 0);
    ASSERT_EQ(copy_ctor.low, 5);
    ASSERT_EQ(copy_operator.high, 0);
    ASSERT_EQ(copy_operator.low, 5);
}

TEST(uint128_t, Move) {
    constexpr uint128_t value = 5;

    constexpr uint128_t move_ctor = std::move(value);
    uint128_t move_operator;
    move_operator = std::move(move_ctor);

    ASSERT_EQ(move_operator.high, 0);
    ASSERT_EQ(move_operator.low, 5);
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
    ASSERT_EQ(plus1.high, 4);
    ASSERT_EQ(plus1.low, 5);
    ASSERT_EQ(plus2.high, 4);
    ASSERT_EQ(plus2.low, 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(plus3.high, 4);
    ASSERT_EQ(plus3.low, 0xFFFFFFFFFFFFFFFFULL);

    constexpr auto minus1 = -value1;
    constexpr auto minus2 = -value2;
    constexpr auto minus3 = -value3;
    ASSERT_EQ(minus1.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus1.low, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus2.low, 2);
    ASSERT_EQ(minus3.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(minus3.low, 1);
    
    constexpr auto inv1 = ~value1;
    constexpr auto inv2 = ~value2;
    constexpr auto inv3 = ~value3;
    ASSERT_EQ(inv1.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv1.low, 0xFFFFFFFFFFFFFFFFULL - 5);
    ASSERT_EQ(inv2.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv2.low, 1);
    ASSERT_EQ(inv3.high, 0xFFFFFFFFFFFFFFFFULL - 4);
    ASSERT_EQ(inv3.low, 0);
    
    constexpr auto sum1 = value1 + 1;
    constexpr auto sum2 = value2 + 1;
    constexpr auto sum3 = value3 + 1;
    constexpr auto sum4 = value1 + uint128_t(10, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sum5 = 1 + value1;
    ASSERT_EQ(sum1.high, 4);
    ASSERT_EQ(sum1.low, 6);
    ASSERT_EQ(sum2.high, 4);
    ASSERT_EQ(sum2.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sum3.high, 5);
    ASSERT_EQ(sum3.low, 0);
    ASSERT_EQ(sum4.high, 15);
    ASSERT_EQ(sum4.low, 2);
    ASSERT_EQ(sum5.high, 4);
    ASSERT_EQ(sum5.low, 6);
    
    constexpr auto sub1 = value1 - 6;
    constexpr auto sub2 = value2 - 7;
    constexpr auto sub3 = value3 - 1;
    constexpr auto sub4 = value1 - uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    constexpr auto sub5 = 10 - uint128_t(7);
    ASSERT_EQ(sub1.high, 3);
    ASSERT_EQ(sub1.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(sub2.high, 4);
    ASSERT_EQ(sub2.low, 0xFFFFFFFFFFFFFFFFULL - 8);
    ASSERT_EQ(sub3.high, 4);
    ASSERT_EQ(sub3.low, 0xFFFFFFFFFFFFFFFFULL - 1);
    ASSERT_EQ(sub4.high, 1);
    ASSERT_EQ(sub4.low, 8);
    ASSERT_EQ(sub5.high, 0);
    ASSERT_EQ(sub5.low, 3);
    
    constexpr auto mul1 = value1 * 6;
    constexpr auto mul2 = value2 * 7;
    constexpr auto mul3 = value3 * 1;
    constexpr auto mul4 = 1000 * uint128_t(2, 0xFFFFFFFFFFFFFFFFULL - 2);
    ASSERT_EQ(mul1.high, 24);
    ASSERT_EQ(mul1.low, 30);
    ASSERT_EQ(mul2.high, 34);
    ASSERT_EQ(mul2.low, 0XFFFFFFFFFFFFFFF2);
    ASSERT_EQ(mul3.high, 4);
    ASSERT_EQ(mul3.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(mul4.high, 0xBB7);
    ASSERT_EQ(mul4.low, 0xFFFFFFFFFFFFF448);
    
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
    ASSERT_EQ(div1.high, 0);
    ASSERT_EQ(div1.low, 0x28F5C28F5C28F5C2);
    ASSERT_EQ(div2.high, 2);
    ASSERT_EQ(div2.low, 0x7fffffffffffffffULL);
    ASSERT_EQ(div3.high, 4);
    ASSERT_EQ(div3.low, 0xFFFFFFFFFFFFFFFFULL);
    ASSERT_EQ(div4.high, 0);
    ASSERT_EQ(div4.low, 0x1F4);
    ASSERT_EQ(div5.high, 0);
    ASSERT_EQ(div5.low, 0);
    ASSERT_EQ(div6.high, 0);
    ASSERT_EQ(div6.low, 0);
    ASSERT_EQ(div7.high, 0);
    ASSERT_EQ(div7.low, 1);
    ASSERT_EQ(div8.high, 0);
    ASSERT_EQ(div8.low, 0);
    ASSERT_EQ(div9.high, 0);
    ASSERT_EQ(div9.low, 1);
    ASSERT_EQ(div10.high, 0);
    ASSERT_EQ(div10.low, 0);

    auto rem1 = value1 % 25;
    auto rem2 = value2 % 2;
    auto rem3 = value3 % 1;
    auto rem4 = 15 % value1;
    ASSERT_EQ(rem1.high, 0);
    ASSERT_EQ(rem1.low, 19);
    ASSERT_EQ(rem2.high, 0);
    ASSERT_EQ(rem2.low, 0);
    ASSERT_EQ(rem3.high, 0);
    ASSERT_EQ(rem3.low, 0);
    ASSERT_EQ(rem4.high, 0);
    ASSERT_EQ(rem4.low, 15);

    auto and1 = uint128_t(1, 1) & uint128_t(3, 3);
    auto and2 = uint128_t(1, 1) & 1;
    auto and3 = 1 & uint128_t(1, 1);
    ASSERT_EQ(and1.high, 1);
    ASSERT_EQ(and1.low, 1);
    ASSERT_EQ(and2.high, 0);
    ASSERT_EQ(and2.low, 1);
    ASSERT_EQ(and3.high, 0);
    ASSERT_EQ(and3.low, 1);

    auto or1 = uint128_t(1, 1) | uint128_t(2, 2);
    auto or2 = uint128_t(1, 1) | 2;
    auto or3 = 2 | uint128_t(1, 1);
    ASSERT_EQ(or1.high, 3);
    ASSERT_EQ(or1.low, 3);
    ASSERT_EQ(or2.high, 1);
    ASSERT_EQ(or2.low, 3);
    ASSERT_EQ(or3.high, 1);
    ASSERT_EQ(or3.low, 3);

    auto xor1 = uint128_t(1, 1) ^ uint128_t(3, 3);
    auto xor2 = uint128_t(1, 1) ^ 3;
    auto xor3 = 3 ^ uint128_t(1, 1);
    ASSERT_EQ(xor1.high, 2);
    ASSERT_EQ(xor1.low, 2);
    ASSERT_EQ(xor2.high, 1);
    ASSERT_EQ(xor2.low, 2);
    ASSERT_EQ(xor3.high, 1);
    ASSERT_EQ(xor3.low, 2);
    
    auto lshift1 = uint128_t(0, 0xFFFFFFFFFFFFFFFFULL) << 1;
    ASSERT_EQ(lshift1.high, 1);
    ASSERT_EQ(lshift1.low, 0xFFFFFFFFFFFFFFFEULL);
}

TEST(uint128_t, Assignment) {
    uint128_t value { 4, 5 };

    value += 3;
    ASSERT_EQ(value.high, 4);
    ASSERT_EQ(value.low, 8);

    value -= 2;
    ASSERT_EQ(value.high, 4);
    ASSERT_EQ(value.low, 6);

    value *= 2;
    ASSERT_EQ(value.high, 8);
    ASSERT_EQ(value.low, 12);

    value /= 2;
    ASSERT_EQ(value.high, 4);
    ASSERT_EQ(value.low, 6);

    value %= 3;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 1);

    value &= 0;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 0);

    value |= 1;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 1);

    value ^= 3;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 2);

    value <<= 1;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 4);
    
    value >>= 1;
    ASSERT_EQ(value.high, 0);
    ASSERT_EQ(value.low, 2);
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
