#include<iostream>
using namespace std;

int main()
{
    int* p1 = new int(1);
    cout << *p1 << endl;
    cout << "请输入数组长度";
    int n;
    cin >> n;
    int* array1 = new int[n];
    int array1_sum = 0,array1_max = -0x3f3f3f3f;
    double array1_aver = 0;
    for(int i = 0;i < n;i++)
    {
        *(array1+i) = i + 1;
    }

    for(int i = 0;i < n;i++)
    {
        array1_sum += *(array1 + i);
        array1_max = max(array1_max,*(array1 + i)); 
    }
    array1_aver = array1_sum / 10.0;
    cout << "sum:" << array1_sum << " max:" << array1_max << " average:" << array1_aver << endl;

    delete p1;
    delete [] array1;
    return 0;
}