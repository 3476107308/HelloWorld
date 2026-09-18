#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<cstdio>
#include<windows.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::move;

// ============================================================
// Day 4 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：RAII 文件管理类 FileGuard
// -----------------------------------------------------------
// 要求：
//   - 类名：FileGuard
//   - 私有成员：FILE* file_（C 语言文件指针）
//       FILE 是 C 标准库的文件结构体，FILE* 指向一个打开的文件
//       类比 C++: FILE* ≈ ofstream 对象，但 FILE* 不会自动关闭
//
//   - 构造函数 FileGuard(const char* path, const char* mode)
//       fopen(path, mode) 打开文件，成功返回 FILE*，失败返回 nullptr
//       path: 文件路径如 "test.txt"
//       mode: "w"(写/覆盖) "r"(读) "a"(追加)
//       类比 C++: ofstream out(path)
//
//   - 析构函数 ~FileGuard()
//       如果 file_ 不是 nullptr，fclose(file_) 关闭文件
//       类比 C++: out.close()
//
//   - void write(const string& text)
//       如果 file_ 不为空，用 fprintf 写入
//       fprintf(file_, "%s\n", text.c_str())
//         %s = 字符串占位符, \n = 换行
//         .c_str() = 把 C++ string 转成 C 字符串(const char*)
//       类比 C++: out << text << endl;
//
//   - bool is_open() const
//       返回 file_ != nullptr（非空=成功，空=失败）
//
//   - 禁止拷贝（拷贝构造和拷贝赋值都 = delete）
//   - 允许移动（移动构造把 other.file_ 偷过来，other.file_ 置为 nullptr）
//
//  提示：需要 #include <cstdio>
//
// 在 test_FileGuard() 中测试：
//   - 创建一个 FileGuard，写入两行文字
//   - 用 is_open() 检查是否成功
//   - 观察离开作用域时是否自动打印关闭信息

// TODO: 写 FileGuard 类

class FileGuard
{
private:
    FILE* file_;
public:
    FileGuard(const char* path,const char* mode)
    {
        file_ = fopen(path,mode);
        if(file_ == nullptr)
        cout << "文件打开失败\n";
        else
        cout << "文件打开成功\n";
    }
    ~FileGuard()
    {
        if(file_ != nullptr)
        {
            fclose(file_);
            cout << "文件已经自动关闭\n";
        }
    }
    void write(const string& text)
    {
        if(file_ != nullptr)
        {
            fprintf(file_,"%s\n",text.c_str());
        }
        else
        return;
    }
    bool is_open() const{return file_ != nullptr;}
    FileGuard(const FileGuard& other) = delete;
    FileGuard& operator=(const FileGuard& other) = delete;
    FileGuard(FileGuard&& other)noexcept:file_(other.file_)
    {
        other.file_ = nullptr;
        cout << "[FileGuard 移动构造   所有权转移]\n";
    }
    FileGuard& operator=(FileGuard&& other)noexcept
    {
        if(this == &other)
        return *this;

        if(file_ != nullptr)
        fclose(file_);

        file_ = other.file_;
        other.file_ = nullptr;

        cout << "[FileGuard 移动赋值   所有权转移]\n";
        return *this;
    }

};
void test_FileGuard()
{
    // TODO: 测试 FileGuard 自动关闭文件
    cout << "作用域外\n";
    {
        FileGuard f1("test1.txt","w");
        cout << f1.is_open() << endl;
        f1.write("第一行文字:王静是人机\n");
        f1.write("第二行文字:算了,王静不是人机\n");
    }
    cout << "作用域外\n";

}

// -----------------------------------------------------------
// 需求 2：unique_ptr 基本使用
// -----------------------------------------------------------
// 写一个简单类 Person，有两个字段：
//   - string name_
//   - int age_
// 要求：
//   - 构造函数打印 "[构造] Person: name age"
//   - 析构函数打印 "[析构] Person: name"
//   - void introduce() const 打印 "我叫xxx，xx岁"
//
// 在 test_unique_ptr_basic() 中完成：
//   - 用 make_unique<Person> 创建一个 Person 对象
//   - 调用 introduce()
//   - 用 make_unique<int> 创建一个 int，值为 100
//   - 修改这个 int 的值为 200，打印验证
//   - 测试 reset()：把 unique_ptr<int> reset 为一个新值，打印新旧值的变化

// TODO: 写 Person 类

class Person
{
private:
    string name_;
    int age_;
public:
    Person(string name,int age):name_(name),age_(age)
    {
        cout << "[构造] Person: name " << name_ << " age " << age_ << endl;
    }
    ~Person()
    {
        cout << "[析构] Person: name " << name_ << endl; 
    }
    void introduce()const
    {
        cout << "我叫" << name_ << "," << age_ << "岁\n";
    }
};
void test_unique_ptr_basic()
{
    // TODO: 测试 unique_ptr 基本操作
    unique_ptr<Person> p1 = make_unique<Person>("陈岗",19);
    p1->introduce();
    unique_ptr<int> p2 = make_unique<int>(100);
    *p2 = 200;
    cout << *p2 << endl;
    p2.reset(new int (22));
    cout << *p2 << endl;
}

