#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<cstdio>      // C 语言文件操作: FILE*, fopen, fclose, fprintf
#include<windows.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::move;

// ============================================================
// Day 4: unique_ptr 和 RAII
// 对应: Cherno C++ 智能指针 + cppreference unique_ptr
// ============================================================

// ============================================================
// Lesson 1: RAII —— 资源获取即初始化
// ============================================================
//
// RAII = Resource Acquisition Is Initialization
// 中文：资源获取即初始化（翻译有点拗口，重点是"绑定生命周期"）
//
// 核心思想：
//   你申请了一个"资源"（内存、文件、锁），把它交给一个对象。
//   对象构造时获取资源，对象析构时自动释放资源。
//   这样你就不用手动记得释放了 —— 对象离开作用域自然会析构。
//
// 就好比你借了图书馆的书：
//   普通做法：自己记住还书日期，忘了就罚款（内存泄漏）
//   RAII 做法：把书交给一个自动还书的机器人，你走了它自动帮你还
//
// C++ 标准库里到处都是 RAII：
//   - unique_ptr / shared_ptr  → 管理堆内存
//   - fstream                   → 管理文件
//   - lock_guard / scoped_lock  → 管理互斥锁
//   - string / vector           → 管理堆内存

// 演示：用 FILE* 手动管理 vs RAII 管理
//
// C 语言的 FILE* 是什么？
//   FILE 是 C 语言标准库定义的结构体，代表一个"打开的文件"
//   FILE* 是指向它的指针，类似 C++ 的 ofstream 对象
//   区别：ofstream 析构时自动关文件，FILE* 必须手动调用 fclose
//
// 类比你学过的 C++ 文件操作：
//   fopen(path, mode)   ≈  ofstream out(path) 或 ifstream in(path)
//   fclose(file_)        ≈  out.close()
//   fprintf(file_, ...)  ≈  out << ...
//   检查成功 fopen 返回非空 ≈  out.is_open()

class FileGuard
{
private:
    FILE* file_;   // 被管理的文件指针（类似 ofstream 对象，但它只是个指针）

public:
    // 构造 = 获取资源：打开文件
    // fopen 参数:
    //   path — 文件路径，如 "test.txt"
    //   mode — 打开模式，如 "w"(写/覆盖)、"r"(读)、"a"(追加)
    // 返回值: 成功返回 FILE*，失败返回 nullptr（空指针）
    // 类比: 类似 C++ 的 ofstream out(path)，用 out.is_open() 判断成败
    FileGuard(const char* path, const char* mode)
    {
        file_ = fopen(path, mode);      // 对应 C++: out.open(path)
        if (file_)                       // 非空 = 打开成功（对应 out.is_open()）
            cout << "[FileGuard] 文件已打开: " << path << endl;
        else
            cout << "[FileGuard] 文件打开失败: " << path << endl;
    }

    // 析构 = 释放资源：关闭文件（保证不会忘记！）
    ~FileGuard()
    {
        if (file_)                       // 如果文件还开着
        {
            fclose(file_);               // 对应 C++: out.close()
            cout << "[FileGuard] 文件已自动关闭\n";
        }
    }

    // 禁止拷贝（文件句柄不能简单复制）
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    // 允许移动
    FileGuard(FileGuard&& other) noexcept : file_(other.file_)
    {
        other.file_ = nullptr;           // 置空源对象的指针（等于 nullptr = 不指向任何文件）
        cout << "[FileGuard] 移动构造 —— 所有权转移\n";
    }

    // 写一行内容到文件
    // fprintf 参数：FILE*、格式字符串、数据
    // "%s\n" 是格式字符串：%s=字符串占位, \n=换行
    // text.c_str() 把 C++ string 转成 C 风格字符串(const char*)
    // 类比: C++ 的 out << text << endl;
    void write(const string& text)
    {
        if (file_)
            fprintf(file_, "%s\n", text.c_str());
    }

    // 检查文件是否成功打开
    // FILE* 为 nullptr = 打开失败（类比 !out.is_open()）
    bool is_open() const { return file_ != nullptr; }

    // 获取原始指针（通常不需要，暴露这个接口只是备用）
    FILE* get() const { return file_; }
};

