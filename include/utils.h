#pragma once
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <xformat.h>

namespace x{

std::string InjectionCheck(const std::string& input);

namespace convert_detail{


/* 判断是否是算术类型 */
/*
template <typename T>
inline constexpr bool is_arithmetic_v = std::is_arithmetic_v<T> && (std::is_same_v<T, bool> == false);
*/

/* 用作算术类型的转换 */
template <typename T = std::string>
constexpr T ConvertSqlValue(std::string_view str){
    T value;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
    if(ec == std::errc{} )return value;
    throw std::logic_error(Format("convert to '{}' error", typeid(T).name()));
}

/* 字符串类型的特化 */
template <>
std::string ConvertSqlValue<std::string>(std::string_view str);

/* bool类型的特化 */
template <>
bool ConvertSqlValue<bool>(std::string_view str);

}

template <typename T>
constexpr T ConvertSqlValue(const std::string& val){
    return convert_detail::ConvertSqlValue<T>(val);
}

template <typename T>
T AnyValueConvert(const std::string& value)
{
    throw std::logic_error("error type");
    return T{};
}

template<typename T>
std::string ConvertToString(T& value)
{
    using type = std::decay_t<T>;
    if constexpr(std::is_same_v<type, bool>){
        return value ? "true" : "false";
    }else if constexpr(std::is_same_v<type, char>){
        return value;
    }else if constexpr(std::is_integral_v<type>){
        return std::to_string(value);
        // if constexpr(std::is_signed_v<type>){
        //     return var(static_cast<int64_t>(value));
        // }else{
        //     return var(static_cast<uint64_t>(value));
        // }
    }else if constexpr(std::is_floating_point_v<type>){
        return std::to_string(value);
    }else if constexpr(std::is_convertible_v<type, std::string>){
        return value;
    }else{
        static_assert(sizeof(T) == 0, "unsupported type");
    }
    return std::string();
}



}
