#include <cstdint>
#include <filesystem>
#include <iostream>
#include <pthread.h>
#include <string>
#include "utils.h"
#include <SqlType.h>
#include <gtest/gtest.h>

using namespace xsql;

// class test
// {
//    int8_t a;
//    int16_t b;
//    int32_t c;
//    int64_t d;
//    std::string e;
//    DateTime f;
// };

TEST(ValueConvert, StringToBool)
{
    EXPECT_EQ(ConvertSqlValue<bool>("true"), true);
    EXPECT_EQ(ConvertSqlValue<bool>("false"), false);

    EXPECT_EQ(ConvertSqlValue<bool>("True"), true);
    EXPECT_EQ(ConvertSqlValue<bool>("False"), false);

    EXPECT_EQ(ConvertSqlValue<bool>("1"), true);
    EXPECT_EQ(ConvertSqlValue<bool>("0"), false);

    EXPECT_EQ(ConvertSqlValue<bool>("yes"), true);
    EXPECT_EQ(ConvertSqlValue<bool>("no"), false);

    EXPECT_EQ(ConvertSqlValue<bool>("on"), true);
    EXPECT_EQ(ConvertSqlValue<bool>("off"), false);

    EXPECT_ANY_THROW(ConvertSqlValue<bool>(""));
    EXPECT_ANY_THROW(ConvertSqlValue<bool>("rue"));
    EXPECT_ANY_THROW(ConvertSqlValue<bool>("12"));
}

TEST(ValueConvert, StringToInt)
{
    EXPECT_EQ(ConvertSqlValue<int>("0"), 0);
    EXPECT_EQ(ConvertSqlValue<int>("-0"), 0);

    EXPECT_EQ(ConvertSqlValue<int>("123"), 123);
    EXPECT_EQ(ConvertSqlValue<int>("12.3"), 12);

    EXPECT_EQ(ConvertSqlValue<int>("-123"), -123);
    EXPECT_EQ(ConvertSqlValue<int>("-12.3"), -12);
    EXPECT_EQ(ConvertSqlValue<int>("1."), 1);

    // EXPECT_EQ(ConvertSqlValue<int>(".1"), 0);

    EXPECT_ANY_THROW(ConvertSqlValue<int>(""));
    EXPECT_ANY_THROW(ConvertSqlValue<int>("1aa23"));
    EXPECT_ANY_THROW(ConvertSqlValue<int>("."));
    EXPECT_ANY_THROW(ConvertSqlValue<int>(".1"));
}

TEST(ValueConvert, StringToString)
{
    EXPECT_EQ(ConvertSqlValue<std::string>(""), "");
    EXPECT_EQ(ConvertSqlValue<std::string>("01234"), "01234");
}

TEST(ValueConvert, StringToUInt)
{
    EXPECT_EQ(ConvertSqlValue<uint8_t>("0"), 0);
    EXPECT_ANY_THROW(ConvertSqlValue<uint8_t>("-0"));

    EXPECT_EQ(ConvertSqlValue<uint8_t>("123"), 123);
    EXPECT_EQ(ConvertSqlValue<uint8_t>("12.3"), 12);

    EXPECT_ANY_THROW(ConvertSqlValue<uint8_t>("256"));
    EXPECT_ANY_THROW(ConvertSqlValue<uint8_t>("1000"));
    EXPECT_ANY_THROW(ConvertSqlValue<uint8_t>("-123"));
    EXPECT_ANY_THROW(ConvertSqlValue<uint8_t>("-12.3"));
    EXPECT_EQ(ConvertSqlValue<uint8_t>("1."), 1);
}

TEST(ValueConvert, StringToDateTime)
{
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0").GetMs(), 1765382400000);
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0.0").GetSec(), 1765382400);
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0").ToString(), "2025-12-11 0:0:0.0");
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0.0").ToString(), "2025-12-11 0:0:0.0");
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0.0").ToString(false), "2025-12-11 0:0:0");

    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10.120").GetMs(), 1765427410120);
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10.120").GetSec(), 1765427410);
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10").ToString(), "2025-12-11 12:30:10.0");
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10.120").ToString(), "2025-12-11 12:30:10.120");
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10.120").ToString(false), "2025-12-11 12:30:10");

    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 0:0:0"), xsql::DateTime(1765382400000));
    EXPECT_EQ(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:10.120"), xsql::DateTime(1765427410120));

    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>(""));
    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>("2025-12-11 12:30:"));
    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>("ac2025-12-11 12:30:"));
    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>("12-11 12:30:"));
    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>("2025-12 12:30:0"));
    EXPECT_ANY_THROW(ConvertSqlValue<xsql::DateTime>("a-b-c 12:30:0"));
}