void lesson1_RAII()
{
    cout << "========== Lesson 1: RAII 思想 ==========\n\n";

    // ----- 手动管理（危险）-----
    cout << "--- 手动管理 FILE* ---\n";
    {
        // fopen("文件名", "w") — "w"=write 模式，文件不存在则创建，存在则清空
        // 返回 FILE* 指针，失败返回 nullptr
        FILE* f = fopen("test_manual.txt", "w");    // 对应 C++: ofstream f("test_manual.txt")
        if (f)                                        // 对应 C++: if (f.is_open())
            fprintf(f, "手动写入一行\n");              // 对应 C++: f << "手动写入一行" << endl;
        // 如果中间某个函数 return 了、或者抛异常了……
        // fclose(f) 可能永远不会被执行 → 文件句柄泄漏！
        fclose(f);                                     // 对应 C++: f.close()
        cout << "手动 fclose 完成（但如果中间 return 了就到不了这里）\n";
    }

    // ----- RAII 管理（安全）-----
    cout << "\n--- RAII 管理 FileGuard ---\n";
    {
        FileGuard fg("test_raii.txt", "w");
        if (fg.is_open())
        {
            fg.write("RAII 写入第一行");
            fg.write("RAII 写入第二行");
        }
        // 即使这里 return 或 throw，fg 析构时一定会 fclose！
        cout << "离开作用域前……\n";
    }   // ← fg 在这里析构，自动关闭文件
    cout << "已经离开作用域，文件自动关闭\n";

    cout << "\n【RAII 的好处】\n";
    cout << "  1. 不用手动释放 → 不会忘\n";
    cout << "  2. 提前 return / 抛异常也安全 → 栈展开会自动析构\n";
    cout << "  3. 代码更短更清晰\n";
}

// ============================================================
// Lesson 2: unique_ptr —— 独占的智能指针
// ============================================================
//
// unique_ptr 是 C++11 引入的智能指针，用来替代裸指针 new/delete。
//
// 三个关键词理解它：
//   「独占」— 同一时刻只有一个 unique_ptr 拥有那个对象
//   「自动」— unique_ptr 析构时自动 delete 对象
//   「零开销」— 大小和性能跟裸指针一样
//
// 语法：
//   unique_ptr<int> p = make_unique<int>(42);  // 推荐写法
//   p.reset();    // 手动释放
//   p.get();      // 获取裸指针
//   *p            // 解引用，得到对象本身
//
// 不能做什么：
//   unique_ptr<int> p2 = p;       // ❌ 不能拷贝！
//   因为"独占"，不能有两个 unique_ptr 指向同一个对象

struct Student
{
    string name;
    int age;
    Student(string n, int a) : name(n), age(a)
    {
        cout << "  [构造] Student: " << name << " " << age << "岁\n";
    }
    ~Student() { cout << "  [析构] Student: " << name << "\n"; }

    void introduce() const
    {
        cout << "  我叫" << name << "，" << age << "岁\n";
    }
};

void lesson2_unique_ptr基础()
{
    cout << "\n========== Lesson 2: unique_ptr 基本使用 ==========\n\n";

    // ----- 创建 unique_ptr -----
    cout << "--- 创建 unique_ptr ---\n";
    {
        // 方式①：make_unique（推荐）
        unique_ptr<int> p1 = make_unique<int>(42);
        cout << "*p1 = " << *p1 << endl;

        // 方式②：用 new（不推荐，可能漏写 delete）
        unique_ptr<int> p2(new int(100));
        cout << "*p2 = " << *p2 << endl;

        // 方式③：管理自定义类型
        unique_ptr<Student> p3 = make_unique<Student>("张三", 20);
        p3->introduce();       // -> 像指针一样访问成员

        // 离开作用域，三个 unique_ptr 自动 delete
        cout << "离开作用域，自动释放...\n";
    }

    // ----- 修改指向的值 -----
    cout << "\n--- 修改值 ---\n";
    {
        unique_ptr<int> p = make_unique<int>(10);
        cout << "原来: " << *p << endl;
        *p = 999;                       // 像普通指针一样解引用修改
        cout << "改后: " << *p << endl;
    }

    // ----- release：放弃所有权（不 delete）-----
    cout << "\n--- release()：放弃所有权 ---\n";
    {
        unique_ptr<int> p = make_unique<int>(50);
        cout << "p 管理的值: " << *p << endl;

        int* raw = p.release();  // p 放弃所有权，raw 现在持有裸指针
        // p 现在是 nullptr
        cout << "release 后，p.get() = " << p.get() << "（nullptr）\n";
        // raw 必须手动 delete，否则泄漏！
        cout << "raw 指向: " << *raw << "（必须手动 delete）\n";
        delete raw;
    }

    // ----- reset：释放旧对象（delete），可选指向新对象 -----
    cout << "\n--- reset()：替换管理的对象 ---\n";
    {
        unique_ptr<int> p = make_unique<int>(10);
        cout << "p = " << *p << endl;

        p.reset(new int(20));    // 释放旧的 10，接管新的 20
        cout << "reset 后 p = " << *p << endl;

        p.reset();               // 释放 20，p 变成 nullptr
        cout << "reset() 后 p = " << p.get() << "（nullptr）\n";
    }

    cout << "\n【make_unique vs new】\n";
    cout << "  make_unique<T>(args): 安全，参数直接传给 T 的构造\n";
    cout << "  new T:                裸露的 new，容易忘了配 delete\n";
    cout << "  优先用 make_unique！\n";
}

