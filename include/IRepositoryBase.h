#pragma once
#include <optional>
#include <vector>
#include <QueryCondition.h>

namespace x{

template <typename T>
class IRepositoryBase
{
public:

    virtual std::optional<T> GetFirstOrDefault(const QueryWhere& where) = 0;
    virtual std::list<T> GetAll() = 0;
    virtual int Add(const T& val) = 0;
    virtual int Add(const std::vector<T>& arr) = 0;
    virtual int Delete() = 0;
    virtual int Update(const T& val) = 0;

};

}
