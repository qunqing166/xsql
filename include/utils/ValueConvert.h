#pragma once
#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <format>
#include <SqlType.h>

namespace xsql{

namespace convert_detail{

/* 用作算术类型的转换 */
template <typename T = std::string>
constexpr T ConvertSqlValue(std::string_view str){
    T value{};
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    if( (ptr != str.data()) &&
        ((ec == std::errc{} && ptr == str.data() + str.size()) || 
        (ptr != str.data() + str.size() && *ptr == '.')))return value;
    throw std::logic_error(std::format("convert to '{}' error", typeid(T).name()));
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

template <>
std::string ConvertSqlValue(const std::string& val);

template <>
xsql::DateTime ConvertSqlValue(const std::string& val);

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
        // return value;
        return XSqlFormat("'{}'", value);
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
        return std::format("'{}'", value);
    }else{
        static_assert(sizeof(T) == 0, "unsupported type");
    }
    return std::string();
}

template<>
std::string ConvertToString(DateTime& value);



}
