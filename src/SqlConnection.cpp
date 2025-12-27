#include <SqlConnection.h>

namespace xsql{

SqlQuery SqlConnection::Query(){
    return SqlQuery(GetSql());   
}

SqlDefine SqlConnection::Define()
{
    return SqlDefine(GetSql());
}

}
