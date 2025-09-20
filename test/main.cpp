#include <exception>
#include <iostream>
#include <string>
#include "QueryCondition.h"
#include "QueryResult.h"
#include "SqlConnection.h"
#include "SqlValue.h"
#include <mysql/mysql.h>
#include "SqlQuery.h"
#include "xformat.h"


int main()
{
    try{
        x::SqlConnection con("root", "114514", "test");
        x::QueryResult res = con.Query().Select("test", {"id", "name", "age"}).Execute();
        auto row = res.GetRow();
        std::cout << x::Format("{} {} {}\n", 
                        row.Get<int>("id"), 
                        row.Get<std::string>("name"),
                        row.Get<int>("age"));
    }catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    
    
    std::cout << "ciallo\n";
    return 0;
}


