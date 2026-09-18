#include<iostream>
#include"solution.h"

int main()
{
    Solution temp;
    std::string a = "aba";
    std::string b = "caa";
    int result1 = temp.minCharacters(a,b);
    std::cout << result1 << std::endl;
    return 0;
}