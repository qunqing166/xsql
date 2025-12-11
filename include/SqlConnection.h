#pragma once
#include <cstdint>
#include <mysql/mysql.h>
#include <string>
#include "SqlQuery.h"
#include <SqlDefine.h>

namespace xsql{

class SqlConnection
{
public:
    SqlConnection(  const std::string& user, 
                    const std::string& passwd, 
                    const std::string& dbName, 
                    const std::string& host = "localhost", 
                    uint16_t port = 0, 
                    int conCount = 1);

    static std::shared_ptr<SqlConnection> Create(   const std::string& user, 
                                                    const std::string& passwd, 
                                                    const std::string& dbName, 
                                                    const std::string& host = "localhost", 
                                                    uint16_t port = 0, 
                                                    int conCount = 1);

    SqlQuery Query();
    SqlDefine Define();

private:
    std::list<MYSQL*> m_sqls;
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    uint16_t m_port;
    int m_conCount;
};

}
