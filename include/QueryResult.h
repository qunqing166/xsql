#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <mysql/mysql.h>
#include <utils.h>
#include <memory>

namespace xsql{


using FieldIndex = std::unordered_map<std::string, int>;

class QueryResult;

class QueryResultRow
{
public:

    template<typename T>
    T Get(const std::string& field){
        if(m_fieldIndex == nullptr){
            throw std::logic_error("invalid row");
        }
        return ConvertSqlValue<T>(std::string(m_row[m_fieldIndex->at(field)]));
    }

    operator bool() const noexcept{
        return m_row != nullptr;
    }

    bool operator==(bool val) noexcept{
        return m_row != nullptr;
    }

private:

    QueryResultRow(MYSQL_ROW row, std::shared_ptr<FieldIndex> fieldIndex);

private:
    friend class QueryResult;

    MYSQL_ROW m_row;
    std::shared_ptr<FieldIndex> m_fieldIndex;
};


class QueryResult
{
public:
    QueryResult(MYSQL *sql);
    ~QueryResult();

    QueryResultRow GetRow();
    int RowCount();

private:
    
    /**
     * @brief 获取被查询的字段的索引
     */
    void GetFieldIndex();

private:
    MYSQL* m_sql;
    MYSQL_RES* m_res;
    std::shared_ptr<FieldIndex> m_fieldIndex;
    std::unordered_map<std::string, const char*> m_values;
};


}

