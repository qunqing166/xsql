#include <gtest/gtest.h>
#include <EntityBase.h>
#include <xsql.h>

class User: public xsql::EntityBase<User>
{
    FIELD_REGISTER(int, Id);
    FIELD_REGISTER(std::string, Name);
    FIELD_REGISTER(int, Age);

    // FIELD_REGISTER(xsql::DateTime, CreateTime);
    // FIELD_REGISTER(xsql::DateTime, UpdateTime);
};

TEST(functoin_xsql_test, create_table)
{
    auto con = xsql::SqlConnection::Create("root", "114514", "test");

    auto val = con->Define().CreateTable("user")
                            .Filed("id", "int", "just index", 
                                                xsql::CreateTable::AUTO_INCREMENT | 
                                                xsql::CreateTable::CAN_BE_NULL |
                                                xsql::CreateTable::IS_UNIQUE
                                                )
                            .Filed("name", "varchar(100)", "user name", xsql::CreateTable::NONE)
                            .Filed("age", "int", "",    xsql::CreateTable::AUTO_INCREMENT | 
                                                        xsql::CreateTable::CAN_BE_NULL |
                                                        xsql::CreateTable::IS_UNIQUE)
                            .AddPrimaryKey("id")
                            .Execute();

    EXPECT_EQ(val, true);
    EXPECT_EQ(con->Define().DropTable("user").Execute(), true);
    
}