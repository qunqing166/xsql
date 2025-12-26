#include "mysql.h"
#include <SqlResult.h>
#include <memory>
#include <stdexcept>

namespace xsql{

SqlResult::SqlResult(MYSQL *sql):
    m_sql(sql)
{
    m_res = mysql_store_result(sql);
    if(m_res == nullptr){
        throw std::logic_error("mysql get result error");
    }
    GetFieldIndex();
}

SqlResult::~SqlResult(){
    mysql_free_result(m_res);
}


SqlResultRow SqlResult::GetRow(){
    return SqlResultRow(mysql_fetch_row(m_res), m_fieldIndex);
}

int SqlResult::RowCount()
{
    return mysql_num_rows(m_res);
}

void SqlResult::GetFieldIndex(){
    m_fieldIndex = std::make_shared<FieldIndex>();
    MYSQL_FIELD* field;
    int index = 0;
    while((field = mysql_fetch_field(m_res))){
        // std::cout << field->name << '\n';
        m_fieldIndex->insert({std::string(field->name), index++});
    }
}

SqlResultRow::SqlResultRow(MYSQL_ROW row, std::shared_ptr<FieldIndex> fieldIndex):
    m_row(row), m_fieldIndex(fieldIndex)
{
    
}

}
