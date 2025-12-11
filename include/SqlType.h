#pragma once

#include <cstdint>
#include <string>

namespace xsql{

class DateTime
{
public:
    DateTime();
    DateTime(int64_t ts);
    DateTime(const std::string& str);

    // std::string ToString(const std::string& format);
    std::string ToString(bool withMs = true);

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
