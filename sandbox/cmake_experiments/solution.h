#pragma
#include<string>

class Solution
{
public:
int minCharacters(std::string a,std::string b)
{
int arr[26]={0};
int brr[26]={0};
int aLen = a.length(),bLen = b.length(),minOps = INT_MAX;
//将字符串中小写子母出现次数记录下来
for (int i=0;i < aLen;i++)
arr[a.at(i)-'a']++;
for (int i=0;i<bLen;i++)
brr[b.at(i)-'a']++;

//定义个字符交互次数计数器
int aTimes = 0,bTimes = 0;
for (int i=0;i<26;i++)
{
aTimes += arr[i];
bTimes += brr[i];
minOps = std::min(
//条件1与2
std::min(aLen-aTimes+bTimes,bLen-bTimes+aTimes),
//条件3
std::min(minOps,aLen -arr[i]+bLen- brr[i])
);
}
return minOps;
}

};