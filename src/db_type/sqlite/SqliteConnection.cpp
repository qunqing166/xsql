#include "SqlExecutor.h"
#include "SqlResult.h"
#include <sqlite/SqliteConnection.h>
#include <sqlite3.h>
#include <stdexcept>

namespace xsql::sqlite{

class SqliteResultRow: public SqlResultRow
{
public:
    
    SqliteResultRow(sqlite3_stmt* stmt, std::shared_ptr<std::map<std::string_view, int>> fieldAndIndex):
        m_stmt(stmt), m_fieldAndIndex(fieldAndIndex)
    {

    }

protected:

    bool IsEnd() const override{return false;}
    std::string GetValue(const std::string& field) override
    {
        return (char*)sqlite3_column_text(m_stmt, m_fieldAndIndex->at(field));
    }

private:
    sqlite3_stmt* m_stmt;
    std::shared_ptr<std::map<std::string_view, int>> m_fieldAndIndex;
};

class SqliteResult: public SqlResult
{
public:
    SqliteResult(sqlite3_stmt* stmt):
        m_stmt(stmt)
    {
        auto colCount = sqlite3_column_count(m_stmt);
        for(int i = 0; i < colCount; ++i){
            m_filedAndIndex->insert({sqlite3_column_name(m_stmt, i), i});
        }

        while(sqlite3_step(m_stmt) == SQLITE_ROW)++m_rowCount;
        sqlite3_reset(m_stmt);
    }

    SqlResultRow::ptr GetRow() override{return nullptr;}
    int RowCount() override
    {
        
        return 0;
    }

private:
    int m_rowCount;
    sqlite3_stmt* m_stmt;
    std::shared_ptr<std::map<std::string_view, int>> m_filedAndIndex;
};

class SqliteExecutor: public SqlExecutor
{
public:
    SqliteExecutor(const std::string& db)
    {
        auto res = sqlite3_open(db.c_str(), &m_sql);
        if(res != SQLITE_OK){
            throw std::runtime_error("sqlite open error");
        }
    }

    bool Execute(std::string_view cmd) override
    {
        auto res = sqlite3_prepare_v2(m_sql, cmd.data(), cmd.size(), &m_stmt, nullptr);
        
        if(res != SQLITE_OK){
            throw std::runtime_error(sqlite3_errmsg(m_sql));
            return false;
        }else return true;
    }

    std::shared_ptr<xsql::SqlResult> Result() override
    {
        return std::make_shared<SqliteResult>(m_stmt);
    }
    int AffectedRowNum() override{return 0;}

private:
    sqlite3_stmt* m_stmt;
    sqlite3* m_sql;

};

SqliteConnection::SqliteConnection(const std::string& db):
    m_db(db)
{
    m_sql = std::make_shared<SqliteExecutor>(db);
}

SqliteConnection::~SqliteConnection()
{

}

SqlExecutor::ptr SqliteConnection::GetSql()
{
    return m_sql;   
}

void SqliteConnection::ReleaseSql(SqlExecutor::ptr sql)
{

}


}
