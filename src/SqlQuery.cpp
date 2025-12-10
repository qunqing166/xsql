#include <SqlQuery.h>
#include <QueryResult.h>
#include <Format.h>
#include <cstdint>
#include <mysql/mysql.h>
#include <stdexcept>
#include <string>

namespace xsql{

/**************************************************/

SqlQuery::SqlQuery(MYSQL* sql):
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


SelectQuery::SelectQuery(MYSQL* sql, 
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
    m_orderby = XSqlFormat("order by {} {}", field, str);
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

QueryResult SelectQuery::Execute()
{
    std::string sub_where;
    std::string sub_orderby;
    std::string sub_limit;
    std::string sub_offset;

    if(sub_where.empty() == false)sub_where = XSqlFormat("where {}", m_where);
    if(m_orderby.empty() == false)sub_orderby = XSqlFormat("order by {}", m_orderby);
    if(m_limit > 0)sub_limit = XSqlFormat("limit {}", m_limit);
    if(m_offset > 0)sub_offset = XSqlFormat("offset {}", m_offset);
    std::string query = XSqlFormat("select {} from {} {} {} {} {}", m_select, m_table, sub_where, m_orderby, sub_limit, sub_offset);
    std::cout << query << '\n';
    if(mysql_query(m_sql, query.c_str())){
        throw std::logic_error(XSqlFormat("query error: {}", mysql_error(m_sql)));
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
        m_fields += XSqlFormat(",{}", begin->first);
        m_values += XSqlFormat(",{}", begin->second.Get());
    }
}

int InsertQuery::Execute()
{
    std::string query = XSqlFormat("insert into {} ({}) values ({})", m_table, m_fields, m_values);
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(XSqlFormat("insert: mysql_query error, {}", mysql_error(m_sql)));
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
    m_fieldAndValue = XSqlFormat("{}={}", begin->first, begin->second.Get());
    while(++begin != fieldAndValue.end())
    {
        m_fieldAndValue += XSqlFormat(",{}={}", begin->first, begin->second.Get());
    }
}

int UpdateQuery::Execute()
{
    if(m_where.empty() == false)m_where = XSqlFormat("where {}", m_where);
    std::string query = XSqlFormat("update {} set {} {}", m_table, m_fieldAndValue, m_where);
    std::cout << query << '\n';
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(XSqlFormat("update: mysql_query error, {}", mysql_error(m_sql)));
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
    if(m_where.empty() == false)m_where = XSqlFormat("where {}", m_where);
    std::string query = XSqlFormat("delete from {} {}", m_table, m_where);
    if(mysql_query(m_sql, query.c_str()))
    {
        throw std::logic_error(XSqlFormat("delete: mysql_query error, {}", mysql_error(m_sql)));
    }
    
    int val = mysql_affected_rows(m_sql);
    if(val == UINT64_MAX)return -1;
    return val;
}



}
