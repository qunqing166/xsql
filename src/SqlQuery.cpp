#include "SqlExecutor.h"
#include <SqlQuery.h>
#include <SqlResult.h>
#include <format>
#include <stdexcept>
#include <string>
#include <Debug.h>

namespace xsql{

/**************************************************/

SqlQuery::SqlQuery(SqlExecutor::ptr sql):
    m_sql(sql)
{

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


SelectQuery::SelectQuery(SqlExecutor::ptr sql, 
            const std::string& table, 
            const std::vector<std::string>& fields):
    m_sql(sql),
    m_table(table)
{
    auto begin = fields.begin();
    if(begin == fields.end())
    {
        throw std::logic_error("select: field can not be empty");
    }
    m_select = *begin;
    while(++begin != fields.end()){
        m_select += ',' + *begin;
    }
}



SelectQuery& SelectQuery::Where(const QueryWhere& condition){
    m_where = condition.Get();
    return *this;
}

SelectQuery& SelectQuery::OrderBy(const std::string& field, char order)
{
    std::string str;
    if(order == 'd')str = "desc";
    else if(order == 'a')str = "asc";
    m_orderby = std::format("order by {} {}", field, str);
    return *this;
} 

SelectQuery& SelectQuery::Limit(int count)
{
    m_limit = count;
    return *this;
}

SelectQuery& SelectQuery::Offset(int count)
{
    m_offset = count;
    return *this;
}

SqlResult::ptr SelectQuery::Execute()
{
    std::string sub_where;
    std::string sub_orderby;
    std::string sub_limit;
    std::string sub_offset;

    if(sub_where.empty() == false)sub_where = std::format("where {}", m_where);
    if(m_orderby.empty() == false)sub_orderby = std::format("order by {}", m_orderby);
    if(m_limit > 0)sub_limit = std::format("limit {}", m_limit);
    if(m_offset > 0)sub_offset = std::format("offset {}", m_offset);
    std::string query = std::format("select {} from {} {} {} {} {}", m_select, m_table, sub_where, m_orderby, sub_limit, sub_offset);
    if(m_sql->Execute(query) == false){
        throw std::logic_error("query error: {}");
    }
    return m_sql->Result();
}
/******************************************************/



InsertQuery::InsertQuery(SqlExecutor::ptr sql, const std::string& table, const std::vector<std::pair<std::string, SqlInputValue>>& fieldAndValues):
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
        m_fields += std::format(",{}", begin->first);
        m_values += std::format(",{}", begin->second.Get());
    }
}

int InsertQuery::Execute()
{
    std::string query = std::format("insert into {} ({}) values ({})", m_table, m_fields, m_values);
    XSQL_DEBUG("insert query: {}", query);
    if(m_sql->Execute(query) == false)
    {
        throw std::logic_error("insert: mysql_query error");
    }
    return m_sql->AffectedRowNum();   
}


/******************************************************************************/


UpdateQuery::UpdateQuery(SqlExecutor::ptr sql, const std::string& table, const std::vector<FieldAndValue>& fieldAndValue):
    m_sql(sql), m_table(table)
{
    auto begin = fieldAndValue.begin();
    if(begin == fieldAndValue.end())
    {
        throw std::logic_error("update: field and value can not be empty");
    }
    m_fieldAndValue = std::format("{}={}", begin->first, begin->second.Get());
    while(++begin != fieldAndValue.end())
    {
        m_fieldAndValue += std::format(",{}={}", begin->first, begin->second.Get());
    }
}

int UpdateQuery::Execute()
{
    if(m_where.empty() == false)m_where = std::format("where {}", m_where);
    std::string query = std::format("update {} set {} {}", m_table, m_fieldAndValue, m_where);
    
    XSQL_DEBUG("update query: {}", query);

    if(m_sql->Execute(query) == false)
    {
        throw std::logic_error("update: mysql_query error");
    }
    return m_sql->AffectedRowNum();
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


DeleteQuery::DeleteQuery(SqlExecutor::ptr sql, const std::string& table):
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
    if(m_where.empty() == false)m_where = std::format("where {}", m_where);
    std::string query = std::format("delete from {} {}", m_table, m_where);

    if(m_sql->Execute(query) == false)
    {
        throw std::logic_error("delete: mysql_query error");
    }
    return m_sql->AffectedRowNum();    
}

}
