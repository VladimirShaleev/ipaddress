#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;
using ipaddress::internal::char_reader;

template <typename T, size_t N>
constexpr bool constexpr_test_error(const T (&str)[N]) noexcept {
    const T* it = str;
    const T* end = it + N;
    while (it != end) {
        auto code = error_code::no_error;
        uint32_t error_symbol = 0;
        char symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
        if (code != error_code::no_error) {
            return false;
        }
        if (symbol == '\0') {
            break;
        }
    }
    return true;
}

template <typename T, size_t N>
constexpr bool constexpr_test(const T (&str)[N]) {
    const T* it = str;
    const T* begin = str;
    const T* end = it + N;
    while (it != end) {
        char symbol = char_reader<T>::next(it, begin, end);
        if (symbol == '\0') {
            break;
        }
    }
    return true;
}

template <typename T, size_t N>
void test(const T (&str)[N]) { // NOLINT(readability-function-cognitive-complexity)
    const T* it = str;
    const T* end = it + N;

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    symbol = char_reader<T>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}

template <typename T, size_t N>
void test_exception(const T (&str)[N]) {
    const T* it = str;
    const T* begin = it;
    const T* end = it + sizeof(str);

    char symbol = '\0';

    symbol = char_reader<T>::next(it, begin, end);
    EXPECT_EQ(symbol, '1');

    symbol = char_reader<T>::next(it, begin, end);
    EXPECT_EQ(symbol, '2');
    
#ifdef IPADDRESS_NO_EXCEPTIONS
    const auto actual = char_reader<T>::next(it, begin, end);
    EXPECT_EQ(actual, '\0');
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [=]() { auto curr_it = it; char_reader<T>::next(curr_it, begin, end); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in string 12{U+10348}{U+d55c}{U+0418}$")));
#else
    ASSERT_THROW((char_reader<T>::next(it, begin, end)), parse_error);
#endif
}

#if IPADDRESS_CPP_VERSION >= 14

TEST(char_reader, CompileTime) {
    constexpr auto result1 = constexpr_test_error("1234");
    constexpr auto result2 = constexpr_test_error(u"1234");
    constexpr auto result3 = constexpr_test_error(u"1猫4");
    constexpr auto result4 = constexpr_test_error(U"1234");
    constexpr auto result5 = constexpr_test_error(U"1猫4");
    constexpr auto result6 = constexpr_test_error(L"1234");
    constexpr auto result7 = constexpr_test_error(L"1猫4");
    ASSERT_TRUE(result1);
    ASSERT_TRUE(result2);
    ASSERT_FALSE(result3);
    ASSERT_TRUE(result4);
    ASSERT_FALSE(result5);
    ASSERT_TRUE(result6);
    ASSERT_FALSE(result7);

    constexpr auto result8 = constexpr_test("1234");
    constexpr auto result9 = constexpr_test(u"1234");
    constexpr auto result10 = constexpr_test(U"1234");
    constexpr auto result11 = constexpr_test(L"1234");
    ASSERT_TRUE(result8);
    ASSERT_TRUE(result9);
    ASSERT_TRUE(result10);
    ASSERT_TRUE(result11);

#if __cpp_char8_t >= 201811L
    constexpr auto result12 = constexpr_test_error(u8"1234");
    constexpr auto result13 = constexpr_test_error(u8"12\ud55c34");
    constexpr auto result14 = constexpr_test(u8"1234");
    ASSERT_TRUE(result12);
    ASSERT_FALSE(result13);
    ASSERT_TRUE(result14);
#endif // __cpp_char8_t
}

#endif

#if __cpp_char8_t >= 201811L
TEST(char_reader, Utf8) {
    test(u8"12\U00010348\ud55c\u0418$");
}
#endif // __cpp_char8_t

TEST(char_reader, Utf16) {
    test(u"12\U00010348\ud55c\u0418$");
}

TEST(char_reader, Utf32) {
    test(U"12\U00010348\ud55c\u0418$");
}

TEST(char_reader, WideChar) {
    test(L"12\U00010348\ud55c\u0418$");
}

#if __cpp_char8_t >= 201811L
TEST(char_reader, Utf8Exception) {
    test_exception(u8"12\U00010348\ud55c\u0418$");
}
#endif // __cpp_char8_t

TEST(char_reader, Utf16Exception) {
    test_exception(u"12\U00010348\ud55c\u0418$");
}

TEST(char_reader, Utf32Exception) {
    test_exception(U"12\U00010348\ud55c\u0418$");
}

TEST(char_reader, WideCharException) {
    test_exception(L"12\U00010348\ud55c\u0418$");
}

#ifdef IPADDRESS_CHAR_IS_UTF8
TEST(char_reader, Char) {
    char str[13] = { 
        char(49), 
        char(50), 
        char(240), char(144), char(141), char(136), 
        char(237), char(149), char(156), 
        char(208), char(152), 
        char(36), 
        char(0) 
    };
    test(str);
}
#else // IPADDRESS_CHAR_IS_UTF8
TEST(char_reader, Char) {
    constexpr char str[] = "abc123";

    const char* it = str;
    const char* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'a');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'b');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'c');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '3');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    symbol = char_reader<char>::next_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}
#endif
