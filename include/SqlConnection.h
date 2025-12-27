#pragma once
#include <SqlExecutor.h>
#include <SqlQuery.h>
#include <SqlDefine.h>

namespace xsql{

class SqlConnection
{
public:
    SqlConnection() = default;
    virtual ~SqlConnection() = default;

    SqlQuery Query();
    SqlDefine Define();

protected:
    
    virtual SqlExecutor::ptr GetSql() = 0;
    virtual void ReleaseSql(SqlExecutor::ptr sql) = 0;
};

}
