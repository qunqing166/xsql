#include <SqlType.h>
#include <cstdint>
#include <string>
#include <xformat.h>
#include <chrono>

namespace x{

DateTime::DateTime():
    m_ts(0)
{
}

DateTime::DateTime(int64_t ts):
    m_ts(ts)
{

}

std::string DateTime::ToString(const std::string& format)
{
    std::chrono::milliseconds dur(m_ts);
    std::chrono::system_clock::time_point tp = std::chrono::time_point<std::chrono::system_clock>(dur);
    auto time_t_now = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    localtime_r(&time_t_now, &tm);
    return Format("{}-{}-{} {}:{}:{}", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

DateTime DateTime::Now()
{
    using namespace std::chrono;
    auto now = system_clock::now();

    return DateTime(duration_cast<milliseconds>(now.time_since_epoch()).count());
}

}
