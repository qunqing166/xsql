#include <exception>
#include <iostream>
#include <string>
#include "QueryCondition.h"
#include "QueryResult.h"
#include "SqlConnection.h"
#include <mysql/mysql.h>
#include "SqlQuery.h"
#include "xformat.h"


int main()
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
    return 0;
}


