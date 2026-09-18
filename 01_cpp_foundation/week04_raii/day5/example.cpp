#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<windows.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;
using std::move;

// ============================================================
// Day 5: shared_ptr 和 weak_ptr
// 对应: Cherno C++ 智能指针 + cppreference shared_ptr/weak_ptr
// ============================================================

// ============================================================
// Lesson 1: shared_ptr —— 共享所有权的智能指针
// ============================================================
//
// unique_ptr: 独占，"这东西是我的，不能给别人"
// shared_ptr: 共享，"这东西大家都可以用，最后一个用完的人负责扔掉"
//
// 核心机制：引用计数（Reference Counting）
//   - 每多一个 shared_ptr 指向同一个对象，计数 +1
//   - 每个 shared_ptr 析构时，计数 -1
//   - 计数归零 → 没有人在用这个对象了 → 自动 delete
//
// 就像一个合租房：
//   - 第一个人搬进来，开灯（new 对象，计数=1）
//   - 第二个人搬进来，灯已经开了（拷贝 shared_ptr，计数=2）
//   - 一个人搬走，不用关灯（shared_ptr 析构，计数=1）
//   - 最后一个人搬走，关灯（计数归零，delete 对象）

struct Fruit
{
    string name_;
    Fruit(string n) : name_(n)
    {
        cout << "  [构造] " << name_ << endl;
    }
    ~Fruit()
    {
        cout << "  [析构] " << name_ << endl;
    }
};

void lesson1_shared_ptr基础()
{
    cout << "========== Lesson 1: shared_ptr 基本使用 ==========\n\n";

    // ----- 创建 shared_ptr -----
    cout << "--- 创建 shared_ptr ---\n";
    {
        // make_shared<T> 是最推荐的创建方式（类似 make_unique）
        shared_ptr<Fruit> p1 = make_shared<Fruit>("苹果");
        // 此时引用计数 = 1

        cout << "  p1.use_count() = " << p1.use_count() << "（只有 p1 在引用）\n";

        {
            shared_ptr<Fruit> p2 = p1;       // 拷贝 = 共享所有权！
            // 此时引用计数 = 2
            cout << "  p2 = p1 后，p1.use_count() = " << p1.use_count() << "（p1 和 p2 共享）\n";

            shared_ptr<Fruit> p3 = p1;       // 再来一个
            cout << "  p3 = p1 后，p1.use_count() = " << p1.use_count() << "（三个共享）\n";

            // 所有人都可以通过自己的 shared_ptr 访问对象
            cout << "  p2->name_ = " << p2->name_ << endl;
            cout << "  p3->name_ = " << p3->name_ << endl;
        }   // p3 和 p2 离开作用域，计数 -2，但 p1 还在，所以计数=1，不 delete

        cout << "  内层作用域结束后，p1.use_count() = " << p1.use_count() << endl;
        cout << "  对象还活着！因为 p1 还在引用它\n";
    }   // p1 离开作用域，计数归零 → 自动 delete
    cout << "  离开作用域，对象被释放\n";

    // ----- 对比 unique_ptr：不能拷贝 -----
    cout << "\n--- 对比：unique_ptr 不能拷贝，shared_ptr 可以 ---\n";
    {
        // unique_ptr<Fruit> u1 = make_unique<Fruit>("香蕉");
        // unique_ptr<Fruit> u2 = u1;       // ❌ 编译错误！不能拷贝
        // unique_ptr<Fruit> u2 = move(u1); // ✅ 只能移动

        shared_ptr<Fruit> s1 = make_shared<Fruit>("香蕉");
        shared_ptr<Fruit> s2 = s1;           // ✅ 可以拷贝，共享所有权
        cout << "  shared_ptr 可以随意拷贝，use_count = " << s1.use_count() << endl;
    }

    // ----- reset 对引用计数的影响 -----
    cout << "\n--- reset() ---\n";
    {
        shared_ptr<Fruit> a = make_shared<Fruit>("橙子");
        shared_ptr<Fruit> b = a;
        cout << "  初始 use_count = " << a.use_count() << endl;

        a.reset();    // a 放弃引用，计数 -1
        cout << "  a.reset() 后，b.use_count() = " << b.use_count() << "（只剩 b 在引用）\n";

        b.reset();    // b 也放弃引用，计数归零 → delete
        cout << "  b.reset() 后，对象被释放\n";
    }

    cout << "\n【shared_ptr 要点】\n";
    cout << "  能拷贝 → 多个 shared_ptr 共享同一个对象\n";
    cout << "  引用计数 → use_count() 查看当前有多少个 shared_ptr 引用它\n";
    cout << "  计数归零 → 自动 delete\n";
    cout << "  make_shared → 推荐创建方式（比 new 高效且安全）\n";
}

