#include <SqlType.h>
#include <cstdint>
#include <string>
#include <Format.h>
#include <chrono>
#include <utils.h>

namespace xsql{

DateTime::DateTime()
{
     *this = this->Now();
}

DateTime::DateTime(int64_t ts):
    m_ts(ts)
{

}

DateTime::DateTime(const std::string &str)
{
    std::tm tm{};
    int ms = 0;
    bool isError = false;
    if(str.find('.') == std::string::npos){
        if (std::sscanf(str.c_str(), "%d-%d-%d %d:%d:%d",
                &tm.tm_year, &tm.tm_mon,  &tm.tm_mday,
                &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6){
            isError = true;
        }
    }else{
        if (std::sscanf(str.c_str(), "%d-%d-%d %d:%d:%d.%d",
                &tm.tm_year, &tm.tm_mon,  &tm.tm_mday,
                &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &ms) != 7){
            isError = true;
        }
    }

    if(isError){
        throw std::runtime_error("");
    }

    tm.tm_year -= 1900;
    tm.tm_mon  -= 1;
    tm.tm_isdst = -1;

    m_ts = std::mktime(&tm) * 1000 + ms;
}

// std::string DateTime::ToString()
// {
//     std::chrono::milliseconds dur(m_ts);
//     std::chrono::system_clock::time_point tp = std::chrono::time_point<std::chrono::system_clock>(dur);
//     auto time_t_now = std::chrono::system_clock::to_time_t(tp);
//     std::tm tm{};
//     localtime_r(&time_t_now, &tm);
//     return XSqlFormat("{}-{}-{} {}:{}:{}.{}", 
//         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, m_ts % 1000);
// }

std::string DateTime::ToString(bool withMs) const
{
    std::chrono::milliseconds dur(m_ts);
    std::chrono::system_clock::time_point tp = std::chrono::time_point<std::chrono::system_clock>(dur);
    auto time_t_now = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    localtime_r(&time_t_now, &tm);
    if(withMs){
        return XSqlFormat("{}-{}-{} {}:{}:{}.{}", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec, m_ts % 1000);
    }else{
        return XSqlFormat("{}-{}-{} {}:{}:{}", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}

int64_t DateTime::GetMs(){
    return m_ts;
}

int64_t DateTime::GetSec(){
    return m_ts / 1000;
}

DateTime DateTime::Now()
{
    using namespace std::chrono;
    auto now = system_clock::now();

    return DateTime(duration_cast<milliseconds>(now.time_since_epoch()).count());
}

bool DateTime::operator==(const DateTime &other) const
{
    return m_ts == other.m_ts;
}

bool DateTime::operator!=(const DateTime &other) const
{
    return m_ts != other.m_ts;
}
bool DateTime::operator>(const DateTime &other) const
{
    return m_ts > other.m_ts;
}
bool DateTime::operator<(const DateTime &other) const
{
    return m_ts < other.m_ts;
}
bool DateTime::operator>=(const DateTime &other) const
{
    return m_ts >= other.m_ts;
}
bool DateTime::operator<=(const DateTime &other) const
{
    return m_ts <= other.m_ts;
}

}
