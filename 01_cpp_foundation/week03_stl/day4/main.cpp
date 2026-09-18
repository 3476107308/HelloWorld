#include<iostream>
#include<algorithm>
#include<vector>
#include<windows.h>
#include<string>


using std::cout;
using std::endl;
using std::vector;
using std::greater;
using std::string;
using std::pair;

void test01()
{
    vector<int> a{1,4,2,7,4,7,10};
    sort(a.begin(),a.end());
    cout << "升序排序\n";
    for_each(a.begin(),a.end(),[](int x){cout << x << " ";});
    cout << endl;
    cout << "降序排序\n";
    sort(a.begin(),a.end(),greater<int>());
    for_each(a.begin(),a.end(),[](int x){cout << x << " ";});
    cout << endl;
}

void test02()
{
    vector<pair<string,int>> a = {
        {"张三",91},{"张四",54},{"张五",66},{"张六",99},{"张七",56},{"张八",43}
    };

    sort(a.begin(),a.end(),[](const pair<string,int>&a,const pair<string,int>&b){return a.second > b.second;});
    cout << "按成绩降序排序\n";
    for_each(a.begin(),a.end(),[](const pair<string,int>&a){cout << "名字 " << a.first << " 成绩 " << a.second << endl;});

}

void test03()
{
    vector<int> a{1,4,2,7,4,7,10};
    auto it = find(a.begin(),a.end(),1);
    if(it != a.end())
    cout << "找到了" << *it << endl;
    else
    cout << "没找到\n";
}

void test04()
{
     vector<pair<string,int>> a = {
        {"张三",91},{"张四",54},{"张五",66},{"张六",99},{"张七",56},{"张八",43}
    };

    int person = count_if(a.begin(),a.end(),[](const pair<string,int>& x){return x.second >= 90;});
    cout << "成绩大于等于90的人数为：" << person << endl;

    // count：统计等于某个值的个数
    vector<int> b{1,4,2,7,4,7,10};
    int cnt = count(b.begin(), b.end(), 4);
    cout << "4出现了 " << cnt << " 次\n";
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    test01();
    test02();
    test03();
    test04();
    return 0;
}