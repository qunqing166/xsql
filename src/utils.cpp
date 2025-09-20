#include "utils.h"
#include <string>
#include <algorithm>

namespace x{

namespace convert_detail{

template <>
bool ConvertSqlValue<bool>(std::string_view str){
    // 去除大小写敏感
    std::string val(str.length(), '0');
    std::transform(str.begin(), str.end(), val.begin(), ::tolower);
    if (str == "true"  || str == "1" || str == "yes" || str == "on")  return true;
    if (str == "false" || str == "0" || str == "no"  || str == "off") return false;
    throw std::logic_error("convert to 'bool' error");
}


template <>
std::string ConvertSqlValue<std::string>(std::string_view str){
    return std::string(str);
}


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
