#include <utils.h>
#include <cstdint>
#include <string>
#include <algorithm>

namespace xsql{

namespace convert_detail{

template <>
bool ConvertSqlValue<bool>(std::string_view str){
    std::string val(str.length(), '0');
    std::transform(str.begin(), str.end(), val.begin(), ::tolower);
    if (val == "true"  || val == "1" || val == "yes" || val == "on")  return true;
    if (val == "false" || val == "0" || val == "no"  || val == "off") return false;
    throw std::logic_error("convert to 'bool' error");
}


template <>
std::string ConvertSqlValue<std::string>(std::string_view str){
    return std::string(str);
}


}

template <>
std::string ConvertSqlValue(const std::string& val){
    return val;
}

template <>
xsql::DateTime ConvertSqlValue(const std::string& val){
    return xsql::DateTime(val);
}

template <>
std::string AnyValueConvert<std::string>(const std::string& value)
{
    return value;
}

template <>
int8_t AnyValueConvert<int8_t>(const std::string& value)
{
    return std::stoll(value);
}

template <>
int16_t AnyValueConvert<int16_t>(const std::string& value)
{
    return std::stoll(value);
}

template <>
int32_t AnyValueConvert<int32_t>(const std::string& value)
{
    return std::stoll(value);
}

template <>
int64_t AnyValueConvert<int64_t>(const std::string& value)
{
    return std::stoll(value);
}

template <>
float AnyValueConvert<float>(const std::string& value)
{
    return std::stof(value);
}

template <>
double AnyValueConvert<double>(const std::string& value)
{
    return std::stod(value);
}

//std::string InjectionCheck(const SqlInputValue& value){
//    if(value.IsInt())return std::to_string(value.GetInt());
//    if(value.IsFloat())return std::to_string(value.GetFloat());
//    std::string ret = std::get<std::string>(value);
//    for(int i = ret.length() - 1; i >=0; --i){
//        if(ret[i] == '\''){
//            ret.insert(ret.begin() + i, '\'');
//        }
//    }
//    return ret;
//}

}
