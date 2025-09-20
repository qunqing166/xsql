#pragma once
#include <string>
#include <strstream>
#include <unordered_map>
#include <mysql/mysql.h>
#include <utils.h>

namespace x{

class QueryResult;

class QueryResultRow
{
public:

    template<typename T>
    T Get(const std::string& field){
        return ConvertSqlValue<T>(std::string(m_row[m_fieldName.at(field)]));
    }

private:
    QueryResultRow(MYSQL_ROW row, const std::unordered_map<std::string, int>& fieldName);

private:
    friend class QueryResult;

    MYSQL_ROW m_row;
    const std::unordered_map<std::string, int>& m_fieldName;
};


class QueryResult
{
public:
    QueryResult(MYSQL *sql);
    ~QueryResult();

    QueryResultRow GetRow();

private:

    void GetFiledName();

private:
    MYSQL* m_sql;
    MYSQL_RES* m_res;
    std::unordered_map<std::string, int> m_fieldName;
    std::unordered_map<std::string, const char*> m_values;
};


}

