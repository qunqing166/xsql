#pragma once
#include <string>
#include <map>
#include <vector>
#include <SqlExecutor.h>
#include <SqlType.h>

namespace xsql
{

class CreateTable;
class DropTable;

class SqlDefine
{
public:

    SqlDefine(SqlExecutor::ptr sql);

    xsql::CreateTable CreateTable(const std::string& table);
    xsql::DropTable DropTable(const std::string& table);

private:
    SqlExecutor::ptr m_sql;
};

class CreateTable
{
public:

    enum Constraint{
        NONE = 0,
        NOT_NULL = 1,
        AUTO_INCREMENT = 2,
        IS_UNIQUE = 4
    };
    
    template<typename T, std::size_t N = 0>
    CreateTable& Filed( const std::string &field, 
                        const std::string &desc = "",
                        int cons = Constraint::NONE,
                        const std::string& defalut = "")
    {
        m_tableStruct.emplace_back(field, TypeMapper<T, N>::value, defalut, desc, cons);
        return *this;
    }


    CreateTable& Filed( const std::string &field, 
                        const std::string &type,
                        const std::string &desc = "",
                        int cons = Constraint::NONE, 
                        const std::string& defalut = "");
    
    bool Execute();

    std::string GetString();

    CreateTable& AddPrimaryKey(const std::string& field);

    //
    // CreateTable& AddUniqueKey(const std::string& field, const std::string& name);
    // CreateTable& AddKey(const std::string& field, const std::string& name);
    // todo: 复合索引实现
    // CreateTable& AddComKey(const std::string& field, const std::string& name);


private:
    struct FieldDescription
    {
        std::string Filed;
        std::string Type;
        std::string Default;
        std::string Description;
        int Constrain;
    };

    static void FormatTableStruct(std::stringstream& ss, const CreateTable::FieldDescription& des);

    CreateTable(SqlExecutor::ptr sql, const std::string& table);

    friend class SqlDefine;
    SqlExecutor::ptr m_sql;
    std::string m_table;
    std::vector<FieldDescription> m_tableStruct;
    // std::vector<std::pair<std::string, std::string>> m_keys;
    std::map<std::string, std::string> m_keys;
};

class DropTable
{
public:

    bool Execute();

private:
    friend class SqlDefine;

    DropTable(SqlExecutor::ptr sql, const std::string& table);

    SqlExecutor::ptr m_sql;
    std::string m_table;

};

}
