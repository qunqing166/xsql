#pragma once
#include <SqlConnection.h>
#include <list>

namespace xsql::mysql{

class MysqlConnection: public xsql::SqlConnection
{
public:
    MysqlConnection(const std::string& user, 
                    const std::string& passwd, 
                    const std::string& dbName, 
                    const std::string& host = "localhost", 
                    uint16_t port = 0, 
                    int conCount = 1);

    static std::shared_ptr<MysqlConnection> Create( const std::string& user, 
                                                    const std::string& passwd, 
                                                    const std::string& dbName, 
                                                    const std::string& host = "localhost", 
                                                    uint16_t port = 0, 
                                                    int conCount = 10);
    
    SqlExecutor::ptr GetSql() override;
    void ReleaseSql(SqlExecutor::ptr sql) override;


private:

    std::list<std::shared_ptr<SqlExecutor>> m_sqls;
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    uint16_t m_port;
    int m_conCount;


};


}
