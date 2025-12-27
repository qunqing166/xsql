#pragma once
#include <string>
#include <utils.h>
#include <memory>
#include <SqlExecutor.h>
#include <ValueConvert.h>

namespace xsql{

class SqlResult;

class SqlResultRow
{
public:
    using ptr = std::shared_ptr<SqlResultRow>;

    virtual ~SqlResultRow() = default;

    template<typename T>
    T Get(const std::string& field){
        return ConvertSqlValue<T>(GetValue(field));
    }

    bool operator==(bool val) noexcept{
        return IsEnd() == false;
    }

protected:

    virtual bool IsEnd() const = 0;
    virtual std::string GetValue(const std::string& field) = 0;

};


class SqlResult
{
public:
    using ptr = std::shared_ptr<SqlResult>;

    virtual ~SqlResult() = default;

    virtual SqlResultRow::ptr GetRow() = 0;
    virtual int RowCount() = 0;
};


}

