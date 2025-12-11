#include <gtest/gtest.h>
#include <EntityBase.h>
#include <xsql.h>

class User: public xsql::EntityBase<User>
{
    FIELD_REGISTER(int, Id);
    FIELD_REGISTER(std::string, Name);
    FIELD_REGISTER(int, Age);

    FIELD_REGISTER(xsql::DateTime, CreateTime);
    FIELD_REGISTER(xsql::DateTime, UpdateTime);
};

TEST(functoin_xsql_test, create_table)
{
    auto con = xsql::SqlConnection::Create("root", "114514", "test");

    auto val = con->Define().CreateTable("user")
                            .Filed("id", "int", "just index", 
                                                xsql::CreateTable::AUTO_INCREMENT | 
                                                xsql::CreateTable::NOT_NULL |
                                                xsql::CreateTable::IS_UNIQUE)
                            .Filed("name", "varchar(100)", "user name", xsql::CreateTable::NONE)
                            .Filed("age", "int", "",    xsql::CreateTable::AUTO_INCREMENT | 
                                                        xsql::CreateTable::NOT_NULL |
                                                        xsql::CreateTable::IS_UNIQUE)
                            .Filed("create_time", "datetime")
                            .Filed("update_time", "datetime")
                            .AddPrimaryKey("id")
                            .Execute();

    EXPECT_EQ(val, true);
    EXPECT_EQ(con->Define().DropTable("user").Execute(), true);
    
}

TEST(functoin_xsql_test, xsql_query)
{
    auto con = xsql::SqlConnection::Create("root", "114514", "test");

    auto val = con->Define().CreateTable("user")
                            .Filed("id", "int", "just index", 
                                                xsql::CreateTable::AUTO_INCREMENT | 
                                                xsql::CreateTable::NOT_NULL |
                                                xsql::CreateTable::IS_UNIQUE)
                            .Filed("name", "varchar(100)", "user name", xsql::CreateTable::NONE)
                            .Filed("age", "int", "",    xsql::CreateTable::AUTO_INCREMENT | 
                                                        xsql::CreateTable::NOT_NULL)
                            .Filed("create_time", "datetime(3)", "", xsql::CreateTable::NOT_NULL)
                            .Filed("update_time", "datetime(3)", "", xsql::CreateTable::NOT_NULL)
                            .AddPrimaryKey("id")
                            .Execute();

    EXPECT_EQ(val, true);

    xsql::DateTime now{};

    auto count = con->Query().Insert("user", {  {"id", 0}, 
                                                {"name", "qunqing166"}, 
                                                {"age", 21}, 
                                                {"create_time", now},
                                                {"update_time", now}}).Execute();
    EXPECT_EQ(count, 1);

    EXPECT_EQ(con->Query().Select("user", {"id"}).Execute().RowCount(), 1);

    auto res = con->Query() .Select("user", {"id", "name", "age", "create_time", "update_time"})
                            .Where(xsql::QueryWhere()
                            .Equal("id", 0))
                            .Execute();

    while(auto row = res.GetRow()){
        EXPECT_EQ(row.Get<int>("id"), 1);
        EXPECT_EQ(row.Get<std::string>("name"), "qunqing166");
        EXPECT_EQ(row.Get<int>("age"), 21);
        EXPECT_EQ(row.Get<xsql::DateTime>("create_time").GetSec(), now.GetSec());
        EXPECT_EQ(row.Get<xsql::DateTime>("update_time").GetSec(), now.GetSec());
    }

    now = xsql::DateTime::Now();
    count = con->Query().Update("user", {   {"name", "qunqing"},
                                            {"age", 18},
                                            {"update_time", now}})
                        .Where(xsql::QueryWhere().Equal("id", 1))
                        .Execute();

    EXPECT_EQ(count, 1);

    auto res1 = con->Query().Select("user", {"id", "name", "age", "create_time", "update_time"})
                            .Where(xsql::QueryWhere()
                            .Equal("id", 0))
                            .Execute();

    while(auto row = res1.GetRow()){
        EXPECT_EQ(row.Get<int>("id"), 1);
        EXPECT_EQ(row.Get<std::string>("name"), "qunqing");
        EXPECT_EQ(row.Get<int>("age"), 18);
        EXPECT_EQ(row.Get<xsql::DateTime>("update_time").GetSec(), now.GetSec());
    }

    count = con->Query().Delete("user").Where(xsql::QueryWhere{}.Equal("id", 1)).Execute();
    EXPECT_EQ(count, 1);

    EXPECT_EQ(con->Query().Select("user", {"id"}).Execute().RowCount(), 0);

    EXPECT_ANY_THROW(con->Query().Insert("user", {  {"id", 0}, 
                                                    {"name", "qunqing166"}, 
                                                    {"age", 21}, 
                                                    {"create_time", xsql::DateTime()}}).Execute());

    EXPECT_EQ(con->Define().DropTable("user").Execute(), true);
}