// ============================================================
// Lesson 3: unique_ptr 的移动语义
// ============================================================
//
// unique_ptr 不允许拷贝（=delete），但允许移动（std::move）。
// 移动 = 所有权转移：我不用了，给你用。
//
// 这在以下场景很有用：
//   1. 工厂函数：函数内部创建对象，把所有权交给调用者
//   2. 存入容器：push_back(make_unique<T>())
//   3. 传递参数：把所有权从一处转移到另一处

// 工厂函数：创建 Student 并返回它的 unique_ptr
unique_ptr<Student> createStudent(string name, int age)
{
    // 创建对象，把所有权 "移动" 给调用者
    return make_unique<Student>(name, age);
    // return 语句自动触发移动（不需要 std::move）
}

// 接收 unique_ptr 作为参数 → 拿走所有权
void takeOwnership(unique_ptr<Student> s)
{
    cout << "  函数接收了 " << s->name << " 的所有权\n";
    // s 是局部变量，函数结束时会 delete Student
}   // ← s 析构，Student 被 delete

void lesson3_move语义()
{
    cout << "\n========== Lesson 3: unique_ptr 移动语义 ==========\n\n";

    // ----- 基本移动 -----
    cout << "--- 基本移动 ---\n";
    {
        unique_ptr<int> p1 = make_unique<int>(100);
        cout << "p1 = " << *p1 << endl;

        // unique_ptr<int> p2 = p1;     // ❌ 编译错误！不能拷贝
        unique_ptr<int> p2 = move(p1);  // ✅ 移动：所有权从 p1 转到 p2

        cout << "移动后 p2 = " << *p2 << endl;
        // p1 现在是 nullptr，不能再解引用！
        cout << "p1.get() = " << (void*)p1.get() << "（nullptr）\n";
    }

    // ----- 工厂函数返回 unique_ptr -----
    cout << "\n--- 工厂函数返回 unique_ptr ---\n";
    {
        unique_ptr<Student> s = createStudent("李四", 22);
        // s 拥有 Student 对象的所有权
        s->introduce();
    }   // s 析构，Student 被释放

    // ----- 把所有权传给函数 -----
    cout << "\n--- 把所有权传给函数 ---\n";
    {
        unique_ptr<Student> s = make_unique<Student>("王五", 25);
        cout << "调用前 s 有值: " << s.get() << endl;

        // takeOwnership(s);           // ❌ 不能拷贝！
        takeOwnership(move(s));        // ✅ 所有权转移进函数

        cout << "调用后 s.get() = " << s.get() << "（nullptr）\n";
        // Student 在 takeOwnership 函数内部被释放了
    }

    // ----- 存入 vector -----
    cout << "\n--- 存入 vector<unique_ptr<int>> ---\n";
    {
        vector<unique_ptr<int>> nums;
        nums.push_back(make_unique<int>(1));
        nums.push_back(make_unique<int>(2));
        nums.push_back(make_unique<int>(3));

        cout << "遍历：";
        for (auto& p : nums)        // 必须用 &，否则会尝试拷贝 unique_ptr
            cout << *p << " ";
        cout << endl;
    }

    cout << "\n【unique_ptr 的移动规则总结】\n";
    cout << "  不能拷贝 → 保证同一时刻只有一个所有者\n";
    cout << "  可以移动 → 主动转移所有权，用完即弃\n";
}

