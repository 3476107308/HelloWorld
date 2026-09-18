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
// Day 5: 值语义与迭代器失效
// 对应视频: vector 容器 P194-P196
// https://www.bilibili.com/video/BV1et411b73Z/?p=194
// ============================================================

// ============================================================
// Lesson 1: 值语义 —— 证明 vector 存的是"副本"
// ============================================================
//
// C++ 容器的核心设计原则：值语义(value semantics)
// 当你把对象放进容器时，容器里存的是这个对象的"副本"。
// 每次 push_back、insert 都会触发拷贝构造或移动构造。
//
// 这意味着：
// - 原对象和容器里的对象是独立的，互不影响
// - 如果对象很大，拷贝开销也很大
// - 这就是为什么有时候我们选择存指针而不是存对象
//
// 下面这个 Person 类在每个特殊函数被调用时打印日志，
// 让你亲眼看到拷贝、移动到底发生了多少次。

class Person
{
public:
    string name_;

    // ① 普通构造：创建一个新对象
    Person(string name) : name_(name)
    {
        cout << "  [构造] " << name_ << endl;
    }

    // ② 析构：对象销毁时调用
    ~Person()
    {
        cout << "  [析构] " << name_ << endl;
    }

    // ③ 拷贝构造：用另一个对象来"克隆"一个新对象
    //    形式：Person p2(p1);  或  Person p2 = p1;
    Person(const Person& other) : name_(other.name_ + "(副本)")
    {
        cout << "  [拷贝构造] 从 " << other.name_ << " 克隆出 " << name_ << endl;
    }

    // ④ 拷贝赋值：把另一个对象的值"覆盖"到已有对象上
    //    形式：p1 = p2;  (两个对象都已经存在)
    Person& operator=(const Person& other)
    {
        cout << "  [拷贝赋值] " << name_ << " 被 " << other.name_ << " 覆盖" << endl;
        name_ = other.name_ + "(被覆盖)";
        return *this;
    }

    // ⑤ 移动构造：把另一个对象的资源"抢"过来
    //    形式：Person p2(std::move(p1));
    //    临时对象(右值)自动匹配移动构造，比拷贝高效
    Person(Person&& other) noexcept : name_(std::move(other.name_))
    {
        other.name_ = "(已被移走)";
        cout << "  [移动构造] 从 " << other.name_ << " 移动资源" << endl;
    }
};

void lesson1_值语义()
{
    cout << "========== Lesson 1: 值语义 ==========\n\n";

    // 1.1 创建对象 → 存入 vector → 观察拷贝
    cout << "--- 1.1 push_back 触发拷贝 ---\n";
    Person p1("张三");
    vector<Person> v1;
    v1.push_back(p1);
    // push_back(const T&) → 调用了拷贝构造
    // vector 里存的是副本，不是 p1 本身

    // 1.2 push_back 临时对象 → 触发移动构造
    cout << "\n--- 1.2 push_back 临时对象触发移动 ---\n";
    v1.push_back(Person("李四"));
    // Person("李四") 是临时对象(右值)，优先匹配移动构造
    // 移动比拷贝高效：只"偷"资源，不复制数据

    // 1.3 扩容触发批量拷贝
    cout << "\n--- 1.3 push_back 触发扩容，旧元素被拷贝 ---\n";
    v1.push_back(Person("王五"));
    v1.push_back(Person("赵六"));
    v1.push_back(Person("孙七"));
    // 当 size > capacity 时，vector 扩容：
    //   ① 申请一块更大的新内存（通常是原来的2倍）
    //   ② 把旧内存里的元素逐个拷贝/移动到新内存
    //   ③ 释放旧内存
    // → 每扩容一次，旧元素都要被拷贝一遍！

    // 1.4 证明 vector 里存的是独立副本
    cout << "\n--- 1.4 副本是独立的 ---\n";
    p1.name_ = "张三(原对象改名了)";
    cout << "p1.name_       = " << p1.name_ << endl;
    cout << "v1[0].name_    = " << v1[0].name_ << endl;
    // vector 里的没变 → 证明确实是独立副本

    cout << "\n--- lesson1 结束 ---\n";
}

