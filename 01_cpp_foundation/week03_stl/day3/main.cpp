#include<map>
#include<string>
#include<unordered_map>
#include<iostream>
#include<vector>
#include<windows.h>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    map<string,int> word_count;
    unordered_map<int,string> students;

    string a = "hello world hello cpp";
    int pos = 0,next = 0;
    while((next = a.find(" ",pos)) != string::npos)
    {
        string word = a.substr(pos,next - pos);
        word_count[word]++;
        pos = next + 1;
    }
    word_count[a.substr(pos)]++;
    for(auto& p: word_count)
    {
        cout << p.first << " 出现了 " << p.second << " 次\n";
    }


    students[1001] = "张三";
    students[1002] = "李四";
    
    int id = 1002;
    auto it = students.find(id);
    if(it != students.end())
    {
        cout << "学号：" << id << "是" << it->second << endl;
    }
    else
    cout << "学号： " << id << "不存在\n";

    unordered_map<string, int> word_count2;
    pos = 0, next = 0;
    string b = "hello world hello cpp";    // 用另一份数据
    while ((next = b.find(" ", pos)) != string::npos) 
    {
        string word = b.substr(pos, next - pos);
        word_count2[word]++;
        pos = next + 1;
    }
    word_count2[b.substr(pos)]++;

    cout << " unordered_map 遍历顺序:" << endl;
    for (auto& p : word_count2) 
    {
         cout << p.first << " 出现了 " << p.second << " 次" << endl;
    }



    return 0;
}