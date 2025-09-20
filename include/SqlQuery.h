#pragma once
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
    QueryBase(MYSQL* sql);

    /**
     * @brief 执行查询操作
     * @return 查询结果对象
     */
    QueryResult Execute();

protected:
    virtual void Generate() = 0;

    MYSQL* m_sql;
    std::string m_query;
};

class SelectQuery: public QueryBase
{
public:
    SelectQuery(MYSQL* sql, 
                const std::string& table, 
                const std::vector<std::string>& fields);

    SelectQuery& Where(const std::string& condition);
    SelectQuery& Where(const QueryWhere& condition){
        m_where = condition.Get();
        return *this;
    }

    SelectQuery& OrderBy(const std::string& field);
    SelectQuery& Limit(int count);

protected:

    virtual void Generate() override;

private:
    std::string FormatFields(const std::vector<std::string>& fields);
    // void GetTableFiledMeta(const std::string& table);

private:
    std::string m_table;
    std::string m_select;
    std::string m_where;
    std::string m_oderby;
};

class SqlQuery
{
public:
    SqlQuery(MYSQL* sql);

    SelectQuery Select(const std::string table, const std::vector<std::string>& fields){
        return SelectQuery(m_sql, table, fields);
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
