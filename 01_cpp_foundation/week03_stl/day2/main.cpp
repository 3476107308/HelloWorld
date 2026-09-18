#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
using std::string;
using std::vector;
using std::cout;
using std::endl;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    string d("陈岗");
    string b("喜欢王静");
    cout << "拼接前：\n a=" << d << "  b=" << b << endl;
    d += b;
    cout << "拼接后：\n a=" << d << " b=" << b << endl;
    int idx = d.find("王静");
    int idx2 = d.find("张三");
    if(idx2 == string::npos)
    {
        cout << " \"张三\" 没找到\n";
    }
    string c = d.substr(idx,6);//中文在UTF_8中 一个字占3个字节
    d.replace(d.find("喜欢"),4,"爱");
    cout << "查找    " << c << endl;


    vector<int> a;
    a.push_back(1);
    a.push_back(4);
    a.push_back(7);
    a.push_back(9);
    a.push_back(14);
    for(auto it = a.begin();it != a.end();it++)
    cout << *it << " ";
    cout << endl;

    for(vector<int>::reverse_iterator it = a.rbegin();it != a.rend();it++)
    cout << *it << " ";
    cout << endl;
    
    for(int i = 0;i < a.size();i++)
    cout << a[i] << " ";
    cout << endl;

    for(auto x:a)
    cout << x << " ";
    cout << endl;

    return 0;
}