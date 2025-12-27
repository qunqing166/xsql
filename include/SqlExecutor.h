#pragma once
#include <string_view>
#include <memory>

namespace xsql{

class SqlResult;

class SqlExecutor
{
public:
    using ptr = std::shared_ptr<SqlExecutor>;

    virtual ~SqlExecutor() = default;

    virtual bool Execute(std::string_view cmd) = 0;
    virtual std::shared_ptr<xsql::SqlResult> Result() = 0;
    virtual int AffectedRowNum() = 0;
};

}
