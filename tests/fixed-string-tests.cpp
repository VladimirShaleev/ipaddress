#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 17
#  define VAL_FIXED_STRING(name, str) fixed_string name = (str)
#else
#  define VAL_FIXED_STRING(name, str) auto (name) = make_fixed_string(str)
#endif

TEST(fixed_string, ConstexprCtor) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_11, "test string\0\0\0");
    IPADDRESS_CONSTEXPR auto str_11_max_length = str_11.capacity();
    IPADDRESS_CONSTEXPR auto str_11_length = str_11.length;
    IPADDRESS_CONSTEXPR auto str_11_size = str_11.size();
    IPADDRESS_CONSTEXPR auto str_11_empty = str_11.empty();
    EXPECT_EQ(str_11_max_length, 14);
    EXPECT_EQ(str_11_length, 11);
    EXPECT_EQ(str_11_size, 11);
    EXPECT_EQ(str_11_empty, false);
    
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_1, "a");
    IPADDRESS_CONSTEXPR auto str_1_length = str_1.length;
    IPADDRESS_CONSTEXPR auto str_1_size = str_1.size();
    IPADDRESS_CONSTEXPR auto str_1_empty = str_1.empty();
    EXPECT_EQ(str_1_length, 1);
    EXPECT_EQ(str_1_size, 1);
    EXPECT_EQ(str_1_empty, false);

    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_2, L"wide string");
    IPADDRESS_CONSTEXPR auto str_2_max_length = str_2.capacity();
    IPADDRESS_CONSTEXPR auto str_2_length = str_2.length;
    IPADDRESS_CONSTEXPR auto str_2_size = str_2.size();
    IPADDRESS_CONSTEXPR auto str_2_empty = str_2.empty();
    EXPECT_EQ(str_2_max_length, 11);
    EXPECT_EQ(str_2_length, 11);
    EXPECT_EQ(str_2_size, 11);
    EXPECT_EQ(str_2_empty, false);
    EXPECT_EQ(str_2[0], 'w');
    EXPECT_EQ(str_2[10], 'g');

    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_3, u"utf16 string");
    IPADDRESS_CONSTEXPR auto str_3_max_length = str_3.capacity();
    IPADDRESS_CONSTEXPR auto str_3_length = str_3.length;
    IPADDRESS_CONSTEXPR auto str_3_size = str_3.size();
    IPADDRESS_CONSTEXPR auto str_3_empty = str_3.empty();
    EXPECT_EQ(str_3_max_length, 12);
    EXPECT_EQ(str_3_length, 12);
    EXPECT_EQ(str_3_size, 12);
    EXPECT_EQ(str_3_empty, false);
    EXPECT_EQ(str_3[0], 'u');
    EXPECT_EQ(str_3[11], 'g');

    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_4, U"utf32 string");
    IPADDRESS_CONSTEXPR auto str_4_max_length = str_4.capacity();
    IPADDRESS_CONSTEXPR auto str_4_length = str_4.length;
    IPADDRESS_CONSTEXPR auto str_4_size = str_4.size();
    IPADDRESS_CONSTEXPR auto str_4_empty = str_4.empty();
    EXPECT_EQ(str_4_max_length, 12);
    EXPECT_EQ(str_4_length, 12);
    EXPECT_EQ(str_4_size, 12);
    EXPECT_EQ(str_4_empty, false);
    EXPECT_EQ(str_4[0], 'u');
    EXPECT_EQ(str_4[11], 'g');

#if IPADDRESS_CPP_VERSION >= 17
    constexpr VAL_FIXED_STRING(str_5, u8"utf8 string");
    constexpr auto str_5_max_length = str_5.capacity();
    constexpr auto str_5_length = str_5.length;
    constexpr auto str_5_size = str_5.size();
    constexpr auto str_5_empty = str_5.empty();
    EXPECT_EQ(str_5_max_length, 11);
    EXPECT_EQ(str_5_length, 11);
    EXPECT_EQ(str_5_size, 11);
    EXPECT_EQ(str_5_empty, false);
    EXPECT_EQ(str_5[0], 'u');
    EXPECT_EQ(str_5[10], 'g');
