#include <iostream>
#include <cstring>   // memcpy
using namespace std;



// 五法则：析构/拷贝构造/拷贝赋值/移动构造/移动赋值


class IntArray
{
private:
    int cap_;     
    int size_;    
    int* p_;     

    // 扩容：容量翻倍（至少扩到 1）
    void expand()
    {
        int new_cap = (cap_ == 0) ? 1 : 2 * cap_;
        int* new_p = new int[new_cap];
        // memcpy 按字节拷贝，所以 size_ 要乘以 sizeof(int)
        memcpy(new_p, p_, size_ * sizeof(int));
        delete[] p_;           
        p_ = new_p;
        cap_ = new_cap;
    }

public:
    IntArray() : cap_(10), size_(0)
    {
        p_ = new int[cap_];
    }

    
    IntArray(int cap) : cap_(cap), size_(0)
    {
        p_ = new int[cap_];
    }

   
    ~IntArray()
    {
        delete[] p_;   // p_ 始终指向堆数组或 nullptr，delete[] 对两者都安全
    }

   
    IntArray(const IntArray& source) : cap_(source.cap_),size_(source.size_)   
    {
        
        p_ = new int[cap_];
        memcpy(p_, source.p_, size_ * sizeof(int));
        cout << "[拷贝构造] 深拷贝完成，size=" << size_
             << ", cap=" << cap_ << endl;
    }

   
    IntArray& operator=(const IntArray& source)
    {
       
        if (this == &source)
        {
            cout << "[拷贝赋值] 检测到自身赋值，跳过" << endl;
            return *this;
        }

     
        delete[] p_;
        cap_ = source.cap_;
        size_ = source.size_;

       
        p_ = new int[cap_];
        memcpy(p_, source.p_, size_ * sizeof(int));

        cout << "[拷贝赋值] 深拷贝完成，size=" << size_ << endl;
        return *this;   // 返回自身引用，支持 a = b = c
    }


    // 【noexcept 必须写！】
    //   告诉编译器"这个函数不抛异常"，这样 vector<IntArray> 扩容时
    //   会优先用移动（O(1)）而不是拷贝（O(n)），性能提升巨大。
    //   如果不写 noexcept，vector 为了"强异常安全保证"，
    //   宁可拷贝也不移动。
    IntArray(IntArray&& source) noexcept : cap_(source.cap_),   size_(source.size_),   p_(source.p_)          
    {
        // 把源对象掏空，让它能安全析构（delete[] nullptr 是安全的）
        source.p_ = nullptr;
        source.cap_ = 0;
        source.size_ = 0;

        cout << "[移动构造] 资源转移完成（O(1)，无深拷贝）" << endl;
    }

    // ============================================================
    // 7. 移动赋值运算符
    // ============================================================
    // 和移动构造一样"偷"资源，但多了两步：
    //   ① 先释放自己的旧内存（不然会泄漏）
    //   ② 掏空源对象
    // ============================================================
    IntArray& operator=(IntArray&& source) noexcept
    {
        if (this == &source)          // 自身赋值检查
        {
            cout << "[移动赋值] 检测到自身赋值，跳过" << endl;
            return *this;
        }

        // ① 先释放自己的旧内存
        delete[] p_;

        // ② 偷走 source 的资源
        p_ = source.p_;
        cap_ = source.cap_;
        size_ = source.size_;

        // ③ 掏空源对象
        source.p_ = nullptr;
        source.cap_ = 0;
        source.size_ = 0;

        cout << "[移动赋值] 资源转移完成（O(1)，无深拷贝）" << endl;
        return *this;
    }

    // ============================================================
    // 以下是功能函数
    // ============================================================

    void push_back(int temp)
    {
        if (size_ + 1 > cap_) expand();   // [修] >= 改为 > ：
        // 原代码 size_+1 >= cap_ 会在还有 1 个空位时就扩容，浪费一次
        p_[size_] = temp;
        size_++;
    }

    int size() const
    {
        return size_;
    }