// ============================================================
// Lesson 2: shared_ptr 的应用场景
// ============================================================
//
// 什么时候用 shared_ptr？
//   一个对象有多个"所有者"，你不知道谁会最后用完它。
//
// 典型场景：
//   - 多个容器共享同一个对象
//   - GUI 程序中多个窗口共享同一个数据
//   - 缓存场景：多个请求共享同一个配置

struct Config
{
    string school_;
    string semester_;
    Config(string s, string sem) : school_(s), semester_(sem)
    {
        cout << "  [Config] 加载配置: " << school_ << " " << semester_ << endl;
    }
    ~Config() { cout << "  [Config] 配置被销毁\n"; }
};

void useConfig(shared_ptr<Config> cfg)
{
    // 传值进来，引用计数 +1
    cout << "  useConfig() 内 use_count = " << cfg.use_count() << endl;
    cout << "  读取配置: " << cfg->school_ << " " << cfg->semester_ << endl;
}   // cfg 析构，引用计数 -1

void lesson2_应用场景()
{
    cout << "\n========== Lesson 2: shared_ptr 应用场景 ==========\n\n";

    // ----- 场景：多个地方需要同一份配置 -----
    cout << "--- 共享配置对象 ---\n";
    {
        // 一个配置对象，多处引用
        auto config = make_shared<Config>("清华大学", "2026春");

        cout << "创建后 use_count = " << config.use_count() << endl;

        // 传给函数（拷贝，计数+1）
        useConfig(config);
        cout << "函数返回后 use_count = " << config.use_count() << endl;

        // 存到另一个变量
        auto backup = config;
        cout << "backup 引用后 use_count = " << config.use_count() << endl;

        // config 和 backup 都离开作用域，计数归零，配置自动销毁
    }

    // ----- 场景：容器中共享对象 -----
    cout << "\n--- vector 存 shared_ptr ---\n";
    {
        vector<shared_ptr<Fruit>> basket;
        auto apple = make_shared<Fruit>("苹果");

        basket.push_back(apple);     // 容器共享，计数 +1
        basket.push_back(apple);     // 再存一次，计数 +1
        cout << "  苹果被 " << apple.use_count() << " 个引用持有\n";

        // apple 和 basket 都析构后，对象才被释放
    }

    cout << "\n【shared_ptr 适用场景】\n";
    cout << "  一个对象被多处引用 → shared_ptr\n";
    cout << "  只有一个明确的拥有者 → unique_ptr（更轻量）\n";
}

// ============================================================
// Lesson 3: weak_ptr —— 打破循环引用
// ============================================================
//
// shared_ptr 有一个致命问题：循环引用。
// 两个对象互相持有对方的 shared_ptr → 引用计数永远不会归零 → 内存泄漏！
//
// weak_ptr 就是来解决这个问题的：
//   - 可以"观察"一个 shared_ptr 管理的对象
//   - 但不增加引用计数（不参与所有权）
//   - 有点像 shared_ptr 的"旁观者"

// 例子：双向链表节点（经典的循环引用场景）
// 父节点用 shared_ptr 指向子节点（拥有它）
// 子节点用 weak_ptr 指向父节点（只是看看，不拥有）

struct Node
{
    string name_;
    shared_ptr<Node> next_;     // 下一个节点（拥有它）
    weak_ptr<Node>   prev_;     // 上一个节点（只是旁观，不拥有）
    //  ↑ 如果这里用 shared_ptr，Node 之间就会循环引用，永远不释放！

    Node(string n) : name_(n)
    {
        cout << "  [Node] 创建: " << name_ << endl;
    }
    ~Node()
    {
        cout << "  [Node] 析构: " << name_ << endl;
    }
};

