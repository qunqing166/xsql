#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <mysql/mysql.h>
#include <QueryCondition.h>
#include "xformat.h"
#include "QueryResult.h"

namespace x
{

class QueryBase
{
public:
    QueryBase(MYSQL* sql):
        m_sql(sql){
    
    }

    QueryResult Execute(){
        Generate();
        std::cout << m_query << "\n";
        if(mysql_query(m_sql, m_query.c_str())){
            throw std::logic_error(Format("query error: {}", mysql_error(m_sql)));
        }
        
        /*
        auto res = mysql_store_result(m_sql);
        if(res == nullptr){
            throw std::logic_error(Format("store_result error: {}", mysql_error(m_sql)));
        }
        */
        return QueryResult(m_sql);
    }
protected:
    virtual void Generate(){}

    MYSQL* m_sql;
    std::string m_query;
};

class SelectQuery: public QueryBase
{
public:
    SelectQuery(MYSQL* sql, 
                std::unordered_map<std::string, FieldMeta>& metas, 
                const std::string& table, 
                const std::vector<std::string>& fields):
        QueryBase(sql),
        m_metas(metas),
        m_table(table){
        m_select = FormatFields(fields);
        GetTableFiledMeta(table);   
    }

    SelectQuery& Where(const std::string& condition);
    SelectQuery& Where(const QueryWhere& condition){
        m_where = condition.Get();
        return *this;
    }

    SelectQuery& OrderBy(const std::string& field);
    SelectQuery& Limit(int count);

protected:
    virtual void Generate() override{
        std::string sub_where;
        if(sub_where.empty() == false)sub_where = Format("where {}", m_where);

        m_query = x::Format("select {} from {} {}", m_select, m_table, sub_where);
    }

private:
    std::string FormatFields(const std::vector<std::string>& fields);
    void GetTableFiledMeta(const std::string& table);

private:
    std::string m_table;
    std::string m_select;
    std::string m_where;
    std::string m_oderby;
    std::unordered_map<std::string, FieldMeta>& m_metas;
};

class SqlQuery
{
public:
    SqlQuery(MYSQL* sql);

    SelectQuery Select(const std::string table, const std::vector<std::string>& fields){
        return SelectQuery(m_sql, m_metas, table, fields);
    }
    SelectQuery Select(const std::map<std::string, std::vector<std::string>>& fields){
        //return SelectQuery(m_sql, table, fields);
    }
    SelectQuery Select(const std::string table){
        //return SelectQuery(m_sql, table);
    }
    void Insert();
    void Update();
    void Delete();
   
    void DisplayFields(){
        for(auto& meta : m_metas){
            std::cout << Format("table: {}\n", meta.first);
            for(auto& it : meta.second.Fields()){
                std::cout << Format("{}\n", it.first);
            }
        }
    }


private:
    MYSQL* m_sql;
    std::unordered_map<std::string, FieldMeta> m_metas;
};


}
