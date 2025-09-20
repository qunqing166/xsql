#include "SqlQuery.h"
#include "QueryResult.h"
#include "xformat.h"
#include <cstdint>
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


InsertQuery SqlQuery::Insert(const std::string& table, const std::vector<FieldAndValue>& fieldAndValue)
{
    return InsertQuery(m_sql, table, fieldAndValue);
}


DeleteQuery SqlQuery::Delete(const std::string& table)
{
    return DeleteQuery(m_sql, table);
}


UpdateQuery SqlQuery::Update(const std::string& table, const std::vector<FieldAndValue>& fieldAndValue)
{
    return  UpdateQuery(m_sql, table, fieldAndValue);
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



InsertQuery::InsertQuery(MYSQL* sql, const std::string& table, const std::vector<std::pair<std::string, SqlInputValue>>& fieldAndValues):
    m_sql(sql), m_table(table)    
{
    auto begin = fieldAndValues.begin();
    if(begin == fieldAndValues.end())
    {
        throw std::logic_error("insert: filed and value can not be empty");
    }

    m_fields = begin->first;
    m_values = begin->second.Get();

    while(++begin != fieldAndValues.end())
    {
        m_fields += Format(",{}", begin->first);
        m_values += Format(",{}", begin->second.Get());
    }
}

int InsertQuery::Execute()
{
    std::string query = Format("insert into {} ({}) values ({})", m_table, m_fields, m_values);
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(Format("insert: mysql_query error, {}", mysql_error(m_sql)));
    }
    
    int val = mysql_affected_rows(m_sql);
    if(val == UINT64_MAX)return -1;
    return val;
}


/******************************************************************************/


UpdateQuery::UpdateQuery(MYSQL* sql, const std::string& table, const std::vector<FieldAndValue>& fieldAndValue):
    m_sql(sql), m_table(table)
{
    auto begin = fieldAndValue.begin();
    if(begin == fieldAndValue.end())
    {
        throw std::logic_error("update: field and value can not be empty");
    }
    m_fieldAndValue = Format("{}={}", begin->first, begin->second.Get());
    while(++begin != fieldAndValue.end())
    {
        m_fieldAndValue += Format(",{}={}", begin->first, begin->second.Get());
    }
}

int UpdateQuery::Execute()
{
    if(m_where.empty() == false)m_where = Format("where {}", m_where);
    std::string query = Format("update {} set {} {}", m_table, m_fieldAndValue, m_where);
    std::cout << query << '\n';
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(Format("update: mysql_query error, {}", mysql_error(m_sql)));
    }
    
    int val = mysql_affected_rows(m_sql);
    if(val == UINT64_MAX)return -1;
    return val;
}

UpdateQuery& UpdateQuery::Where(const std::string& condition)
{
    m_where = condition;
    return *this;
}

UpdateQuery& UpdateQuery::Where(const QueryWhere& condition)
{
    m_where = condition.Get();
    return *this;
}

/*****************************************************************************/


DeleteQuery::DeleteQuery(MYSQL* sql, const std::string& table):
    m_sql(sql), m_table(table)
{
    
}

DeleteQuery& DeleteQuery::Where(const std::string& condition)
{
    m_where = condition;
    return *this;
}

DeleteQuery& DeleteQuery::Where(const QueryWhere& condition)
{
    m_where = condition.Get();
    return *this;
}

int DeleteQuery::Execute()
{
    if(m_where.empty() == false)m_where = Format("where {}", m_where);
    std::string query = Format("delete from {} {}", m_table, m_where);
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(Format("delete: mysql_query error, {}", mysql_error(m_sql)));
    }
    
    int val = mysql_affected_rows(m_sql);
    if(val == UINT64_MAX)return -1;
    return val;
}



}