void lesson3_weak_ptr()
{
    cout << "\n========== Lesson 3: weak_ptr 打破循环引用 ==========\n\n";

    // ----- 演示循环引用问题 -----
    cout << "--- 如果用 shared_ptr 互相引用（错误示范）---\n";
    {
        // 构造两个 Node，用 shared_ptr 互相持有 —— 这会泄漏！
        // （这里只演示概念，不实际执行，因为泄漏不好看）
        cout << "  想象：A.next 指向 B（shared_ptr），B.prev 指向 A（shared_ptr）\n";
        cout << "  离开作用域时，A 说：B 还指向我，我不能析构\n";
        cout << "                B 说：A 还指向我，我不能析构\n";
        cout << "  结果：谁都不析构 → 内存泄漏！\n";
        cout << "  解法：把 B.prev 改成 weak_ptr，不参与计数\n";
    }

    // ----- 正确做法：weak_ptr 打破循环 -----
    cout << "\n--- 正确做法：prev 用 weak_ptr ---\n";
    {
        auto node1 = make_shared<Node>("节点1");
        auto node2 = make_shared<Node>("节点2");

        node1->next_ = node2;     // shared_ptr → 计数 +1
        node2->prev_ = node1;     // weak_ptr   → 计数不变！

        cout << "  node1 use_count = " << node1.use_count() << "（node2.prev 是 weak，不加计数）\n";
        cout << "  node2 use_count = " << node2.use_count() << "（node1.next 是 shared，计数=2）\n";

        // node1 和 node2 析构时：
        // node2 先析构（node1->next_ 释放，node2 计数-1，归零 → delete node2）
        // node1 再析构（计数归零 → delete node1）
    }
    cout << "  两个 Node 都正确析构了！\n";

    // ----- weak_ptr 的 lock() 用法 -----
    cout << "\n--- lock()：把 weak_ptr 临时升级为 shared_ptr ---\n";
    {
        weak_ptr<Node> weak;      // 空的旁观者

        {
            auto node = make_shared<Node>("临时节点");
            weak = node;           // weak_ptr 观察 node
            // 注意：weak = node 不会让 use_count 增加

            cout << "  node.use_count() = " << node.use_count() << endl;

            // lock() 尝试获取一个临时的 shared_ptr
            // 如果原对象还活着 → 返回一个 shared_ptr（计数+1）
            // 如果原对象已经被 delete → 返回 nullptr
            if (auto sp = weak.lock())           // lock 升级为 shared_ptr
            {
                cout << "  lock 成功！name = " << sp->name_ << endl;
                cout << "  use_count = " << node.use_count() << "（lock 临时 +1）\n";
            }
        }   // node 离开作用域，计数归零，对象被 delete

        // 此时 weak 指向的对象已经不存在了
        cout << "  原对象离开作用域后，weak.lock() 返回: " << weak.lock().get() << "（nullptr）\n";
    }

    cout << "\n【weak_ptr 要点】\n";
    cout << "  不增加引用计数 → 打破循环引用的关键\n";
    cout << "  lock() → 临时升级为 shared_ptr，对象已死则返回 nullptr\n";
    cout << "  场景：双向链表、树结构中的父指针、观察者模式\n";
}

// ============================================================
// Lesson 4: unique_ptr vs shared_ptr 对比总结
// ============================================================
//
//  ┌──────────────────┬───────────────────┬───────────────────┐
//  │                  │   unique_ptr      │   shared_ptr      │
//  ├──────────────────┼───────────────────┼───────────────────┤
//  │ 所有权           │ 独占              │ 共享              │
//  │ 能否拷贝         │ 不能（= delete）  │ 可以               │
//  │ 性能开销         │ 零开销（和裸指针一样）│ 引用计数有一定开销 │
//  │ 占用内存         │ 一个指针大小      │ 两个指针大小        │
//  │ 创建方式         │ make_unique       │ make_shared        │
//  │ 适用场景         │ 明确的单一所有者   │ 多个人需要同一个对象 │
//  │ 配对的弱引用     │ 无                │ weak_ptr           │
//  └──────────────────┴───────────────────┴───────────────────┘
//
// 选择原则：
//   默认用 unique_ptr → 大多数情况只有一个明确的所有者
//   确定一个对象被多处共享时 → 改用 shared_ptr
//   需要打破循环引用时 → weak_ptr
//   永远不要用裸指针 new/delete！

void lesson4_对比()
{
    cout << "\n========== Lesson 4: unique_ptr vs shared_ptr 对比 ==========\n\n";

    // ----- unique_ptr：独占，轻量 -----
    cout << "--- unique_ptr：独占场景 ---\n";
    {
        unique_ptr<Fruit> u = make_unique<Fruit>("西瓜");
        // unique_ptr<Fruit> u2 = u;    // ❌ 独占，不能拷贝

        // 转移到容器里
        vector<unique_ptr<Fruit>> cart;
        cart.push_back(move(u));         // 所有权转移
        cout << "  西瓜被移动到购物车，u.get() = " << u.get() << endl;

        cout << "  unique_ptr 适合：容器存多态对象，明确容器是唯一拥有者\n";
    }

    // ----- shared_ptr：共享，灵活 -----
    cout << "\n--- shared_ptr：共享场景 ---\n";
    {
        auto apple = make_shared<Fruit>("苹果");
        vector<shared_ptr<Fruit>> basket1;
        vector<shared_ptr<Fruit>> basket2;

        basket1.push_back(apple);        // 两个篮子共享同一个苹果
        basket2.push_back(apple);

        cout << "  苹果被 " << apple.use_count() << " 个引用共享\n";
        cout << "  shared_ptr 适合：多个地方需要同一份数据\n";
    }

    cout << "\n【选择原则】\n";
    cout << "  1. 默认用 unique_ptr（更轻量、更安全）\n";
    cout << "  2. 必须共享时用 shared_ptr\n";
    cout << "  3. 打破循环用 weak_ptr\n";
    cout << "  4. 永远不要写 new 和 delete\n";
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_shared_ptr基础();
    lesson2_应用场景();
    lesson3_weak_ptr();
    lesson4_对比();

    return 0;
}