#endif
}

TEST(fixed_string, ConstexprCtorEmpty) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_0, "");
    IPADDRESS_CONSTEXPR auto str_0_length = str_0.length;
    IPADDRESS_CONSTEXPR auto str_0_size = str_0.size();
    IPADDRESS_CONSTEXPR auto str_0_empty = str_0.empty();
    EXPECT_EQ(str_0_length, 0);
    EXPECT_EQ(str_0_size, 0);
    EXPECT_EQ(str_0_empty, true);
}

TEST(fixed_string, ConstexprDefaultCtor) {
#if IPADDRESS_CPP_VERSION >= 17 && (!defined(__clang_major__) || __clang_major__ >= 7)
    constexpr fixed_string str_default;
#else
    IPADDRESS_CONSTEXPR fixed_string<0> str_default;
#endif
    IPADDRESS_CONSTEXPR auto str_default_length = str_default.length;
    IPADDRESS_CONSTEXPR auto str_default_size = str_default.size();
    IPADDRESS_CONSTEXPR auto str_default_empty = str_default.empty();
    EXPECT_EQ(str_default_length, 0);
    EXPECT_EQ(str_default_size, 0);
    EXPECT_EQ(str_default_empty, true);
}

TEST(fixed_string, ConstexprCopyCtor) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str, "test string");
#if IPADDRESS_CPP_VERSION >= 17
    constexpr fixed_string copy_str = str;
#else
    IPADDRESS_CONSTEXPR auto copy_str = str;
#endif

    EXPECT_EQ(copy_str.length, 11);
    EXPECT_EQ(copy_str.size(), 11);
}

template <typename Iter>
IPADDRESS_CONSTEXPR bool test_iterator(Iter begin, Iter end) {
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 'e') { return false; }
    if (begin == end || *begin++ != 's') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != ' ') { return false; }
    if (begin == end || *begin++ != 'i') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 'e') { return false; }
    if (begin == end || *begin++ != 'r') { return false; }
    if (begin == end || *begin++ != 'a') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 'o') { return false; }
    if (begin == end || *begin++ != 'r') { return false; }
    if (begin != end) { return false; }
    return true;
}

template <typename Iter>
IPADDRESS_CONSTEXPR bool test_reverse_iterator(Iter begin, Iter end) {
    if (begin == end || *begin++ != 'r') { return false; }
    if (begin == end || *begin++ != 'o') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 'a') { return false; }
    if (begin == end || *begin++ != 'r') { return false; }
    if (begin == end || *begin++ != 'e') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 'i') { return false; }
    if (begin == end || *begin++ != ' ') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin == end || *begin++ != 's') { return false; }
    if (begin == end || *begin++ != 'e') { return false; }
    if (begin == end || *begin++ != 't') { return false; }
    if (begin != end) { return false; }
    return true;
}

TEST(fixed_string, ConstexprIterators) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str, "test iterator");

    IPADDRESS_CONSTEXPR auto actual = test_iterator(str.begin(), str.end());
    EXPECT_TRUE(actual);
    
    IPADDRESS_CONSTEXPR auto actual_const = test_iterator(str.cbegin(), str.cend());
    EXPECT_TRUE(actual_const);
    
    IPADDRESS_CONSTEXPR_17 auto actual_reverse = test_reverse_iterator(str.rbegin(), str.rend());
    EXPECT_TRUE(actual_reverse);
    
    IPADDRESS_CONSTEXPR_17 auto actual_const_reverse = test_reverse_iterator(str.crbegin(), str.crend());
    EXPECT_TRUE(actual_const_reverse);
    
    IPADDRESS_CONSTEXPR auto actual_ptr = test_iterator(str.data(), str.data() + str.size());
    EXPECT_TRUE(actual_ptr);
}

