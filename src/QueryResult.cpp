#include "mysql.h"
#include <QueryResult.h>
#include <stdexcept>

namespace x{

QueryResult::QueryResult(MYSQL *sql):
    m_sql(sql){
    m_res = mysql_store_result(sql);
    if(m_res == nullptr){
        throw std::logic_error("mysql get result error");
    }
    GetFiledName();
}

QueryResult::~QueryResult(){
    mysql_free_result(m_res);
}


QueryResultRow QueryResult::GetRow(){
    return QueryResultRow(mysql_fetch_row(m_res), m_fieldName);
}

void QueryResult::GetFiledName(){
    MYSQL_FIELD* field;
    int index = 0;
    while((field = mysql_fetch_field(m_res))){
        std::cout << field->name << '\n';
        m_fieldName[field->name] = index++;
    }
}

QueryResultRow::QueryResultRow(MYSQL_ROW row, const std::unordered_map<std::string, int>& fieldName):
    m_row(row), m_fieldName(fieldName){
    
}

}
