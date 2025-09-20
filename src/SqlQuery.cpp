#include "SqlQuery.h"
#include "QueryResult.h"
#include <mysql/mysql.h>
#include <stdexcept>
#include <string>

namespace x{

/**************************************************/

SqlQuery::SqlQuery(MYSQL* sql):
    m_sql(sql)
{

}


SelectQuery::SelectQuery(MYSQL* sql, 
            const std::string& table, 
            const std::vector<std::string>& fields):
    m_sql(sql),
    m_table(table)
{
    m_select = FormatFields(fields);
}


SelectQuery SqlQuery::Select(const std::string table, const std::vector<std::string>& fields)
{
    return SelectQuery(m_sql, table, fields);
}


/***************************************************/



SelectQuery& SelectQuery::Where(const QueryWhere& condition){
    m_where = condition.Get();
    return *this;
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


QueryResult SelectQuery::Execute()
{
    std::string sub_where;
    if(sub_where.empty() == false)sub_where = Format("where {}", m_where);
    std::string query = x::Format("select {} from {} {}", m_select, m_table, sub_where);
    std::cout << query << '\n';
    if(mysql_query(m_sql, query.c_str())){
        throw std::logic_error(Format("query error: {}", mysql_error(m_sql)));
    }
    return QueryResult(m_sql);
}
/******************************************************/
















}