TEST(fixed_string, ConstexprEmptyIterators) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str, "");

    IPADDRESS_CONSTEXPR auto actual = str.begin() == str.end();
    EXPECT_TRUE(actual);
    
    IPADDRESS_CONSTEXPR auto actual_const = str.cbegin() == str.cend();
    EXPECT_TRUE(actual_const);
    
    IPADDRESS_CONSTEXPR_17 auto actual_reverse = str.rbegin() == str.rend();
    EXPECT_TRUE(actual_reverse);
    
    IPADDRESS_CONSTEXPR_17 auto actual_const_reverse = str.crbegin() == str.crend();
    EXPECT_TRUE(actual_const_reverse);
    
    IPADDRESS_CONSTEXPR auto actual_ptr = str.data() == str.data() + str.size();
    EXPECT_TRUE(actual_ptr);
}

TEST(fixed_string, ConstexprAt) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str, "test");
    IPADDRESS_CONSTEXPR auto at0 = str.at(0);
    IPADDRESS_CONSTEXPR auto at1 = str.at(1);
    IPADDRESS_CONSTEXPR auto at2 = str.at(2);
    IPADDRESS_CONSTEXPR auto at3 = str.at(3);
#ifndef IPADDRESS_NO_EXCEPTIONS
    EXPECT_THROW(str.at(4), std::out_of_range);
#endif

    EXPECT_EQ(at0, 't');
    EXPECT_EQ(at1, 'e');
    EXPECT_EQ(at2, 's');
    EXPECT_EQ(at3, 't');
    
    IPADDRESS_CONSTEXPR auto c0 = str[0];
    IPADDRESS_CONSTEXPR auto c1 = str[1];
    IPADDRESS_CONSTEXPR auto c2 = str[2];
    IPADDRESS_CONSTEXPR auto c3 = str[3];
#ifndef IPADDRESS_NO_EXCEPTIONS
    EXPECT_THROW(str[4], std::out_of_range);
#endif
    
    EXPECT_EQ(c0, 't');
    EXPECT_EQ(c1, 'e');
    EXPECT_EQ(c2, 's');
    EXPECT_EQ(c3, 't');
}

TEST(fixed_string, ConstexprFrontBack) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_3, "str");
    IPADDRESS_CONSTEXPR auto front_3 = str_3.front();
    IPADDRESS_CONSTEXPR auto back_3 = str_3.back();
    EXPECT_EQ(front_3, 's');
    EXPECT_EQ(back_3, 'r');
    
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_1, "s");
    IPADDRESS_CONSTEXPR auto front_1 = str_1.front();
    IPADDRESS_CONSTEXPR auto back_1 = str_1.back();
    EXPECT_EQ(front_1, 's');
    EXPECT_EQ(back_1, 's');
}