// ============================================================
// Lesson 4: unique_ptr + 多态
// ============================================================
//
// 第 3 周 Day 5 我们学过：vector<Animal*> 存裸指针能多态但需要手动 delete。
// 现在用 unique_ptr<Animal> 替代 Animal*，既能多态又能自动释放！
//
// 对比：
//   vector<Animal*>              手动 delete，容易忘 → 内存泄漏
//   vector<unique_ptr<Animal>>   自动 delete，安全！

class Animal
{
public:
    string type_;
    Animal(string t) : type_(t) {}
    virtual ~Animal() = default;           // 虚析构：保证子类析构被调用
    virtual void speak() const { cout << "???" << endl; }
};

class Dog : public Animal
{
public:
    Dog() : Animal("狗") {}
    void speak() const override { cout << "汪汪！" << endl; }
};

class Cat : public Animal
{
public:
    Cat() : Animal("猫") {}
    void speak() const override { cout << "喵喵！" << endl; }
};

// 创建动物工厂
unique_ptr<Animal> createAnimal(int type)
{
    if (type == 1)
        return make_unique<Dog>();
    else
        return make_unique<Cat>();
}

// 接收 vector<unique_ptr<Animal>> 的引用（必须用引用！）
void showAllAnimals(const vector<unique_ptr<Animal>>& animals)
{
    cout << "动物们依次发言：\n";
    for (auto& a : animals)      // auto& 不能省略 &
        a->speak();
}

// 按值接收 unique_ptr（需要 move）
void adoptAnimal(unique_ptr<Animal> pet)
{
    cout << "  领养了一只" << pet->type_ << endl;
    pet->speak();
    // pet 析构时自动释放
}

void lesson4_多态()
{
    cout << "\n========== Lesson 4: unique_ptr + 多态 ==========\n\n";

    // ----- 创建多态对象 -----
    cout << "--- 创建多态对象 ---\n";
    {
        unique_ptr<Animal> dog = make_unique<Dog>();    // 基类指针指向子类对象
        unique_ptr<Animal> cat = make_unique<Cat>();

        dog->speak();   // 多态：Dog::speak()
        cat->speak();   // 多态：Cat::speak()

        // 离开作用域，两个 unique_ptr 自动 delete，且因为虚析构，
        // 子类析构函数会被正确调用
    }

    // ----- 工厂函数 -----
    cout << "\n--- 工厂函数创建动物 ---\n";
    {
        unique_ptr<Animal> a1 = createAnimal(1);  // 返回 Dog
        unique_ptr<Animal> a2 = createAnimal(2);  // 返回 Cat
        a1->speak();
        a2->speak();
    }

    // ----- vector 存多态对象 -----
    cout << "\n--- vector<unique_ptr<Animal>> ---\n";
    {
        vector<unique_ptr<Animal>> zoo;
        zoo.push_back(make_unique<Dog>());
        zoo.push_back(make_unique<Cat>());
        zoo.push_back(make_unique<Dog>());

        showAllAnimals(zoo);     // 传引用

        // 领养一只
        adoptAnimal(move(zoo[0]));  // 所有权转移，zoo[0] 变为 nullptr

        cout << "领养后 zoo[0] = " << zoo[0].get() << "（nullptr）\n";
    }   // 剩下的动物自动释放

    cout << "\n【unique_ptr + 多态 总结】\n";
    cout << "  基类必须有虚析构 virtual ~Animal()=default;\n";
    cout << "  用 make_unique<Dog>() 创建，存到 unique_ptr<Animal>\n";
    cout << "  传 vector 时必须用引用 &（unique_ptr 不能拷贝）\n";
    cout << "  离开作用域自动 delete，告别内存泄漏！\n";
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_RAII();
    lesson2_unique_ptr基础();
    lesson3_move语义();
    lesson4_多态();

    return 0;
}
