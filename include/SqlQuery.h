#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <mysql/mysql.h>
#include <QueryCondition.h>
#include "QueryResult.h"

namespace x
{

class SelectQuery;
class InsertQuery;
class UpdateQuery;
class DeleteQuery;


class SqlQuery
{
public:

    SqlQuery(MYSQL* sql);

    SelectQuery Select(const std::string table, const std::vector<std::string>& fields);
    InsertQuery Insert(const std::string& table, const std::vector<std::pair<std::string, std::string>>& fieldAndValue);
    UpdateQuery Update(const std::string& table, const std::vector<std::pair<std::string, std::string>>& fieldAndValue);
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


}
