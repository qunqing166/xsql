#include <gtest/gtest.h>
#include <EntityBase.h>
#include <xsql.h>
#include <CommonRepository.h>

class User: public xsql::EntityBase<User>
{
    FIELD_REGISTER(int, Id);
    FIELD_REGISTER(std::string, Name);
    FIELD_REGISTER(int, Age);

    FIELD_REGISTER(xsql::DateTime, CreateTime);
    FIELD_REGISTER(xsql::DateTime, UpdateTime);
};

TEST(test_repository, curd)
{
    auto con = xsql::SqlConnection::Create("root", "114514", "test");

    auto val = con->Define().CreateTable("User")
                            .Filed("id", "int", "just index", 
                                                xsql::CreateTable::AUTO_INCREMENT | 
                                                xsql::CreateTable::NOT_NULL |
                                                xsql::CreateTable::IS_UNIQUE)
                            .Filed("name", "varchar(100)", "user name", xsql::CreateTable::NONE)
                            .Filed("age", "int", "",    xsql::CreateTable::AUTO_INCREMENT | 
                                                        xsql::CreateTable::NOT_NULL |
                                                        xsql::CreateTable::IS_UNIQUE)
                            .Filed("createtime", "datetime")
                            .Filed("updatetime", "datetime")
                            .AddPrimaryKey("id")
                            .Execute();
    EXPECT_EQ(val, true);
                            
    auto repo = xsql::CommonRepository<User>(con);

    User user;
    user.SetName("qunqing166");
    user.SetAge(18);
    
    EXPECT_EQ(repo.Add(user), 1);

    auto res = repo.GetFirstOrDefault(xsql::QueryWhere{}.Equal("id", 1));
    EXPECT_NE(res, std::nullopt);
    if(res != std::nullopt){
        EXPECT_EQ(res->GetId(), 1);
        EXPECT_EQ(res->GetName(), user.GetName());
        EXPECT_EQ(res->GetAge(), user.GetAge());
    }

    res->SetName("qunqing");
    res->SetAge(20);

    EXPECT_EQ(repo.Update(*res, xsql::QueryWhere{}.Equal("id", 1)), 1);

    res = repo.GetFirstOrDefault(xsql::QueryWhere{}.Equal("id", 1));
    EXPECT_NE(res, std::nullopt);
    if(res != std::nullopt){
        EXPECT_EQ(res->GetId(), 1);
        EXPECT_EQ(res->GetName(), "qunqing");
        EXPECT_EQ(res->GetAge(), 20);
    }

    EXPECT_EQ(con->Define().DropTable("User").Execute(), true);
}