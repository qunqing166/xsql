#include "Debug.h"
#include "SqlResult.h"
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <mysql/MysqlExecutor.h>
#include <mysql/mysql.h>

namespace xsql::mysql{


using FieldIndex = std::unordered_map<std::string, int>;

class MysqlResultRow: public SqlResultRow
{
public:
    MysqlResultRow(MYSQL_ROW row, std::shared_ptr<FieldIndex> filedIndex):
        m_row(row), m_fieldIndex(filedIndex)
    {}

protected:

    bool IsEnd() const override
    {
        return m_row == nullptr;
    }

    std::string GetValue(const std::string& field) override
    {
        return std::string(m_row[m_fieldIndex->at(field)]);
    }

private:

    MYSQL_ROW m_row;
    std::shared_ptr<FieldIndex> m_fieldIndex;
};

class MysqlResult: public SqlResult
{
public:
    MysqlResult(MYSQL_RES* res):
        m_res(res)
    {
        m_fieldIndex = std::make_shared<FieldIndex>();
        MYSQL_FIELD* field;
        int index = 0;
        while((field = mysql_fetch_field(m_res))){
            m_fieldIndex->insert({std::string(field->name), index++});
        }
    }
    
    SqlResultRow::ptr GetRow() override
    {
        auto row = mysql_fetch_row(m_res);
        if(row == nullptr)return nullptr;
        return std::make_shared<MysqlResultRow>(row, m_fieldIndex);
    }

    int RowCount() override
    {
        return mysql_num_rows(m_res);
    }

private:
    // MYSQL* m_sql;
    MYSQL_RES* m_res;

    std::shared_ptr<FieldIndex> m_fieldIndex;
};

MysqlExecutor::MysqlExecutor(MYSQL* sql):
    m_sql(sql)
{

}

MysqlExecutor::~MysqlExecutor()
{
//    mysql_close(m_sql);
}

bool MysqlExecutor::Execute(std::string_view cmd)
{
    bool res = mysql_query(m_sql, cmd.data()) == 0;
    if(res == false){
        XSQL_DEBUG("query error: {}", mysql_error(m_sql));
    }
    return res;
}

std::shared_ptr<xsql::SqlResult> MysqlExecutor::Result()
{
    auto res = mysql_store_result(m_sql);
    return std::make_shared<MysqlResult>(res);
}

int MysqlExecutor::AffectedRowNum()
{
    auto count = mysql_affected_rows(m_sql);
    if(count == INT64_MAX)count = 0;
    return count;
}

}