// -----------------------------------------------------------
// 需求 3：unique_ptr 移动语义
// -----------------------------------------------------------
// 在 test_move_semantics() 中完成：
//
//   ① 创建 unique_ptr<int> p1 = make_unique<int>(42)
//   ② 用 std::move 把 p1 移动到 p2
//   ③ 打印 p2 的值，打印 p1.get() 验证它已经变成 nullptr
//
//   ④ 写一个工厂函数 makePerson(string name, int age)
//      返回 unique_ptr<Person>（用 make_unique 创建）
//      在 test 里调用 makePerson，接收返回值并调用 introduce()
//
//   ⑤ 写一个函数 takePerson(unique_ptr<Person> p)
//      函数内调用 p->introduce()
//      在 test 里用 std::move 把 Person 传给 takePerson
//      调用后打印原来的 unique_ptr.get()，验证它是 nullptr

// TODO: 写 makePerson 函数
unique_ptr<Person> makePerson(string name,int age)
{
    return make_unique<Person>(name,age);
}
// TODO: 写 takePerson 函数
void takePerson(unique_ptr<Person> p)
{
    p->introduce();
}
void test_move_semantics()
{
    // TODO: 测试移动语义
    unique_ptr<int> p1 = make_unique<int>(42);
    unique_ptr<int> p2 = std::move(p1);
    cout << *p2 << endl;
    cout << "p1 = " << (void*)p1.get() << endl;
    auto p3 = makePerson("张三",10);
    p3->introduce();
    takePerson(move(p3));
    cout << p3.get() << endl;
}

// -----------------------------------------------------------
// 需求 4（综合）：unique_ptr + 多态
// -----------------------------------------------------------
// 定义一个动物继承体系：
//   - 基类 Animal：
//       string type_
//       virtual ~Animal() = default
//       virtual void speak() const（打印 "???"）
//
//   - 子类 Dog：type_("狗")，speak() 打印 "汪汪！"
//   - 子类 Cat：type_("猫")，speak() 打印 "喵喵！"
//
class Animal
{
private:
    string type_;
public:
    Animal(string type):type_(type){}
    virtual ~Animal() = default;
    virtual void speak() const {cout << "???\n";}
};

class Dog:public Animal
{
public:
    Dog():Animal("狗"){}
    void speak()const override
    {
        cout << "汪汪！\n";
    }

};

class Cat:public Animal
{
public:
    Cat():Animal("猫"){}
    void speak()const override
    {
        cout << "喵喵!\n";
    }
};
// 写一个工厂函数：
//   unique_ptr<Animal> createAnimal(int type)
//       type==1 返回 make_unique<Dog>()
//       否则返回 make_unique<Cat>()
//
unique_ptr<Animal> createAnimal(int type)
{
    if(type == 1)
    return make_unique<Dog>();
    else
    return make_unique<Cat>();
}
// 写一个打印函数：
//   void showAll(const vector<unique_ptr<Animal>>& animals)
//       遍历所有动物，调用 speak()（注意：遍历时用 auto&）
//
void showAll(const vector<unique_ptr<Animal>>& animals)
{
    for(auto& x:animals)
    {
        if(x != nullptr)
        x->speak();
        else
        cout << "已经被移动了\n";
    }
}
// 在 test_polymorphism() 中：
//   - 用工厂函数创建 3 个动物（2 狗 1 猫），push 到 vector<unique_ptr<Animal>>
//   - 调用 showAll 打印
//   - 用 std::move 把 vector 的第 0 个元素"领养"走（移到另一个 unique_ptr）
//   - 打印领养后原位置的 get()，验证变成 nullptr
//   - 再次调用 showAll（只剩两只动物）

// TODO: 写 Animal、Dog、Cat 类

// TODO: 写 createAnimal 工厂函数

// TODO: 写 showAll 函数

void test_polymorphism()
{
    // TODO: 综合测试 unique_ptr + 多态
    vector<unique_ptr<Animal>> v;
    auto p1 = createAnimal(1);
    auto p2 = createAnimal(1);
    v.push_back(move(p1));
    v.push_back(move(p2));
    v.push_back(createAnimal(0));
    showAll(v);
    auto p4 = move(v[0]);
    cout << v[0].get() << endl;
    showAll(v);

}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    test_FileGuard();
    test_unique_ptr_basic();
    test_move_semantics();
    test_polymorphism();

    return 0;
}
