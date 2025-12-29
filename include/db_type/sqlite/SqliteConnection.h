#pragma once
#include "SqlExecutor.h"
#include <SqlConnection.h>
namespace xsql::sqlite{

class SqliteConnection: public xsql::SqlConnection
{
public:
    
    SqliteConnection(const std::string& db);
    ~SqliteConnection();

protected:
    
    virtual SqlExecutor::ptr GetSql() override;
    virtual void ReleaseSql(SqlExecutor::ptr sql) override;

private:
    SqlExecutor::ptr m_sql;
    std::string m_db;
};

}
