#pragma once

#include <cstdint>
#include <string>

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

class DateTime
{
public:
    DateTime();
    DateTime(int64_t ts);
    DateTime(const std::string& str);

    // std::string ToString(const std::string& format);
    std::string ToString(bool withMs = true) const;

    int64_t GetMs();
    int64_t GetSec();

    static DateTime Now();

    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;


private:
    int64_t m_ts;
};

}
