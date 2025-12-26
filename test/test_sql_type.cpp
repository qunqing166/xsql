#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <SqlType.h>
#include <string>

TEST(test_sql_type_convert, bool)
{
    EXPECT_EQ(std::string(xsql::TypeMapper<bool>::value), "bool");
}

TEST(test_sql_type_convert, integer)
{
    EXPECT_EQ(std::string(xsql::TypeMapper<int8_t>::value),  "tinyint");
    EXPECT_EQ(std::string(xsql::TypeMapper<int16_t>::value), "smallint");
    EXPECT_EQ(std::string(xsql::TypeMapper<int32_t>::value), "int");
    EXPECT_EQ(std::string(xsql::TypeMapper<int64_t>::value), "bigint");

    // EXPECT_EQ(std::string(xsql::TypeMapper<char>::value),   "tinyint");
    // EXPECT_EQ(std::string(xsql::TypeMapper<short>::value),  "smallint");
    // EXPECT_EQ(std::string(xsql::TypeMapper<int>::value),    "int");
    // EXPECT_EQ(std::string(xsql::TypeMapper<long long>::value), "bigint");
}

TEST(test_sql_type_convert, unsigned_integer)
{
    EXPECT_EQ(std::string(xsql::TypeMapper<uint8_t>::value),  "tinyint unsigned");
    EXPECT_EQ(std::string(xsql::TypeMapper<uint16_t>::value), "smallint unsigned");
    EXPECT_EQ(std::string(xsql::TypeMapper<uint32_t>::value), "int unsigned");
    EXPECT_EQ(std::string(xsql::TypeMapper<uint64_t>::value), "bigint unsigned");
}

TEST(test_sql_type_convert, float_num)
{
    EXPECT_EQ(std::string(xsql::TypeMapper<float>::value), "float");
    EXPECT_EQ(std::string(xsql::TypeMapper<double>::value), "double");
}

TEST(test_sql_type_convert, char_varchar)
{
    // EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 0>>::value), "char(0)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 1>>::value), "char(1)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 5>>::value), "char(5)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 10>>::value), "char(10)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 255>>::value), "char(255)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::array<char, 256>>::value), "char(256)");

    // EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 0>::value), "varchar(0)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 1>::value), "varchar(1)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 5>::value), "varchar(5)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 10>::value), "varchar(10)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 65535>::value), "varchar(65535)");
    EXPECT_EQ(std::string(xsql::TypeMapper<std::string, 65536>::value), "varchar(65536)");

}

TEST(test_sql_type_convert, datetime)
{
    EXPECT_EQ(std::string(xsql::TypeMapper<xsql::DateTime>::value), "datetime");
    EXPECT_EQ(std::string(xsql::TypeMapper<xsql::DateTimeWithAccuracy<xsql::accuracy_s>>::value), "datetime(0)");
    EXPECT_EQ(std::string(xsql::TypeMapper<xsql::DateTimeWithAccuracy<xsql::accuracy_ms>>::value), "datetime(3)");
    EXPECT_EQ(std::string(xsql::TypeMapper<xsql::DateTimeWithAccuracy<xsql::accuracy_us>>::value), "datetime(6)");
}
