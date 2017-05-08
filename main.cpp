#include <iostream>
#include "TypeList.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    typedef TL::TypeList<int, long long, double, short, int> Types;
    return 0;
}