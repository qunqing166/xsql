#pragma once
#include <cstdint>
#include <filesystem>
#include <mysql/plugin_auth_common.h>
#include <stdexcept>
#include <string>
#include <mysql/mysql.h>
#include <map>
#include <variant>
#include <type_traits>
#include <SqlType.h>

namespace xsql{

class SqlOutputValue
{
public:
    SqlOutputValue(const std::string& val){}

    template<typename T>
    T Get(){
        throw std::logic_error("不支持的sql类型");
    }

private:
    std::string m_value;
};

    

class SqlInputValue
{
public:
    template<typename T>
    constexpr SqlInputValue(T&& value){
        using type = std::decay_t<T>;
        if constexpr(std::is_same_v<type, bool>){
            m_val = (bool(value));
        }else if constexpr(std::is_same_v<type, char>){
            m_val = char(value);
        }else if constexpr(std::is_integral_v<type>){
            m_val = static_cast<int64_t>(value);
        }else if constexpr(std::is_floating_point_v<type>){
            m_val = static_cast<double>(value);
        }else if constexpr(std::is_convertible_v<type, std::string>){
            m_val = std::string(std::forward<T>(value));
        }else{
            static_assert(sizeof(T) == 0, "unsupported type");
        }
    }

    SqlInputValue(xsql::DateTime&& value):
        m_val(value)
    {
    }

    SqlInputValue(xsql::DateTime& value):
        m_val(value)
    {
    }

    std::string Get() const{
        std::string ret;
        switch(m_val.index())
        {
        case 0:return std::to_string(std::get<int64_t>(m_val));
        case 1:return std::to_string(std::get<double>(m_val));
        case 2:ret = std::get<std::string>(m_val);break;
        case 3:ret = std::get<xsql::DateTime>(m_val).ToString();break;
        default:throw std::logic_error("SqlInputValue get error");
        }

        for(int i = ret.length() - 1; i >=0; --i){
            if(ret[i] == '\''){
                ret.insert(ret.begin() + i, '\'');
            }
        }
        return "'" + ret + "'";
    }

    bool IsNumber(){return m_val.index() == 0 || m_val.index() == 1;}
    bool IsInt() const{return m_val.index() == 0;}
    bool IsFloat() const{return m_val.index() == 1;}
    bool IsString() const{return m_val.index() == 2;}

    std::string GetInt() const{return std::to_string(std::get<int64_t>(m_val));}
    std::string GetFloat() const{return std::to_string(std::get<double>(m_val));}
    std::string GetString() const{return std::get<std::string>(m_val);}

private:
    using value = std::variant<int64_t, double, std::string, xsql::DateTime>;
    value m_val;
};


class FieldMeta
{
public:
    FieldMeta(){}
    FieldMeta(const std::string& name, const std::string& type);
    
    enum Type{
        TYPE_UNKOWN,
        TYPE_BIT_8 = MYSQL_TYPE_TINY,
        TYPE_BIT_16 = MYSQL_TYPE_SHORT,
        TYPE_BIT_24 = MYSQL_TYPE_INT24,
        TYPE_BIT_32 = MYSQL_TYPE_LONG,
        TYPE_BIT_64 = MYSQL_TYPE_LONGLONG,
        TYPE_FLOAT = MYSQL_TYPE_FLOAT,
        TYPE_DOUBLE = MYSQL_TYPE_DOUBLE,
        TYPE_STRING = MYSQL_TYPE_STRING
    };

    void InsertType(const std::string& field, const std::string& type);
    Type GetType(const std::string& field);

    const std::map<std::string, Type>& Fields(){return m_fieldType;}
private:
    std::string m_tableName;
    std::string m_name;
    std::string m_typeStr;
    std::map<std::string, Type> m_fieldType;
    Type m_type;
};

template<typename Input, typename Output>
Output ConvertToSqlValue(Input input, FieldMeta::Type type){
    switch(type){
    case FieldMeta::TYPE_BIT_8:return static_cast<int8_t>(input);
    case FieldMeta::TYPE_BIT_16:return static_cast<int16_t>(input);
    case FieldMeta::TYPE_BIT_32:return static_cast<int32_t>(input);
    case FieldMeta::TYPE_BIT_64:return static_cast<int64_t>(input);
    case FieldMeta::TYPE_FLOAT:return static_cast<float>(input);
    case FieldMeta::TYPE_DOUBLE:return static_cast<double>(input);
    default:return input;
    }
}

}
