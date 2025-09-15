#include "utils.h"
#include "SqlValue.h"
#include <cstdint>
#include <string>

namespace x{

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