TEST(fixed_string, ConstexprCompare) {
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_1, "12345\0\0\0");
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_2, "12346");
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_3, "");
    IPADDRESS_CONSTEXPR VAL_FIXED_STRING(str_4, "\0\0\0");

    IPADDRESS_CONSTEXPR auto b_1 = str_1 == str_2;
    IPADDRESS_CONSTEXPR auto b_2 = str_1 != str_2;
    IPADDRESS_CONSTEXPR auto b_3 = str_1 < str_2;
    IPADDRESS_CONSTEXPR auto b_4 = str_1 > str_2;
    IPADDRESS_CONSTEXPR auto b_5 = str_1 <= str_2;
    IPADDRESS_CONSTEXPR auto b_6 = str_1 >= str_2;
    ASSERT_FALSE(b_1);
    ASSERT_TRUE(b_2);
    ASSERT_TRUE(b_3);
    ASSERT_FALSE(b_4);
    ASSERT_TRUE(b_5);
    ASSERT_FALSE(b_6);
    
    IPADDRESS_CONSTEXPR auto b_7 = str_2 == str_1;
    IPADDRESS_CONSTEXPR auto b_8 = str_2 != str_1;
    IPADDRESS_CONSTEXPR auto b_9 = str_2 < str_1;
    IPADDRESS_CONSTEXPR auto b_10 = str_2 > str_1;
    IPADDRESS_CONSTEXPR auto b_11 = str_2 <= str_1;
    IPADDRESS_CONSTEXPR auto b_12 = str_2 >= str_1;
    ASSERT_FALSE(b_7);
    ASSERT_TRUE(b_8);
    ASSERT_FALSE(b_9);
    ASSERT_TRUE(b_10);
    ASSERT_FALSE(b_11);
    ASSERT_TRUE(b_12);
    
    IPADDRESS_CONSTEXPR auto b_13 = str_2 == str_3;
    IPADDRESS_CONSTEXPR auto b_14 = str_2 != str_3;
    IPADDRESS_CONSTEXPR auto b_15 = str_2 < str_3;
    IPADDRESS_CONSTEXPR auto b_16 = str_2 > str_3;
    IPADDRESS_CONSTEXPR auto b_17 = str_2 <= str_3;
    IPADDRESS_CONSTEXPR auto b_18 = str_2 >= str_3;
    ASSERT_FALSE(b_13);
    ASSERT_TRUE(b_14);
    ASSERT_FALSE(b_15);
    ASSERT_TRUE(b_16);
    ASSERT_FALSE(b_17);
    ASSERT_TRUE(b_18);
    
    IPADDRESS_CONSTEXPR auto b_19 = str_3 == str_2;
    IPADDRESS_CONSTEXPR auto b_20 = str_3 != str_2;
    IPADDRESS_CONSTEXPR auto b_21 = str_3 < str_2;
    IPADDRESS_CONSTEXPR auto b_22 = str_3 > str_2;
    IPADDRESS_CONSTEXPR auto b_23 = str_3 <= str_2;
    IPADDRESS_CONSTEXPR auto b_24 = str_3 >= str_2;
    ASSERT_FALSE(b_19);
    ASSERT_TRUE(b_20);
    ASSERT_TRUE(b_21);
    ASSERT_FALSE(b_22);
    ASSERT_TRUE(b_23);
    ASSERT_FALSE(b_24);
    
    IPADDRESS_CONSTEXPR auto b_25 = str_3 == str_3; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_26 = str_3 != str_3; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_27 = str_3 < str_3; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_28 = str_3 > str_3; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_29 = str_3 <= str_3; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_30 = str_3 >= str_3; // NOLINT(misc-redundant-expression)
    ASSERT_TRUE(b_25);
    ASSERT_FALSE(b_26);
    ASSERT_FALSE(b_27);
    ASSERT_FALSE(b_28);
    ASSERT_TRUE(b_29);
    ASSERT_TRUE(b_30);
    
    IPADDRESS_CONSTEXPR auto b_31 = str_2 == str_2; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_32 = str_2 != str_2; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_33 = str_2 < str_2; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_34 = str_2 > str_2; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_35 = str_2 <= str_2; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_36 = str_2 >= str_2; // NOLINT(misc-redundant-expression)
    ASSERT_TRUE(b_31);
    ASSERT_FALSE(b_32);
    ASSERT_FALSE(b_33);
    ASSERT_FALSE(b_34);
    ASSERT_TRUE(b_35);
    ASSERT_TRUE(b_36);

    IPADDRESS_CONSTEXPR auto b_37 = str_3 == str_4;
    IPADDRESS_CONSTEXPR auto b_38 = str_3 != str_4;
    IPADDRESS_CONSTEXPR auto b_39 = str_3 < str_4;
    IPADDRESS_CONSTEXPR auto b_40 = str_3 > str_4;
    IPADDRESS_CONSTEXPR auto b_41 = str_3 <= str_4;
    IPADDRESS_CONSTEXPR auto b_42 = str_3 >= str_4;
    ASSERT_TRUE(b_37);
    ASSERT_FALSE(b_38);
    ASSERT_FALSE(b_39);
    ASSERT_FALSE(b_40);
    ASSERT_TRUE(b_41);
    ASSERT_TRUE(b_42);

    IPADDRESS_CONSTEXPR auto b_43 = str_4 == str_3;
    IPADDRESS_CONSTEXPR auto b_44 = str_4 != str_3;
    IPADDRESS_CONSTEXPR auto b_45 = str_4 < str_3;
    IPADDRESS_CONSTEXPR auto b_46 = str_4 > str_3;
    IPADDRESS_CONSTEXPR auto b_47 = str_4 <= str_3;
    IPADDRESS_CONSTEXPR auto b_48 = str_4 >= str_3;
    ASSERT_TRUE(b_43);
    ASSERT_FALSE(b_44);
    ASSERT_FALSE(b_45);
    ASSERT_FALSE(b_46);
    ASSERT_TRUE(b_47);
    ASSERT_TRUE(b_48);

    IPADDRESS_CONSTEXPR auto b_49 = str_4 == str_4; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_50 = str_4 != str_4; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_51 = str_4 < str_4; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_52 = str_4 > str_4; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_53 = str_4 <= str_4; // NOLINT(misc-redundant-expression)
    IPADDRESS_CONSTEXPR auto b_54 = str_4 >= str_4; // NOLINT(misc-redundant-expression)
    ASSERT_TRUE(b_49);
    ASSERT_FALSE(b_50);
    ASSERT_FALSE(b_51);
    ASSERT_FALSE(b_52);
    ASSERT_TRUE(b_53);
    ASSERT_TRUE(b_54);
}

