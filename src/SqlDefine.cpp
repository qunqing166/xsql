#include <SqlDefine.h>
#include <SqlExecutor.h>
#include <sstream>
#include <format>

namespace xsql{

SqlDefine::SqlDefine(SqlExecutor::ptr sql):
    m_sql(sql)
{
}

CreateTable xsql::SqlDefine::CreateTable(const std::string &table)
{
    return xsql::CreateTable(m_sql, table);
}

DropTable xsql::SqlDefine::DropTable(const std::string &table)
{
    return xsql::DropTable(m_sql, table);
}

CreateTable &CreateTable::Filed(const std::string &field, 
                                const std::string &type,
                                const std::string &desc,
                                int cons,
                                const std::string& defalut)
{
    m_tableStruct.emplace_back(field, type, defalut, desc, cons);
    return *this;
}

bool CreateTable::Execute()
{
    std::string cmd = GetString();
    return m_sql->Execute(cmd);
}

std::string CreateTable::GetString()
{
    std::stringstream ss;
    ss << "create table if not exists " << m_table << "(\n";
    auto begin = m_tableStruct.begin();
    if((begin->Constrain & Constraint::AUTO_INCREMENT && m_keys.count(begin->Filed)) == false){
        begin->Constrain &= ~Constraint::AUTO_INCREMENT;
    }

    FormatTableStruct(ss, *begin);
    while(++begin != m_tableStruct.end()){
        if((begin->Constrain & Constraint::AUTO_INCREMENT && m_keys.count(begin->Filed)) == false){
            begin->Constrain &= ~Constraint::AUTO_INCREMENT;
        }
        ss << ",\n";
        FormatTableStruct(ss, *begin);
    }

    for(auto& it : m_keys){
        ss << ",\n\t" << it.second;
    }

    ss << "\n);";

    return ss.str();
}

CreateTable &CreateTable::AddPrimaryKey(const std::string &field)
{
    m_keys.insert({field, std::format("primary key({})", field)});
    return *this;
}

// CreateTable &CreateTable::AddUniqueKey(const std::string &field, const std::string &name)
// {
//     m_keys.push_back(std::format("unique key {} ({})", name, field));
//     return *this;
// }

// CreateTable & CreateTable::AddKey(const std::string & field, const std::string & name)
// {
//     m_keys.push_back(std::format("key {} ({})", name, field));
//     return *this;
// }

void CreateTable::FormatTableStruct(std::stringstream &ss, const CreateTable::FieldDescription &des)
{
    ss  << '\t'
        << des.Filed << ' '
        << des.Type;
    if(des.Constrain & Constraint::NOT_NULL)ss << " not null";
    if(des.Constrain & Constraint::IS_UNIQUE)ss << " unique";
    if(des.Constrain & Constraint::AUTO_INCREMENT)ss << " auto_increment";
    if(des.Constrain & Constraint::AUTO_INCREMENT == 0 && des.Default.empty() == false)ss << " default " << des.Default;
    if(des.Description.empty() == false)ss << " comment '" << des.Description << "'";
}

CreateTable::CreateTable(SqlExecutor::ptr sql, const std::string &table):
    m_sql(sql), m_table(table)
{
}

bool DropTable::Execute()
{
    return m_sql->Execute(std::format("drop table {};", m_table));
}

DropTable::DropTable(SqlExecutor::ptr sql, const std::string &table):
    m_sql(sql), m_table(table)
{
}

}