// ============================================================
// Lesson 2: 迭代器失效
// ============================================================
//
// 迭代器指向容器内部某个位置的内存。
// 当容器的内存布局发生变化时（比如扩容换了更大的内存块），
// 旧的迭代器还指向已经被释放的旧内存 → "野指针"
// 再用它就是未定义行为，程序可能崩溃。
//
// 四种常见失效场景：
//  ① vector push_back 导致扩容 → 所有迭代器失效
//  ② vector insert 导致扩容或移位 → 插入位置之后的迭代器失效
//  ③ vector erase 删除元素 → 被删位置及之后的迭代器失效
//  ④ map/set erase 删除元素 → 仅被删位置的迭代器失效

void lesson2_迭代器失效()
{
    cout << "\n========== Lesson 2: 迭代器失效 ==========\n\n";

    // ----- 场景①：push_back 扩容 -----
    cout << "--- 场景①: push_back 扩容导致迭代器失效 ---\n";
    vector<int> v{1, 2, 3, 4, 5};
    cout << "初始 size=" << v.size() << " capacity=" << v.capacity() << endl;

    auto it = v.begin() + 2;  // it 指向第3个元素，即 3
    cout << "*it = " << *it << " (此时有效)\n";

    cout << "push_back 100 次...\n";
    for (int i = 0; i < 100; i++)
        v.push_back(i);

    cout << "扩容后 size=" << v.size() << " capacity=" << v.capacity() << endl;
    cout << "原来的 it 已经指向被释放的旧内存，是野指针！\n";
    cout << "再访问 *it 会崩溃或读到垃圾值。\n";

    // 正确做法：扩容后重新获取迭代器
    auto it_new = v.begin() + 2;
    cout << "重新获取后 *it_new = " << *it_new << " (正确)\n";

    // ----- 场景②：erase 删除元素 -----
    cout << "\n--- 场景②: erase 导致迭代器失效 ---\n";
    vector<int> v2{10, 20, 30, 40, 50};
    cout << "v2 原始：";
    for (auto x : v2) cout << x << " ";
    cout << "\n";

    // 错误写法（注释掉以免真的崩溃）：
    // for (auto it = v2.begin(); it != v2.end(); ++it) {
    //     if (*it == 30)
    //         v2.erase(it);  // it 失效！下一轮的 ++it 是未定义行为！
    // }

    // 正确写法：erase 返回下一个有效迭代器
    cout << "删除 30 后：";
    for (auto it = v2.begin(); it != v2.end(); )  // 注意：没有 ++it
    {
        if (*it == 30)
            it = v2.erase(it);  // erase 返回被删元素的下一个迭代器
        else
            ++it;               // 不删才自增
    }
    for (auto x : v2) cout << x << " ";
    cout << "\n";

    // ----- 场景③：insert 导致扩容 -----
    cout << "\n--- 场景③: insert 导致扩容 ---\n";
    vector<int> v3{1, 2, 3};
    auto it3 = v3.begin();
    cout << "插入前 *it3 = " << *it3 << "，capacity=" << v3.capacity() << endl;

    // insert 可能触发扩容，it3 失效
    v3.insert(v3.begin() + 1, 999);
    cout << "插入后 capacity=" << v3.capacity() << " (可能变了)\n";
    cout << "it3 可能已失效，不要再用\n";

    cout << "\n【迭代器使用原则】\n";
    cout << "  对容器做了增删操作后，之前获取的迭代器统统算作失效。\n";
    cout << "  安全做法：用完就扔，下次用的时候重新从 begin()/end() 获取。\n";
}