TEST(fixed_string, InvalidUnicodeString) {
#if __cpp_char8_t >= 201811L

    const char8_t utf8_1[2] = { char8_t(240), char8_t(0) };
    const char8_t utf8_2[3] = { char8_t(240), char8_t(144), char8_t(0) };
    const char8_t utf8_3[4] = { char8_t(240), char8_t(144), char8_t(141), char8_t(0) };
    const char8_t utf8_4[5] = { char8_t(240), char8_t(144), char8_t(141), char8_t(136), char8_t(0) };

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf8_1, utf8_1);
    EXPECT_EQ(str_utf8_1.size(), 0);
#else
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf8_1, utf8_1); },
        ThrowsMessage<parse_error>(StrEq("incorrect sequence of bytes in Unicode encoding for address ")));
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf8_2, utf8_2);
    EXPECT_EQ(str_utf8_2.size(), 0);
#else
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf8_2, utf8_2); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10000} in address {U+10000}")));
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf8_3, utf8_3);
    EXPECT_EQ(str_utf8_3.size(), 0);
#else
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf8_3, utf8_3); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10340} in address {U+10340}")));
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf8_4, utf8_4);
    EXPECT_EQ(str_utf8_4.size(), 0);
#else
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf8_4, utf8_4); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in address {U+10348}")));
#endif

#endif // __cpp_char8_t

    const char16_t utf16_2[2] = { char16_t(55296), char16_t(0) };
    const char16_t utf16_3[3] = { char16_t(55296), char16_t(48), char16_t(0) };
    const char16_t utf16_4[3] = { char16_t(55296), char16_t(57160), char16_t(0) };

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf16_2, utf16_2);
    EXPECT_EQ(str_utf16_2.size(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf16_2, utf16_2); },
        ThrowsMessage<parse_error>(StrEq("incorrect sequence of bytes in Unicode encoding for address ")));
#else
    ASSERT_THROW((make_fixed_string(utf16_2)), parse_error);
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf16_3, utf16_3);
    EXPECT_EQ(str_utf16_3.size(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf16_3, utf16_3); },
        ThrowsMessage<parse_error>(StrEq("incorrect sequence of bytes in Unicode encoding for address ")));
#else
    ASSERT_THROW((make_fixed_string(utf16_3)), parse_error);
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf16_4, utf16_4);
    EXPECT_EQ(str_utf16_4.size(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf16_4, utf16_4); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in address {U+10348}")));
#else
    ASSERT_THROW((make_fixed_string(utf16_4)), parse_error);
#endif

    const char32_t utf32_3[2] = { char32_t(259), char32_t(0) };
    const char32_t utf32_4[2] = { char32_t(66376), char32_t(0) };

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf32_3, utf32_3);
    EXPECT_EQ(str_utf32_3.size(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf32_3, utf32_3); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+0103} in address {U+0103}")));
