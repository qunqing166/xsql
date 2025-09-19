#include <iostream>
#include <string>
#include "../include/utils.h"

int main()
{

    int num1 = x::ConvertSqlValue<int>("123");
    int num2 = x::ConvertSqlValue<int>("12.3");
    int num3 = x::ConvertSqlValue<int>("1aa23");
    double num4 = x::ConvertSqlValue<double>("12.3");
    float num5 = x::ConvertSqlValue<float>("12.32");

    std::cout << x::ConvertSqlValue<std::string>("ciallo") << "\n";

    std::cout   << num1 << '\n'
                << num1 << '\n'
                << num2 << '\n'
                << num3 << '\n'
                << num4 << '\n'
                << num5 << '\n';
    std::cout << "ciallo\n";
    return 0;
}
