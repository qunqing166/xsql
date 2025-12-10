#include "SqlConnection.h"
#include "SqlQuery.h"
#include <mysql/mysql.h>
#include <stdexcept>

namespace xsql{


SqlConnection::SqlConnection(   const std::string& user, 
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
        m_sqls.push_back(sql);
    }
}

SqlQuery SqlConnection::Query(){
    return SqlQuery(m_sqls.front());   
}


}
