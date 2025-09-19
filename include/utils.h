#pragma once
#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <xformat.h>
#include <algorithm>

namespace x{

std::string InjectionCheck(const std::string& input);

namespace convert_detail{


/* 判断是否是算术类型 */
/*
template <typename T>
inline constexpr bool is_arithmetic_v = std::is_arithmetic_v<T> && (std::is_same_v<T, bool> == false);
*/

/* 用作算术类型的转换 */
template <typename T>
constexpr T ConvertSqlValue(std::string_view str){
    T value;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
    if(ec == std::errc{} )return value;
    throw std::logic_error(Format("convert to '{}' error", typeid(T).name()));
}

/* 字符串类型的特化 */
template <>
constexpr std::string ConvertSqlValue<std::string>(std::string_view str){
    return std::string(str);
}

/* bool类型的特化 */
template <>
bool ConvertSqlValue<bool>(std::string_view str){
    // 去除大小写敏感
    std::string val(str.length(), '0');
    std::transform(str.begin(), str.end(), val.begin(), ::tolower);
    if (str == "true"  || str == "1" || str == "yes" || str == "on")  return true;
    if (str == "false" || str == "0" || str == "no"  || str == "off") return false;
    throw std::logic_error("convert to 'bool' error");
}


}

template <typename T>
constexpr T ConvertSqlValue(const std::string& val){
    return convert_detail::ConvertSqlValue<T>(val);
}

}
