#include<iostream>
#include<windows.h>
#include<vector>
#include<stdexcept>
using std::vector;
using std::cout;
using std::endl;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    vector<int> a;
    for(int i = 0;i < 100;i++)
    {
        a.push_back((i + 1) * 2);
        cout << "第" << (i+1) << "次 push后：size= " << a.size() << " capacity=" << a.capacity() << endl;
    }

    cout << "越界\n";
    //cout << "a[100] = " << a[100];
    //cout <<"operator[] 越界不报错\n";
    try
    {
        cout << a.at(100) << endl;
    } 
    catch(const std::out_of_range& e)
    {
        cout << "a.at(10)报错:" << e.what() << "at()会检查越界" << endl;
    }
    return 0;
}