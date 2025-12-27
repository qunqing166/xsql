#include <memory>
#include <mysql/MysqlConnection.h>
#include <mysql/MysqlExecutor.h>

namespace xsql::mysql{


MysqlConnection::MysqlConnection(   const std::string& user, 
                                    const std::string& passwd, 
                                    const std::string& dbName, 
                                    const std::string& host, 
                                    uint16_t port, 
                                    int conCount):
    m_user(user), m_passwd(passwd), m_dbName(dbName),
    m_host(host), m_port(port), m_conCount(conCount)
{
    for(int i = 0; i < conCount; ++i){
        MYSQL* sql = mysql_init(nullptr);
        if(sql == nullptr){
            throw std::logic_error("sql init error");
        }
        if(mysql_real_connect(  sql, 
                                host.c_str(), 
                                user.c_str(), 
                                passwd.c_str(), 
                                dbName.c_str(),
                                port, nullptr, 0) == nullptr){
            throw std::logic_error("sql connect error");
        }
        m_sqls.push_back(std::make_shared<MysqlExecutor>(sql));
    }
}

std::shared_ptr<MysqlConnection> MysqlConnection::Create( const std::string &user, 
                                                        const std::string &passwd, 
                                                        const std::string &dbName, 
                                                        const std::string &host, 
                                                        uint16_t port, 
                                                        int conCount)
{
    return std::make_shared<MysqlConnection>(user, passwd, dbName, host, port, conCount);
}

SqlExecutor::ptr MysqlConnection::GetSql()
{
    auto sql = m_sqls.front();
    m_sqls.pop_front();
    return sql;
}

void MysqlConnection::ReleaseSql(SqlExecutor::ptr sql)
{
    m_sqls.push_back(sql);
}



}
