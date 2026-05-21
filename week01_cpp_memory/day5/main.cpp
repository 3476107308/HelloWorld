#include <iostream>
using namespace std;

// ============================================================
// Day 5：野指针、悬垂指针、内存泄漏
// ============================================================

// ---------- 错误案例1：未初始化的指针（野指针） ----------
// 危险原因：指针 p 没有初始化，里面存的是一个随机的垃圾地址。
// 如果对这个地址解引用（*p），可能修改到别的程序的内存，
// 轻则程序崩溃，重则破坏系统数据。
void error1_wild_pointer()
{
    cout << "=== 错误1：未初始化指针（野指针） ===" << endl;

    int* p;             // p 未初始化，存的是随机地址
    // *p = 100;        // 取消注释会崩溃！向随机地址写入数据
    cout << "p 的地址（随机值）: " << p << endl;
    cout << "危险！不能解引用未初始化的指针" << endl;
    cout << endl;
}

// ---------- 修正版1：指针必须初始化 ----------
// 方法：声明指针时立刻初始化为 nullptr，或用 & 取已有变量的地址。
void fix1_init_pointer()
{
    cout << "=== 修正1：初始化指针 ===" << endl;

    // 方法A：初始化为 nullptr（空指针）
    int* p1 = nullptr;
    if (p1 == nullptr) {
        cout << "p1 是空指针，安全，不能解引用但至少不会误写" << endl;
    }

    // 方法B：指向一个已有的合法变量
    int a = 10;
    int* p2 = &a;       // p2 存的是变量 a 的地址
    *p2 = 20;           // 安全：修改的是 a
    cout << "a = " << a << " (通过 *p2 修改成功)" << endl;

    // 方法C：用 new 在堆上分配
    int* p3 = new int(30);
    cout << "*p3 = " << *p3 << endl;
    delete p3;          // 记得释放
    cout << endl;
}

// ---------- 错误案例2：释放后继续访问（悬垂指针） ----------
// 危险原因：delete 把内存还给了系统，但指针还存着原来的地址。
// 这块内存可能已经被分配给别的变量使用，
// 再通过旧指针访问就会读到错误数据，或者破坏别人的数据。
void error2_dangling_pointer()
{
    cout << "=== 错误2：释放后继续访问（悬垂指针） ===" << endl;

    int* p = new int(42);
    cout << "释放前 *p = " << *p << endl;

    delete p;           // 内存已归还，p 变成悬垂指针
    // p 仍然存着原来的地址，但那个地址已经不属于我们了

    // *p = 100;        // 取消注释会出问题！访问已释放的内存
    cout << "p 还存着地址: " << p << "（但内存已不属于我们）" << endl;
    cout << "危险！delete 后不能再通过这个指针访问" << endl;
    cout << endl;
}

// ---------- 修正版2：delete 后将指针置为 nullptr ----------
// 这样如果后面误用了，至少程序会立即崩溃（访问 nullptr），
// 而不是悄悄读写错误的内存。
void fix2_null_after_delete()
{
    cout << "=== 修正2：delete 后置为 nullptr ===" << endl;

    int* p = new int(42);
    cout << "释放前 *p = " << *p << endl;

    delete p;
    p = nullptr;        // 关键：释放后立即置空

    // 之后使用前先检查
    if (p == nullptr) {
        cout << "p 是 nullptr，已经安全释放过了" << endl;
    }
    cout << endl;
}

// ---------- 错误案例3：忘记释放（内存泄漏） ----------
// 危险原因：new 分配的内存不会自动回收。如果忘记 delete，
// 程序跑得越久，占的内存越多，最终可能导致系统内存耗尽。
// 尤其是循环里 new 却忘记 delete，泄漏速度非常快。
void error3_memory_leak()
{
    cout << "=== 错误3：忘记释放（内存泄漏） ===" << endl;

    int* p = new int(99);
    cout << "*p = " << *p << endl;
    // 注意：这里没有 delete p！
    // 这个函数结束后，指针 p 本身会被销毁（栈变量），
    // 但它指向的堆内存不会被自动释放，就泄漏了。

    // 更严重的例子：
    for (int i = 0; i < 3; i++) {
        int* leak = new int(i);     // 每次循环分配一块新内存
        cout << "第" << i << "次分配，地址: " << leak << endl;
        // 没有 delete！每次循环都丢失了上一次分配的地址
        // 循环 3 次就泄漏了 3 个 int
    }
    cout << "循环结束，泄漏了 3 个 int，再也找不回来了" << endl;
    cout << endl;
}

// ---------- 修正版3：配对使用 new 和 delete ----------
// 原则：每个 new 都要有一个对应的 delete。
// 在类里用构造函数分配、析构函数释放（RAII）。
void fix3_pair_new_delete()
{
    cout << "=== 修正3：配对 new/delete ===" << endl;

    // 单个变量：new 配对 delete
    int* p1 = new int(99);
    cout << "*p1 = " << *p1 << endl;
    delete p1;

    // 数组：new[] 配对 delete[]
    int n = 5;
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = i * 10;
        cout << arr[i] << " ";
    }
    cout << endl;
    delete[] arr;       // 数组用 delete[]，不能用 delete

    cout << "所有内存都已正确释放" << endl;
    cout << endl;
}

// ---------- 扩展：返回局部变量的地址（另一个悬垂指针场景） ----------
// 危险原因：函数内的局部变量存在栈上，函数返回后栈空间被回收。
// 如果返回了局部变量的地址，调用者拿到的就是一个悬垂指针。
int* error4_return_local_address()
{
    int x = 10;         // x 是局部变量，在栈上
    return &x;          // 错误！返回了局部变量的地址
}                       // 函数结束，x 被销毁，返回的地址变成悬垂指针

void fix4_return_heap_address()
{
    cout << "=== 扩展：返回局部变量地址 vs 返回堆地址 ===" << endl;

    // 错误做法：error4_return_local_address() 返回了已销毁的地址
    // int* p = error4_return_local_address(); // 危险！

    cout << "正确做法：返回 new 分配的堆地址，调用者负责 delete" << endl;
    cout << "          或者用引用/指针参数来输出结果" << endl;
    cout << endl;
}

// ---------- 总结：三类指针错误对照表 ----------
void summary()
{
    cout << "==================== 总结 ====================" << endl;
    cout << "1. 野指针：未初始化的指针" << endl;
    cout << "   -> 解法：声明时初始化为 nullptr 或 &合法变量" << endl;
    cout << endl;
    cout << "2. 悬垂指针：指向已释放内存的指针" << endl;
    cout << "   -> 解法：delete 后立刻 p = nullptr" << endl;
    cout << endl;
    cout << "3. 内存泄漏：忘记 delete 堆内存" << endl;
    cout << "   -> 解法：每个 new 配一个 delete" << endl;
    cout << "   -> 进阶：用 RAII，构造分配/析构释放" << endl;
    cout << "==============================================" << endl;
}

// ============================================================
int main()
{
    // 三个错误案例（只展示原理，危险代码已注释掉）
    error1_wild_pointer();
    error2_dangling_pointer();
    error3_memory_leak();

    // 三个修正版
    fix1_init_pointer();
    fix2_null_after_delete();
    fix3_pair_new_delete();

    // 扩展与总结
    fix4_return_heap_address();
    summary();

    return 0;
}
