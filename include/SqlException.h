#pragma once

#include <exception>

namespace xsql{

class ParamError: public std::exception
{
public:
    ParamError();
    // const char * what() const override;
};

}
