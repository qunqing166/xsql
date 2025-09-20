#pragma once
#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <QueryCondition.h>
#include "QueryResult.h"
#include "SqlValue.h"

namespace x
{

using FieldAndValue = std::pair<std::string, SqlInputValue>;

class SelectQuery;
class InsertQuery;
class UpdateQuery;
class DeleteQuery;


class SqlQuery
{
public:

    SqlQuery(MYSQL* sql);

    SelectQuery Select(const std::string table, const std::vector<std::string>& fields);
    InsertQuery Insert(const std::string& table, const std::vector<FieldAndValue>& fieldAndValue);
    UpdateQuery Update(const std::string& table, const std::vector<FieldAndValue>& fieldAndValue);
    DeleteQuery Delete(const std::string& table);
   
private:

    MYSQL* m_sql;

};


class SelectQuery
{
public:
    SelectQuery(MYSQL* sql, 
                const std::string& table, 
                const std::vector<std::string>& fields);

    SelectQuery& Where(const std::string& condition);
    SelectQuery& Where(const QueryWhere& condition);

    SelectQuery& OrderBy(const std::string& field);
    SelectQuery& Limit(int count);

    QueryResult Execute();

protected:

    void Generate();

private:

    std::string FormatFields(const std::vector<std::string>& fields);

private:
    MYSQL* m_sql;
    std::string m_table;
    std::string m_select;
    std::string m_where;
    std::string m_oderby;
};

class InsertQuery
{
public:

    int Execute();

private:

    InsertQuery(MYSQL* sql, const std::string& table, const std::vector<FieldAndValue>& filedAndValues);

private:
    friend class SqlQuery;

    MYSQL* m_sql;
    std::string m_table;
    std::string m_fields;
    std::string m_values;
};


/********************************************************************************/


class DeleteQuery
{
public:
    int Execute();

    DeleteQuery& Where(const std::string& condition);
    DeleteQuery& Where(const QueryWhere& condition);

private:

    DeleteQuery(MYSQL* sql, const std::string& table);

private:

    friend class SqlQuery;

    MYSQL* m_sql;
    std::string m_table;
    std::string m_where;
    
};



}
