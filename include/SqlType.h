#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <compare>

namespace xsql{

/**
 * 在使用mysql的datetime类型的时候, mysql的datetime精度默认是秒级
 * 但是xsql::DateTime的与数据库交互的时候用的精度为毫秒
 * 当mysql的datetime使用秒级精度的时候传入毫秒级的时间, mysql会将大于500的毫秒看作1秒
 * 因此可能会出现1秒的误差
 * 将'2025-11-11 0:0:0.500'写入mysql的datetime, 其保存的额值为'2025-11-11 00:00:01'
 * 
 * 解决办法: 提升mysql的datetime的精度, 将类型改为 datetime(3), 这就是毫秒精度, 或者datetime(6)微妙精度 
 */

template <typename T, size_t L = 0>
struct TypeMapper{
    static const char* Get();
};

enum DateTimeAccuracy{
    accuracy_s = 0,
    accuracy_ms = 3,
    accuracy_us = 6,
};

//template <DateTimeAccuracy A = accuracy_ms>
class DateTime
{
public:
    DateTime();
    DateTime(int64_t ts);
    DateTime(const std::string& str);

    std::string ToString(bool withMs = true) const;

    int64_t GetMs();
    int64_t GetSec();

    static DateTime Now();

    std::strong_ordering operator<=>(const DateTime& other) const;
    bool operator==(const DateTime& other) const;

private:
    int64_t m_ts;
};

template <DateTimeAccuracy A>
class DateTimeWithAccuracy{

};

using DateTime_S = DateTimeWithAccuracy<accuracy_s>;
using DateTime_Ms = DateTimeWithAccuracy<accuracy_ms>;
using DateTime_Us = DateTimeWithAccuracy<accuracy_us>;


template <>
struct TypeMapper<bool>{
    static const char* Get(){return "bool";}
};

/**
 * 无符号数
 */
template <> struct TypeMapper<uint8_t>{
    static const char* Get(){return "tinyint unsigned";}
};
template <> struct TypeMapper<uint16_t>{
    static const char* Get(){return "smallint unsigned";}
};
template <> struct TypeMapper<uint32_t>{
    static const char* Get(){return "int unsigned";}
};
template <> struct TypeMapper<uint64_t>{
    static const char* Get(){return "bigint unsigned";}
};

/**
 * 有符号数
 */
template <> struct TypeMapper<int8_t>{
    static const char* Get(){return "tinyint";}
};
template <> struct TypeMapper<int16_t>{
    static const char* Get(){return "smallint";}
};
template <> struct TypeMapper<int32_t>{
    static const char* Get(){return "int";}
};
template <> struct TypeMapper<int64_t>{
    static const char* Get(){return "bigint";}
};

/**
 * 浮点类型
 */
template <> struct TypeMapper<float>{
    static const char* Get(){return "float";}
};
template <> struct TypeMapper<double>{
    static const char* Get(){return "double";}
};


/**
 * 枚举
 */

/**
 * 日期与时间
 * date
 * time
 * datetime
 * timestamp
 * year
 */

/**
 * json
 */

namespace detail{

template <std::size_t N>
requires(N > 0)
inline constexpr auto inli_charNTypename = []() consteval{
    constexpr auto numLen = []() consteval{
        std::size_t ret = 0;
        auto n = N;
        while(n != 0){n /= 10; ++ret;}
        return ret;
    }();

    int n = N;
    constexpr std::size_t constLen = std::char_traits<char>::length("char()");
    constexpr std::size_t totalLen = constLen + numLen + 1;
    std::array<char, totalLen> type{"char()"};
    type[totalLen - 1] = '\0';
    type[totalLen - 2] = ')';
    for(int i = totalLen - 3; i > constLen - 2; --i){
        type[i] = n % 10 + '0';
        n /= 10;
    }
    return type;
}();

template <std::size_t N>
requires(N > 0)
inline constexpr auto inli_varcharNTypename = []() consteval{
    constexpr auto numLen = []() consteval{
        std::size_t ret = 0;
        auto n = N;
        while(n != 0){n /= 10; ++ret;}
        return ret;
    }();

    int n = N;
    constexpr std::size_t constLen = std::char_traits<char>::length("varchar()");
    constexpr std::size_t totalLen = constLen + numLen + 1;
    std::array<char, totalLen> type{"varchar()"};
    type[totalLen - 1] = '\0';
    type[totalLen - 2] = ')';
    for(int i = totalLen - 3; i > constLen - 2; --i){
        type[i] = n % 10 + '0';
        n /= 10;
    }
    return type;
}();

template <DateTimeAccuracy A>
inline constexpr auto inli_datetimeTypename = []() consteval{
    const auto constLen = std::char_traits<char>::length("datetime(n)") + 1;
    std::array<char, constLen> type{"datetime(n)"};
    type[constLen - 1] = '\0';
    type[constLen - 3] = '0' + (int)A;
    return type;
}();

}

/**
 * 字符串类型
 * char(n) 0 ~ 255
 * varchar(n) 0 ~ 65535
 * tinytext 255
 * text 65535
 */

/**
 * char(n)
 */ 
template <std::size_t N>
struct TypeMapper<std::array<char, N>>{
    static const char* Get(){return detail::inli_charNTypename<N>.data();}
};

/**
 * varchar(n)
 */
template <std::size_t N>
struct TypeMapper<std::string, N>{
    static const char* Get(){return detail::inli_varcharNTypename<N>.data();}
};

template <>
struct TypeMapper<DateTime>{
    static const char* Get(){return "datetime";}  
};

template <template <DateTimeAccuracy = accuracy_ms> class C, DateTimeAccuracy A>
requires std::is_same_v<C<A>, DateTimeWithAccuracy<A>>
struct TypeMapper<C<A>>{
    static const char* Get(){
        return detail::inli_datetimeTypename<A>.data();
    }
};


}
