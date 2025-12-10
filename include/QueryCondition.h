#pragma once
#include "SqlValue.h"
#include <string>
#include <utility>
#include <vector>
#include <list>

namespace xsql
{

class QueryWhere
{
public:
    QueryWhere& Equal(const std::string&, const SqlInputValue&);
    QueryWhere& Equal(const std::vector<std::pair<std::string, SqlInputValue>>&);

    QueryWhere& Greater(const std::string&, const SqlInputValue&);
    QueryWhere& Greater(const std::vector<std::pair<std::string, SqlInputValue>>&);

    QueryWhere& Less(const std::string&, const SqlInputValue&);
    QueryWhere& Less(const std::vector<std::pair<std::string, SqlInputValue>>&);
    
    QueryWhere& LessOrEqual(const std::string&, const SqlInputValue&);
    QueryWhere& LessOrEqual(const std::vector<std::pair<std::string, SqlInputValue>>&);
    
    QueryWhere& GreaterOrEqual(const std::string&, const SqlInputValue&);
    QueryWhere& GreaterOrEqual(const std::vector<std::pair<std::string, SqlInputValue>>&);
    
    QueryWhere& NotEqual(const std::string&, const SqlInputValue&);
    QueryWhere& NotEqual(const std::vector<std::pair<std::string, SqlInputValue>>&);
    
    QueryWhere& Between(const std::string& first, const std::string& second);
    QueryWhere& In(const std::vector<std::string>& values);
    QueryWhere& NotIn(const std::vector<std::string>& values);
    QueryWhere& Like(const std::string& pattern);
    QueryWhere& IsNull(const std::string& field);
    QueryWhere& IsNotNull(const std::string& field);

    const std::list<std::string>& Values() const{
        return m_values;
    }

    const std::string& Get() const{
        return m_where;
    }

    const std::string& Prepare() const{
        return m_where;
    }

private:
    void CompareDetail( const std::string& field, const SqlInputValue& value, 
                        const std::string op);
    
private:
    std::string m_where;
    std::list<std::string> m_values;
    // int offset = 0;
};

}
