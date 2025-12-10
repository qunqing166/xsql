#pragma once

#include <cstdint>
#include <string>

namespace xsql{

class DateTime
{
public:
    DateTime();
    DateTime(int64_t ts);
    //DateTime(const std::string& str);

    std::string ToString(const std::string& format = "yyyy-MM-ddThh:mm:ss");
    int64_t GetTimeStamp();

    static DateTime Now();

private:
    int64_t m_ts;
};

}
