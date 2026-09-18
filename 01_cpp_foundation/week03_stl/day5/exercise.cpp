#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;

// ============================================================
// Day 5 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：完成 Person 类的五个特殊函数（值语义演示）
// -----------------------------------------------------------
// 要求：
//   - 成员：string name_
//   - ① 普通构造：接收 string，初始化 name_，打印 "[构造] xxx"
//   - ② 析构：打印 "[析构] xxx"
//   - ③ 拷贝构造：name_ = other.name_ + "(副本)"，打印 "[拷贝构造] ..."
//   - ④ 拷贝赋值：打印 "[拷贝赋值] ..."
//   - ⑤ 移动构造：用 std::move 偷走 other.name_，
//                 把 other.name_ 设为 "(已被移走)"，打印 "[移动构造] ..."

class Person
{
public:
    string name_;

    // TODO: 写 ① 构造
    Person(string name):name_(name)
    {
        cout << name_ << "已经被构造" << endl;
    }
    // TODO: 写 ② 析构
    ~Person(){cout << name_ << "被析构\n";}
    // TODO: 写 ③ 拷贝构造  (提示：参数是 const Person& other)
    Person(const Person& other):name_("拷贝构造的" + other.name_)
    {
        cout << name_ << "被拷贝构造\n";
    }
    // TODO: 写 ④ 拷贝赋值    (提示：返回 Person&，参数是 const Person& other)
    Person& operator=(const Person& other)
    {
        name_ = "拷贝赋值的" + other.name_;
        cout << name_ << "被移动构造\n";
        return *this;
    }
    // TODO: 写 ⑤ 移动构造    (提示：参数是 Person&& other，加 noexcept)
    Person(Person&& other):name_(std::move(other.name_))
    {
        cout << name_ << "被移动构造\n";
        other.name_ = "（已被移走）"; // 这行一定要写 相当于给other打上标记了 这样访问时 可以知道 这个地址的资源已经被移走了 如果这个资源在堆区开辟 则要进行释放
    }
};

// -----------------------------------------------------------
// 需求 2：演示 vector 值语义
// -----------------------------------------------------------
// 在这个函数中：
//   2.1 创建一个 Person p1("张三")
//   2.2 创建一个空的 vector<Person> v
//   2.3 用 v.push_back(p1)，观察控制台有没有打印拷贝构造
//   2.4 用 v.push_back(Person("李四"))，观察控制台是拷贝还是移动
//   2.5 修改 p1.name_ = "张三改名"
//       然后分别打印 p1.name_ 和 v[0].name_
//       观察它们是否一致 → 证明 vector 存的是独立副本

void test_值语义()
{
    // TODO: 完成需求 2.1 ~ 2.5
    Person p1("张三");
    vector<Person> v;
    v.push_back(p1);
    v.push_back(Person("李四"));
    p1.name_ = "张三（改名后）";
    cout << "p1.name_ = " << p1.name_ << "  v[0].name_ = " << v[0].name_ << endl;
    cout << endl; 
}

// -----------------------------------------------------------
// 需求 3：演示迭代器失效
// -----------------------------------------------------------
// 在这个函数中：
//   3.1 创建一个 vector<int> v{1,2,3,4,5}
//   3.2 打印初始的 size() 和 capacity()
//   3.3 获取迭代器 auto it = v.begin() + 2，打印 *it
//   3.4 用 for 循环 push_back 100 个整数，让 vector 扩容
//   3.5 再次打印 size() 和 capacity()
//   3.6 用注释说明：此时 it 为什么不能再用？
//   3.7 重新获取迭代器 it_new = v.begin() + 2，打印 *it_new
//
//   3.8 用正确的写法删除 vector 中值为 30 的元素：
//       v2{10,20,30,40,50}，删除 30，遍历打印结果
//       注意：要用 it = v2.erase(it) 接收返回值，不能直接 erase 后 ++it

void test_迭代器失效()
{
    // TODO: 完成需求 3.1 ~ 3.8
    vector<int> v{1,2,3,4,5};
    cout << "没扩容时 size = " << v.size() << " capacity = " << v.capacity() << endl;
    auto it = v.begin() + 2;
    cout << *it << endl;
    for(int i = 1;i <= 100;i++)
    {
        v.push_back(i + 3);
    }
    cout << "扩容后 size = " << v.size() << " capacity = " << v.capacity() << endl;
    /*
    扩容后 v的地址发生了变化 it指向的地址不是现在v的地址 it成了悬垂指针 所以不能访问it
    */ 
    auto it_new = v.begin() + 2;
    cout << *it_new << endl;

    vector<int> v2{10,20,30,40,50};
    cout << "删除前\n";
    for(auto x: v2)
    {
        cout << x << " ";
    } 
    cout << endl;

    for(auto it = v2.begin();it != v2.end();)
    {
        if(*it == 30)
        it = v2.erase(it);
        else
        it++;
    }
    cout << "删除后\n";
    for(auto x:v2)
    {
        cout << x << " ";
    }
    cout << endl;
}

// -----------------------------------------------------------
// 需求 4：对比存对象 vs 存指针
// -----------------------------------------------------------
// 先完成下面的 Animal/Dog/Cat 类（简单继承），然后：
//   4.1 用 vector<Dog> 存对象
//       - 创建一个 Dog 对象，push_back 到 vector
//       - 调用 speak()
//       - 观察离开作用域时是否自动析构（无需手动 delete）
//
//   4.2 用 vector<Animal*> 存裸指针
//       - 用 new Dog() 和 new Cat() 分别 push_back
//       - 用 for 循环遍历，调用 speak()，观察多态是否生效
//       - 用 for 循环手动 delete 每个元素
//       - 用注释说明：如果忘了 delete 会怎样？
//
//   4.3 用 vector<unique_ptr<Animal>> 存智能指针
//       - 用 make_unique<Dog>() 和 make_unique<Cat>() 分别 push_back
//       - 遍历时注意：for (auto& a : animals)，为什么必须是引用 &？
//       - 离开作用域，无需手动 delete

class Animal
{
public:
    string type_;
    Animal(string t) : type_(t) {}
    // TODO: 虚析构
    virtual ~Animal() = default;
    // TODO: 虚函数 speak()，打印 "???"
    virtual void speak()
    {
        cout << "???\n";
    }
};

class Dog : public Animal
{
public:
    Dog() : Animal("狗") {}
    // TODO: override speak()，打印 "汪汪！"
    void speak() override
    {
        cout << "汪汪！\n";
    }
};

class Cat : public Animal
{
public:
    Cat() : Animal("猫") {}
    // TODO: override speak()，打印 "喵喵！"
    void speak() override
    {
        cout << "喵喵！\n";
    }
};

void test_存对象vs存指针()
{
    // TODO: 完成需求 4.1
    vector<Dog> dog;
    Dog d1;
    dog.push_back(d1);
    dog[0].speak();
    // TODO: 完成需求 4.2
    vector<Animal*> animal;
    animal.push_back(new Dog());
    animal.push_back(new Cat());
    for(auto it:animal)
    {
        it->speak();
    }
    for(auto it:animal)
    {
        delete it;
    }
    //vector存指针时 不会自动析构 需要手动析构 
    // TODO: 完成需求 4.3
    vector<unique_ptr<Animal>> an;
    an.push_back(make_unique<Dog>());
    an.push_back(make_unique<Cat>());
    for(auto& a: an)
    {
        a->speak();
    }
    //无需手动delete 也能多态
}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //test_值语义();
    test_迭代器失效();
    test_存对象vs存指针();

    return 0;
}
