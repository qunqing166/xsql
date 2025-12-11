#pragma once

#define XSQL_DEBUG_ENABLE

#ifdef XSQL_DEBUG_ENABLE

#include <iostream>
#include <Format.h>

#define XSQL_DEBUG(format, ...) \
        std::cout << XSqlFormat(format, ##__VA_ARGS__) << '\n'
#elif

#define XSQL_DEBUG(...)

#endif