    // [修] 返回 int& 引用，这样才能通过 arr[i] = xx 修改元素
    // 原来返回 int 是"值返回"，arr[i] = xx 修改不了数组内容
    int& operator[](int i)
    {
        if (i >= size_)     // [修] > 改为 >=：下标从 0 开始，
        // size_=5 时合法下标是 0~4，i==5 才越界
        {
            cout << "数组越界\n";
            // 返回一个静态变量的引用，避免返回局部变量的引用（悬垂引用）
            // 实际项目中这里应该抛异常，这里为了演示先这样处理
            static int dummy = 0;
            return dummy;
        }
        return p_[i];
    }

    // 只读版本的下标访问（供 const 对象使用）
    const int& operator[](int i) const
    {
        if (i >= size_)
        {
            cout << "数组越界\n";
            static int dummy = 0;
            return dummy;
        }
        return p_[i];
    }

    // 打印数组内容（方便调试）
    void print() const
    {
        cout << "[";
        for (int i = 0; i < size_; i++)
        {
            if (i > 0) cout << ", ";
            cout << p_[i];
        }
        cout << "] (size=" << size_ << ", cap=" << cap_ << ")" << endl;
    }
};


int main()
{
    cout << "========== Day 6 验收：IntArray 五法则验证 ==========" << endl;
    cout << endl;

    // --- 1. push_back / operator[] / size ---
    cout << "--- 1. 基本功能测试 ---" << endl;
    IntArray arr1;
    for (int i = 1; i <= 10; i++)
    {
        arr1.push_back(i * 10);   // 10, 20, 30, ..., 100
    }
    arr1.print();

    // --- 2. 拷贝构造（深拷贝）---
    cout << "\n--- 2. 拷贝构造 ---" << endl;
    IntArray arr2 = arr1;          // 这里调用拷贝构造函数
    arr2[0] = 999;                 // 修改 arr2 的第 0 个元素
    cout << "修改 arr2[0] = 999 后：" << endl;
    cout << "arr1: "; arr1.print(); // arr1[0] 仍然是 10（深拷贝成功！）
    cout << "arr2: "; arr2.print(); // arr2[0] 变成了 999
    // 如果浅拷贝，arr1[0] 也会变成 999（两个对象共享同一块内存）

    // --- 3. 拷贝赋值（深拷贝）---
    cout << "\n--- 3. 拷贝赋值 ---" << endl;
    IntArray arr3(3);              // 创建一个小容量数组
    arr3.push_back(1);
    arr3.push_back(2);
    cout << "赋值前 arr3: "; arr3.print();
    arr3 = arr1;                   // 这里调用拷贝赋值运算符
    cout << "赋值后 arr3: "; arr3.print();
    cout << "修改 arr3[1] = 777 后：" << endl;
    arr3[1] = 777;
    cout << "arr1: "; arr1.print(); // arr1 不受影响（深拷贝成功！）
    cout << "arr3: "; arr3.print();

    // --- 4. 移动构造 ---
    cout << "\n--- 4. 移动构造 ---" << endl;
    IntArray arr4 = std::move(arr1); // std::move 将 arr1 转成右值，触发移动构造
    cout << "arr1 被掏空后: "; arr1.print();  // arr1 已空
    cout << "arr4 获得资源: "; arr4.print();  // arr4 持有 arr1 原来的数据
    // 注意：移动比拷贝快得多，因为只改了 3 个指针/变量，没有 new/memcpy

    // --- 5. 移动赋值 ---
    cout << "\n--- 5. 移动赋值 ---" << endl;
    IntArray arr5;
    arr5.push_back(7);
    arr5.push_back(8);
    arr5.push_back(9);
    cout << "赋值前 arr5: "; arr5.print();
    arr5 = std::move(arr2);        // std::move 触发移动赋值
    cout << "arr2 被掏空后: "; arr2.print();
    cout << "arr5 获得资源: "; arr5.print();

    // --- 6. 自身赋值检查 ---
    cout << "\n--- 6. 自身赋值 ---" << endl;
    arr4 = arr4;                   // 自赋值 → 安全跳过

    cout << "\n========== 全部测试通过 ==========" << endl;
    return 0;
}
