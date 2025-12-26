#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <cxxabi.h>
#include <utils.h>
#include <SqlValue.h>

namespace xsql{

template <class T>
class EntityBase
{
public:
    using class_name = T;
    using Getter = std::function<std::string(T*)>;
    using Setter = std::function<void(T*, const std::string&)>;

    InputString Get(const std::string& field) const
    {
        auto it = m_table.find(field);
        if(it == m_table.end())return std::string();
        return it->second.first((T*)this);
    }

    constexpr void Set(const std::string& name, const std::string& value)
    {
        auto it = m_table.find(name);
        if(it == m_table.end())return;
        it->second.second((T*)this, value);
   }

    static std::vector<std::string> GetFields()
    {
        std::vector<std::string> ret;
        ret.reserve(m_table.size());
        for(auto& it: m_table)
        {
            ret.push_back(it.first);
        }
        return ret;
    }

    template<typename TYPE>
    constexpr void FromString(const std::string& value)
    {
        
    }

    template<typename TYPE>
    inline static TYPE Register(const std::string& filed, Getter getter, Setter setter, TYPE&& value = TYPE{})
    {
        if(m_isTableInited)return value;
        if(m_table.contains(filed) == false){
            m_table[filed] = {getter, setter};
        }else m_isTableInited = true;
        return value;
    }

    static std::string GetTableName()
    {
        std::string name;
    #ifdef __GNUC__
        int status = 0;
        std::unique_ptr<char, void(*)(void*)> dem(
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status), std::free);
        name = (status == 0) ? dem.get() : typeid(T).name();
    #else
        name = typeid(T).name();        // MSVC 已可读
    #endif

        auto pos = name.rfind("::");
        return (pos == std::string::npos) ? name : name.substr(pos + 2);
    }

private:

    // void SetValueInt8();
    // void SetValueInt16();
    // void SetValueInt32();
    // void SetValieInt64();
    //
    void SetValueDetail(const std::string value, const Setter& setter)
    {
             
    }

private:
    inline static std::unordered_map<std::string, std::pair<Getter, Setter>> m_table;
    inline static bool m_isTableInited = false;
};

//}

#define REGISTER_WITHOUT_VALUE(type, property, setter, getter)  \
        Register<type>(#property, setter, getter)

#define REGISTER_WITH_VALUE(type, property, setter, getter, default_)  \
        Register<type>(#property, setter, getter, default_)


#define FIELD_REGISTER(type, property)\
        public:\
            type Get##property() const{return m_##property;}\
            void Set##property(const type& value){m_##property = value;}\
        private:\
            std::string Get##property##_(){return xsql::ConvertToString<type>(m_##property);}\
            void Set##property##_(const std::string& value){m_##property = xsql::ConvertSqlValue<type>(value);}\
        private:\
            type m_##property = REGISTER_WITHOUT_VALUE(type, property, &class_name::Get##property##_, &class_name::Set##property##_);

#define FIELD_REGISTER_WITH_VALUE(type, property, default_)\
        public:\
            type Get##property() const{return m_##property;}\
            void Set##property(const type& value){m_##property = value;}\
        private:\
            std::string Get##property##_(){return xsql::ConvertToString<type>(m_##property);}\
            void Set##property##_(const std::string& value){m_##property = xsql::ConvertSqlValue<type>(value);}\
        private:\
            type m_##property = REGISTER_WITH_VALUE(type, property, &class_name::Get##property##_, &class_name::Set##property##_, default_);

}