#else
    ASSERT_THROW((make_fixed_string(utf32_3)), parse_error);
#endif

#ifdef IPADDRESS_NO_EXCEPTIONS
    VAL_FIXED_STRING(str_utf32_4, utf32_4);
    EXPECT_EQ(str_utf32_4.size(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { VAL_FIXED_STRING(str_utf32_4, utf32_4); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in address {U+10348}")));
#else
    ASSERT_THROW((make_fixed_string(utf32_4)), parse_error);
#endif
}

template <typename T, size_t N>
IPADDRESS_CONSTEXPR auto make_fixed_string_and_code(const T (&str)[N]) noexcept -> decltype(std::make_pair(make_fixed_string(str), error_code::no_error)) {
    auto code = error_code::no_error;
    auto result = make_fixed_string(str, code);
    return std::make_pair(result, code);
}

TEST(fixed_string, make_fixed_string) {
    IPADDRESS_CONSTEXPR auto result1 = make_fixed_string_and_code("");
    IPADDRESS_CONSTEXPR auto result2 = make_fixed_string_and_code(u"");
    IPADDRESS_CONSTEXPR auto result3 = make_fixed_string_and_code(U"");
    IPADDRESS_CONSTEXPR auto result4 = make_fixed_string_and_code(L"");
    IPADDRESS_CONSTEXPR auto result5 = make_fixed_string_and_code("1");
    IPADDRESS_CONSTEXPR auto result6 = make_fixed_string_and_code(u"1");
    IPADDRESS_CONSTEXPR auto result7 = make_fixed_string_and_code(U"1");
    IPADDRESS_CONSTEXPR auto result8 = make_fixed_string_and_code(L"1");
    IPADDRESS_CONSTEXPR auto result9 = make_fixed_string_and_code(u"1\U00010348");
    IPADDRESS_CONSTEXPR auto result10 = make_fixed_string_and_code(U"1\U00010348");
    IPADDRESS_CONSTEXPR auto result11 = make_fixed_string_and_code(L"1\U00010348");
    ASSERT_EQ(result1.first.size(), 0);
    ASSERT_EQ(result2.first.size(), 0);
    ASSERT_EQ(result3.first.size(), 0);
    ASSERT_EQ(result4.first.size(), 0);
    ASSERT_EQ(result5.first.size(), 1);
    ASSERT_EQ(result6.first.size(), 1);
    ASSERT_EQ(result7.first.size(), 1);
    ASSERT_EQ(result8.first.size(), 1);
    ASSERT_EQ(result9.first.size(), 1);
    ASSERT_EQ(result10.first.size(), 1);
    ASSERT_EQ(result11.first.size(), 1);
    ASSERT_EQ(result1.second, error_code::no_error);
    ASSERT_EQ(result2.second, error_code::no_error);
    ASSERT_EQ(result3.second, error_code::no_error);
    ASSERT_EQ(result4.second, error_code::no_error);
    ASSERT_EQ(result5.second, error_code::no_error);
    ASSERT_EQ(result6.second, error_code::no_error);
    ASSERT_EQ(result7.second, error_code::no_error);
    ASSERT_EQ(result8.second, error_code::no_error);
    ASSERT_EQ(result9.second, error_code::unexpected_symbol);
    ASSERT_EQ(result10.second, error_code::unexpected_symbol);
    ASSERT_EQ(result11.second, error_code::unexpected_symbol);

#if __cpp_char8_t >= 201811L
    constexpr auto result12 = make_fixed_string_and_code(u8"");
    constexpr auto result13 = make_fixed_string_and_code(u8"1");
    constexpr auto result14 = make_fixed_string_and_code(u8"1\U00010348");
    ASSERT_EQ(result12.first.size(), 0);
    ASSERT_EQ(result13.first.size(), 1);
    ASSERT_EQ(result14.first.size(), 1);
    ASSERT_EQ(result12.second, error_code::no_error);
    ASSERT_EQ(result13.second, error_code::no_error);
    ASSERT_EQ(result14.second, error_code::unexpected_symbol);
#endif
}
