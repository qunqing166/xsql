#include "SqlQuery.h"
#include "QueryResult.h"
#include <mysql/mysql.h>
#include <stdexcept>
#include <string>

namespace x{

QueryBase::QueryBase(MYSQL* sql):
    m_sql(sql)
{
    
}

QueryResult QueryBase::Execute()
{
    Generate();
    if(mysql_query(m_sql, m_query.c_str())){
        throw std::logic_error(Format("query error: {}", mysql_error(m_sql)));
    }
    return QueryResult(m_sql);
}

SelectQuery::SelectQuery(MYSQL* sql, 
            const std::string& table, 
            const std::vector<std::string>& fields):
    QueryBase(sql),
    m_table(table)
{
    m_select = FormatFields(fields);
}

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

void SelectQuery::Generate(){
    std::string sub_where;
    if(sub_where.empty() == false)sub_where = Format("where {}", m_where);
    m_query = x::Format("select {} from {} {}", m_select, m_table, sub_where);
    std::cout << m_query << '\n';
}

}
