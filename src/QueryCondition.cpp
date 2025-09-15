#include "QueryCondition.h"
#include "SqlValue.h"

namespace x{

void QueryWhere::CompareDetail( const std::string& filed, const SqlInputValue& value, 
                                const std::string op){
    if(m_where.empty() == false)m_where.append(" and ");
    m_where.append(filed + op + value.Get());
}

QueryWhere& QueryWhere::Equal(  const std::string& field, 
                                const SqlInputValue& value){
    CompareDetail(field, value, "=");
    return *this;
}

QueryWhere& QueryWhere::Greater(const std::string& field, 
                                const SqlInputValue& value){
    CompareDetail(field, value, ">");
    return *this;
}

QueryWhere& QueryWhere::Less(   const std::string& field, 
                                const SqlInputValue& value){
    CompareDetail(field, value, "<");
    return *this;
}

QueryWhere& QueryWhere::LessOrEqual(const std::string& field, 
                                    const SqlInputValue& value){
    CompareDetail(field, value, "<=");
    return *this;
}

QueryWhere& QueryWhere::GreaterOrEqual( const std::string& field, 
                                        const SqlInputValue& value){
    CompareDetail(field, value, ">=");
    return *this;
}

QueryWhere& QueryWhere::NotEqual(   const std::string& field, 
                                    const SqlInputValue& value){
    CompareDetail(field, value, "!=");
    return *this;
}
 
QueryWhere& QueryWhere::Between(const std::string& first, const std::string& second){
    return *this;
}

QueryWhere& QueryWhere::In(const std::vector<std::string>& values){
    return *this;
}

QueryWhere& QueryWhere::NotIn(const std::vector<std::string>& values){
    return *this;
}

QueryWhere& QueryWhere::Like(const std::string& pattern){
    return *this;
}
QueryWhere& QueryWhere::IsNull(const std::string& field){
    return *this;
}

QueryWhere& QueryWhere::IsNotNull(const std::string& field){
    return *this;
}

}

