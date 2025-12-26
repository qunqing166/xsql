#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <mysql/mysql.h>
#include <utils.h>
#include <memory>

namespace xsql{


using FieldIndex = std::unordered_map<std::string, int>;

class SqlResult;

class SqlResultRow
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

    SqlResultRow(MYSQL_ROW row, std::shared_ptr<FieldIndex> fieldIndex);

private:
    friend class SqlResult;

    MYSQL_ROW m_row;
    std::shared_ptr<FieldIndex> m_fieldIndex;
};


class SqlResult
{
public:
    SqlResult(MYSQL *sql);
    ~SqlResult();

    SqlResultRow GetRow();
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

