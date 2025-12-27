#include "mysql/MysqlConnection.h"
#include <gtest/gtest.h>
#include <SqlDefine.h>

struct TestCreateTable:testing::Test{
    void SetUp() override{
        con = xsql::mysql::MysqlConnection::Create("root", "114514", "test");
    }
    void TearDown() override{
        con.reset();
    }

    std::shared_ptr<xsql::SqlConnection> con;
};

TEST_F(TestCreateTable, t1)
{
    con->Define().CreateTable("user")
                    .Filed<int>("id")
                    .Filed<std::string, 20>("name")
                    .Filed<int>("age")
                    .Filed<std::string, 50>("addr")
                    .Execute();
                               
    con->Define().DropTable("user").Execute();
}

// TEST(test_create_table, runtime_add_field)
// {
//     auto con = xsql::SqlConnection::Create("root", "114514", "test");
//
//     for(int i = 0; i < 1000000; ++i){
//         con->Define().CreateTable("t").Filed("name", "varchar(10)");
//     }
// }
//
// TEST(test_create_table, runtime_add_field1)
// {
//     auto con = xsql::SqlConnection::Create("root", "114514", "test");
//
//     for(int i = 0; i < 1000000; ++i){
//         con->Define().CreateTable("t").Filed<std::string, 10>("name");
//     }
// }



