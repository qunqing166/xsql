#include <iostream>
#include <string>
#include "QueryCondition.h"
#include "SqlConnection.h"
#include "SqlValue.h"
#include <mysql/mysql.h>
#include "SqlQuery.h"

void test_where();
void test_get_table_meta();
void test_prepare();
void test_sql_input();
void test_select();

int main()
{
    test_select();
    std::cout << "ciallo\n";
    return 0;
}

void test_select(){
    x::SqlConnection con("root", "114514", "test");
    auto query = con.Query();
    query.Select("test", {"*"}).Where(x::QueryWhere()
                                     .Greater("age", 1))
                                     .Execute();
    query.DisplayFields();
}
    
void test_sql_input(){
    std::cout << x::SqlInputValue(12).Get() << '\n';
    std::cout << x::SqlInputValue(3.14159).Get() << '\n';
    std::cout << x::SqlInputValue("sql").Get() << '\n';
    std::cout << x::SqlInputValue("insa'sd").Get() << '\n';
    
}

void test_prepare(){
     MYSQL* sql = mysql_init(nullptr);
    if(sql == nullptr){
        std::cout << "mysql init error\n";
    }
    if(mysql_real_connect(sql, "127.0.0.1", "root", "114514", "test", 0, NULL, 0) == nullptr){
        std::cout << "connect error" << mysql_error(sql) << '\n';
    }

    auto stmt = mysql_stmt_init(sql);
    if(stmt == nullptr){
        std::cout << "stmt init error\n";
        return;
    }
}

void test_get_table_meta(){
    MYSQL* sql = mysql_init(nullptr);
    if(sql == nullptr){
        std::cout << "mysql init error\n";
    }
    if(mysql_real_connect(sql, "127.0.0.1", "root", "114514", "test", 0, NULL, 0) == nullptr){
        std::cout << "connect error" << mysql_error(sql) << '\n';
    }

    const char* query = "describe test;";
    if(mysql_query(sql, query)){
        std::cout << "query error" << mysql_error(sql) <<  "\n";
    }
    auto res = mysql_store_result(sql);
    if(res == nullptr){
        std::cout << "res is null\n";
        return;
    }

    MYSQL_ROW row;
    int i = 0;
    while(row = mysql_fetch_row(res)){
        std::cout << "name:" << row[0] << '\n';
        std::cout << "type:" << row[1] << '\n';
    //  x::FieldMeta meta(row[0], row[1]);
    }
}

void test_where(){
    auto val = x::QueryWhere().Equal("id", "a").GreaterOrEqual("name", "ciallo'").NotEqual("a", 12).Less("sda", 22).Prepare();
    std::cout << val << "\n";
}


