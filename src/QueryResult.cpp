#include "mysql.h"
#include <QueryResult.h>
#include <memory>
#include <stdexcept>

namespace x{

QueryResult::QueryResult(MYSQL *sql):
    m_sql(sql)
{
    m_res = mysql_store_result(sql);
    if(m_res == nullptr){
        throw std::logic_error("mysql get result error");
    }
    GetFieldIndex();
}

QueryResult::~QueryResult(){
    mysql_free_result(m_res);
}


QueryResultRow QueryResult::GetRow(){
    return QueryResultRow(mysql_fetch_row(m_res), m_fieldIndex);
}

void QueryResult::GetFieldIndex(){
    m_fieldIndex = std::make_shared<FieldIndex>();
    MYSQL_FIELD* field;
    int index = 0;
    while((field = mysql_fetch_field(m_res))){
        std::cout << field->name << '\n';
        m_fieldIndex->insert({std::string(field->name), index++});
    }
}

QueryResultRow::QueryResultRow(MYSQL_ROW row, std::shared_ptr<FieldIndex> fieldIndex):
    m_row(row), m_fieldIndex(fieldIndex)
{
    
}

}
