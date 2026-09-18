#include<iostream>
#include<vector>

int minNumberInRotateArray(std::vector<int> rotateArray)
{
    size_t left =  0;
    size_t right = rotateArray.size()-1;
    while(left < right)
    {
    size_t mid = (left + right)/2;//最小的数字在mid右边
    if (rotateArray[mid] > rotateArray[right])
    left = mid +1;
    else if(rotateArray[mid] < rotateArray[right])
        right = mid;
    //最小数字要么是mid要么在mid左边
    else
    right--;
    }
    return rotateArray[left];
}

int main()
{
    std::vector<int> test1 = {3,4,5,1,2};
    std::vector<int> test2 = {5,5,5,6,4,5};
    int a1 = minNumberInRotateArray(test1);
    int a2 = minNumberInRotateArray(test2);
    return 0;
}