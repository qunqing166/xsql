#include <any>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include "QueryCondition.h"
#include "QueryResult.h"
#include "SqlConnection.h"
#include <mysql/mysql.h>
#include "SqlQuery.h"
#include "xformat.h"
#include <TableBase.h>
#include <SqlType.h>
#include <CommonRepository.h>

class People: public TableBase<People>
{
    FIELD_REGISTER(int, Id);
    FIELD_REGISTER(std::string, Name);
    FIELD_REGISTER(int, Age);
};

void test0();
void test1();
void test2();
void test3();

int main()
{
    test3();
    return 0;
}

void test3()
{
class test:public TableBase<test>
{
    FIELD_REGISTER(int, Id);
    FIELD_REGISTER(std::string, Name);
    FIELD_REGISTER(int, Age);
};


    try{
        std::shared_ptr<x::SqlConnection> con = std::make_shared<x::SqlConnection>("root", "114514", "test");
        x::CommonRepository<test> repo(con);
        //auto data = repo.GetFirstOrDefault();
        auto datas = repo.GetAll();

        for(auto& it: datas)
        {
            std::cout   << "id: "   << it.GetId() << ' '
                        << "name: " << it.GetName() << ' '
                        << "age: "  << it.GetAge() << '\n';
        }
    }catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
}

void test2()
{
    x::DateTime dt = x::DateTime::Now();
    std::cout << dt.ToString() << "\n";
}

void test1()
{
    // People p;
    // p.Set("Id", 10);
    // p.Set("Name", "juanzhi");
    // p.Set("Age", 18);
    //
    // std::cout   << "id:" << std::any_cast<int>(p.Get("Id"))
    //             << " name:" << std::any_cast<std::string>(p.Get("Name"))
    //             << " age:" << std::any_cast<int>(p.Get("Age")) << '\n';
    //
}

void test0()
{
    try{
        x::SqlConnection con("root", "114514", "test");

        /*
        int val = con.Query().Insert("test", {  {"id", "3"},
                                                {"name", "none"},
                                                {"age", 14}}
                                    ).Execute();
        if(val > 0){
            std::cout << "inset ok\n";
        }else{
            std::cout << "error";
        }
        */

        // int val = con.Query().Delete("test").Where(x::QueryWhere().Equal("name", "none")).Execute();
       x::QueryResult res = con.Query().Select("test", {"id", "name", "age"})
                                        .OrderBy("age")
                                        .Limit(2)
                                        .Offset(1)
                                        .Execute();
        while(auto row = res.GetRow())
        {
            std::cout << x::Format("{} {} {}\n", 
                        row.Get<int>("id"), 
                        row.Get<std::string>("name"),
                        row.Get<int>("age"));
        }
        // std::cout << (row == true);
    }catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }


    std::cout << "ciallo\n";
}
