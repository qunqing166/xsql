#pragma once

#include <exception>

namespace x{

class ParamError: public std::exception
{
public:
    ParamError();
    // const char * what() const override;
};

}
