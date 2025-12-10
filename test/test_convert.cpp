#include <cstdint>
#include <filesystem>
#include <iostream>
#include <pthread.h>
#include <string>
#include "utils.h"
#include <SqlType.h>

using namespace xsql;

class test
{
   int8_t a;
   int16_t b;
   int32_t c;
   int64_t d;
   std::string e;
   DateTime f;
};

int main()
{

    int num1 = ConvertSqlValue<int>("123");
    int num2 = ConvertSqlValue<int>("12.3");
    int num3 = ConvertSqlValue<int>("1aa23");
    double num4 = ConvertSqlValue<double>("12.3");
    float num5 = ConvertSqlValue<float>("12.32");

    std::cout << ConvertSqlValue<std::string>("ciallo") << "\n";

    std::cout   << num1 << '\n'
                << num1 << '\n'
                << num2 << '\n'
                << num3 << '\n'
                << num4 << '\n'
                << num5 << '\n';
    std::cout << "ciallo\n";
    return 0;
}
