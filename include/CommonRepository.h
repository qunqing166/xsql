#pragma once
#include "QueryResult.h"
#include "SqlConnection.h"
#include <IRepositoryBase.h>
#include <memory>
#include <SqlQuery.h>
#include <optional>
#include <string>
#include <TableBase.h>
#include <SqlConnection.h>

namespace x{

template <typename T>
class CommonRepository: public IRepositoryBase<T>
{
public:
    CommonRepository(std::shared_ptr<SqlConnection> con):
        m_con(con)
    {
        
    }

    std::optional<T> GetFirstOrDefault(const QueryWhere& where = QueryWhere()) override
    {
        T ret{};
        TableBase<T>& base = ret;
        std::vector<std::string> fields = base.GetFields();
        QueryResult res = m_con->Query().Select(T::GetTableName(), fields).Where(where).Limit(1).Execute();
        if(auto row = res.GetRow())
        {
            for(auto& field: fields)
            {
                ret.Set(field, row.Get<std::string>(field));
            }
        }else{
            return std::nullopt;
        }
        return ret;
    }
    std::list<T> GetAll() override
    {
        std::list<T> ret;
        T tmp{};
        std::vector<std::string> fields = tmp.GetFields();
        QueryResult res = m_con->Query().Select(T::GetTableName(), fields).Execute();
        while(auto row = res.GetRow())
        {
            T val{};
            for(auto& field: fields)
            {
                val.Set(field, row.Get<std::string>(field));
            }
            ret.push_back(val);
        }
        return ret;
    }

    int Add(const T& val) override
    {
        std::vector<FieldAndValue> fvs;
        for(auto& it: T::GetFields())
        {
            fvs.push_back({it, val.Get(it)});
        }
        return m_con->Query().Insert(T::GetTableName(), fvs).Execute();
    }

    int Add(const std::vector<T>& arr) override
    {
        return 0;
    }

    int Delete() override
    {
        return 0;
    }

    int Update(const T& val) override
    {
        return 0;
    }

private:
    std::shared_ptr<SqlConnection> m_con;

};

}
