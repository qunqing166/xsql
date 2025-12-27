#pragma once
#include <SqlExecutor.h>
#include <memory>
#include <mysql/mysql.h>

namespace xsql::mysql{

class MysqlExecutor: public SqlExecutor, std::enable_shared_from_this<MysqlExecutor>
{
public:
    using ptr = std::shared_ptr<MysqlExecutor>;

    MysqlExecutor(MYSQL* sql);
    ~MysqlExecutor();

    bool Execute(std::string_view cmd) override;
    std::shared_ptr<xsql::SqlResult> Result() override;
    int AffectedRowNum() override;

private:

    MYSQL* m_sql;

};


}
