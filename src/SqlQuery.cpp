#include "SqlQuery.h"
#include <mysql/mysql.h>
#include <stdexcept>
#include <string>

namespace x{

std::string SelectQuery::FormatFields(const std::vector<std::string>& fields){
    std::string ret;
    auto begin = fields.begin();
    ret = *begin;
    while(++begin != fields.end()){
        ret += ',' + *begin;
    }
    return ret;
}

SqlQuery::SqlQuery(MYSQL* sql):
    m_sql(sql){
}


void SelectQuery::GetTableFiledMeta(const std::string& table){
    if(mysql_query(m_sql, ("describe " + table).c_str())){
        throw std::logic_error(
                Format("describe table: {} error, {}", table, mysql_error(m_sql))
        );
    }

    m_metas[table] = FieldMeta();
    FieldMeta& meta = m_metas[table];
    auto res = mysql_store_result(m_sql);
    if(res == nullptr){
        throw std::logic_error("store result error");
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))){
        meta.InsertType(row[0], row[1]);
    }
    mysql_free_result(res);
}

}
