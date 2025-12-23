#pragma once

#define XSQL_DEBUG_ENABLE

#ifdef XSQL_DEBUG_ENABLE

#include <iostream>
#include <format>

#define XSQL_DEBUG(fmt, ...)    std::cout << std::format(fmt, ##__VA_ARGS__) << '\n'
#elif

#define XSQL_DEBUG(...)

#endif