// ============================================================
// Lesson 3: 存对象 vs 存指针
// ============================================================
//
// 现在你知道 vector 存对象会有拷贝开销，那能不能存指针？
// 三种方式各有利弊：
//
//  ┌──────────────────┬──────────────────┬──────────────────┐
//  │ vector<Person>   │ vector<Person*>  │ vector<unique_ptr│
//  ├──────────────────┼──────────────────┼──────────────────┤
//  │ 存副本           │ 存裸指针         │ 存智能指针       │
//  │ 自动析构          │ 需手动delete     │ 自动delete       │
//  │ push_back 有拷贝 │ push_back 很便宜 │ push_back 需移动 │
//  │ 不支持多态        │ 支持多态         │ 支持多态         │
//  │ 适合小对象        │ 危险，不推荐      │ 推荐！           │
//  └──────────────────┴──────────────────┴──────────────────┘

// 用于演示多态的继承体系
class Animal
{
public:
    string type_;
    Animal(string t) : type_(t) {}
    virtual ~Animal() = default;           // 虚析构：保证子类析构被调用
    virtual void speak() { cout << "???" << endl; }
};

class Dog : public Animal
{
public:
    Dog() : Animal("狗") {}
    void speak() override { cout << "汪汪！" << endl; }
};

class Cat : public Animal
{
public:
    Cat() : Animal("猫") {}
    void speak() override { cout << "喵喵！" << endl; }
};

void lesson3_存对象vs存指针()
{
    cout << "\n========== Lesson 3: 存对象 vs 存指针 ==========\n\n";

    // ----- 方式①：存对象 -----
    cout << "--- 方式①: vector<Dog> 存对象 ---\n";
    {
        vector<Dog> dogs;
        Dog d1;
        dogs.push_back(d1);   // 存的是 d1 的副本
        dogs[0].speak();      // 正常调用
        // 离开作用域，vector 自动析构，不需要 delete
        cout << "自动析构，安全！\n";
    }
    // 问题：不支持多态！vector<Dog> 只能存 Dog，不能存 Cat

    // ----- 方式②：存裸指针 -----
    cout << "\n--- 方式②: vector<Animal*> 存裸指针 ---\n";
    {
        vector<Animal*> animals;
        animals.push_back(new Dog());  // new 在堆上创建对象，push_back 只存8字节指针
        animals.push_back(new Cat());

        cout << "多态遍历：\n";
        for (auto a : animals)
            a->speak();   // 多态生效！Dog 汪汪，Cat 喵喵

        // ⚠ 致命问题：必须手动 delete！
        // 如果下面的循环忘了写，new 出来的 Dog 和 Cat 永远不会被释放
        // 这叫"内存泄漏"，程序跑久了内存越占越多
        for (auto a : animals)
            delete a;
        cout << "手动 delete 完毕\n";
    }

    // ----- 方式③：存智能指针（C++11 推荐） -----
    cout << "\n--- 方式③: vector<unique_ptr<Animal>> 存智能指针 ---\n";
    {
        vector<unique_ptr<Animal>> animals;
        animals.push_back(make_unique<Dog>());  // make_unique = new + 包装成unique_ptr
        animals.push_back(make_unique<Cat>());

        cout << "多态遍历：\n";
        for (auto& a : animals)  // 必须是引用 &，因为 unique_ptr 不能拷贝
            a->speak();

        // 离开作用域，vector 析构 → unique_ptr 自动 delete
        // 既能多态，又不会内存泄漏！
        cout << "离开作用域，unique_ptr 自动释放内存，完美！\n";
    }

    cout << "\n【选型建议】\n";
    cout << "  小对象 / 不需要多态   → vector<对象>\n";
    cout << "  大对象 / 需要多态     → vector<unique_ptr<基类>>\n";
    cout << "  裸指针 vector<T*>     → 尽量不要用\n";
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_值语义();
    lesson2_迭代器失效();
    lesson3_存对象vs存指针();

    return 0;
}
