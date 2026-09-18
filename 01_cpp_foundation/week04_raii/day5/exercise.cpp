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
// Day 5 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：shared_ptr 基本使用和引用计数
// -----------------------------------------------------------
// 写一个简单的类 Book，有两个字段：
//   - string title_
//   - int pages_
// 要求：
//   - 构造函数打印 "[Book] 创建: title"
//   - 析构函数打印 "[Book] 销毁: title"
//   - void info() const 打印 "《title》共pages页"
//
// 在 test_shared_ptr_basic() 中完成：
//   - ① 用 make_shared<Book> 创建一个 Book，打印 use_count()
//   - ② 创建另一个 shared_ptr 指向同一个 Book（拷贝），打印 use_count()
//   - ③ 在一个内层 {} 作用域中再拷贝一份，打印 use_count()
//       离开内层作用域后再打印 use_count()
//   - ④ 测试 reset()：把第一个 shared_ptr reset()，打印另一个的 use_count()
//   - ⑤ 尝试写 shared_ptr<Book> p2 = p1; 看能不能编译通过
//       再写 unique_ptr<Book> u1、unique_ptr<Book> u2 = u1;
//       对比两者的报错

// TODO: 写 Book 类
class Book
{
private:
    string title_;
    int pages_;
public:
    Book(string title,int pages):title_(title),pages_(pages)
    {
        cout << "[Book] 创建:" << title_ << endl;
    }
    ~Book()
    {
        cout << "[Book] 销毁:" << title_ << endl;
    }
    void info()const
    {
        cout << title_ << "共" << pages_ << "页\n";
    }
};
void test_shared_ptr_basic()
{
    // TODO: 测试 shared_ptr 引用计数
    shared_ptr<Book> a = make_shared<Book>("百年孤独",80);
    a->info();
    cout << a.use_count() << endl;
    shared_ptr<Book> b = a;
    cout << b.use_count() << endl;
    {
        shared_ptr<Book> c = b;
        cout << c.use_count() << endl;
    }
    cout << a.use_count() << endl;
    a.reset();
    b.use_count();
    //shared_ptr可以拷贝 支持p2 = p1;
    //unique_ptr不可以拷贝 不支持p2 = p1;
}

// -----------------------------------------------------------
// 需求 2：循环引用与 weak_ptr
// -----------------------------------------------------------
// 写一个双向链表节点类 PersonNode：
//   - string name_
//   - shared_ptr<PersonNode> next_   （下一个节点）
//   - weak_ptr<PersonNode> prev_     （上一个节点，用 weak_ptr！）
//   - 构造函数打印 "[PersonNode] 创建: name"
//   - 析构函数打印 "[PersonNode] 析构: name"
//
// 写另一个类 BadNode（错误示范）：
//   - 和 PersonNode 一样，但 prev_ 用 shared_ptr<BadNode>（不用 weak_ptr）
//   - 构造函数打印 "[BadNode] 创建: name"
//   - 析构函数打印 "[BadNode] 析构: name"
//
// 在 test_circular_reference() 中完成：
//   - ① 创建两个 BadNode（用 shared_ptr），互相指向对方
//       node1->next_ = node2;  node2->prev_ = node1;
//       离开作用域，观察析构函数有没有被调用（应该没有！循环引用泄漏了）
//       打印一条提示："注意：上面没有出现 [BadNode] 析构，内存泄漏了！"
//
//   - ② 创建两个 PersonNode（用 shared_ptr），互相指向对方
//       node1->next_ = node2;  node2->prev_ = node1;（prev 是 weak_ptr）
//       离开作用域，观察析构函数是否被调用
//
//   - ③ 测试 weak_ptr 的 lock() 用法：
//       创建一个 shared_ptr<PersonNode>
//       让一个 weak_ptr 观察它
//       调用 lock() 获取临时的 shared_ptr，打印 name_
//       把原 shared_ptr reset() 掉
//       再次 lock()，打印结果（应该是 nullptr）

// TODO: 写 PersonNode 类（正确做法：weak_ptr）
struct PersonNode
{

    string name_;
    shared_ptr<PersonNode> next_;
    weak_ptr<PersonNode> pre_;

    PersonNode(string name):name_(name)
    {
       cout << "[PersonNode] 创建: " << name_ << endl;
    }
    ~PersonNode()
    {
        cout << "[PersonNode] 析构: " << name_ << endl;
    }
};
// TODO: 写 BadNode 类（错误示范：shared_ptr 循环引用）

struct BadNode
{

    string name_;
    shared_ptr<BadNode> next_;
    shared_ptr<BadNode> pre_;

    BadNode(string name):name_(name)
    {
       cout << "[BadNode] 创建: " << name_ << endl;
    }
    ~BadNode()
    {
        cout << "[BadNode] 析构: " << name_ << endl;
    }
};

void test_circular_reference()
{
    // TODO: 测试循环引用和 weak_ptr
    {
        shared_ptr<BadNode> a1 = make_shared<BadNode>("a1");
        shared_ptr<BadNode> a2 = make_shared<BadNode>("a2");
        a1->next_ = a2;
        a2->pre_ = a1;
    }
    cout << "注意:上面没有析构 内存泄漏了！\n";

    {
        shared_ptr<PersonNode> a1 = make_shared<PersonNode>("a1");
        shared_ptr<PersonNode> a2 = make_shared<PersonNode>("a2");
        a1->next_ = a2;
        a2->pre_ = a1;
    }
    cout << "析构了 没有内存泄漏！\n";
    shared_ptr<PersonNode> a1 = make_shared<PersonNode>("a1");
    weak_ptr<PersonNode> b;
    b = a1;
    cout << b.lock()->name_ << endl;
    a1.reset();
    cout << b.lock().get() << endl;
}

// -----------------------------------------------------------
// 需求 3（综合）：选择 unique_ptr 还是 shared_ptr
// -----------------------------------------------------------
// 读下面的场景，写代码实现，并说明为什么选这个指针类型：
//
// 场景 A：动物园
//   - 每只动物只能属于一个动物园
//   - 动物可能被转移（移动到另一个动物园）
//   → 用什么指针？理由？
//unique_ptr 动物这个资源在某一刻只可能属于一个动物园
// 场景 B：共享文档
//   - 一份文档，多个编辑器窗口同时打开
//   - 每个窗口都能编辑这份文档
//   - 最后一个窗口关闭时，文档自动保存并销毁
//   → 用什么指针？理由？
//shared_ptr  一个文档被多个窗口持有
// 场景 C：树结构
//   - 父节点拥有子节点（shared_ptr<Child>）
//   - 子节点需要能访问父节点
//   → 子节点指向父节点用什么指针？理由？
//shared_ptr weak_ptr 父节点要拥有子节点 同时 子节点不能拥有父节点 只能访问
// 在 test_choose_pointer() 中实现这三个场景的简化版本。

// 场景 A 的类：动物（用 unique_ptr，因为一只动物只属于一个动物园）
class Animal
{
private:
    string name_;
public:
    Animal(string name):name_(name)
    {
        cout << "[Animal] 创建: " << name_ << endl;
    }
    ~Animal()
    {
        cout << "[Animal] 转移/销毁: " << name_ << endl;
    }
    void info()const { cout << "动物: " << name_ << endl; }
};

// 场景 B 的类：文档（用 shared_ptr，因为多个窗口共享同一份文档）
class Document
{
private:
    string content_;
public:
    Document(string content):content_(content)
    {
        cout << "[Document] 创建, use_count=1\n";
    }
    ~Document()
    {
        cout << "[Document] 销毁，自动保存: " << content_ << endl;
    }
    void show()const { cout << "文档内容: " << content_ << endl; }
    void edit(string newContent) { content_ = newContent; }
};

// 场景 C 的类：树节点
//   父→子用 shared_ptr（拥有），子→父用 weak_ptr（只访问，不拥有）
class TreeNode
{
public:
    string name_;
    vector<shared_ptr<TreeNode>> children_;
    weak_ptr<TreeNode> parent_;
    TreeNode(string name):name_(name)
    {
        cout << "[TreeNode] 创建: " << name_ << endl;
    }
    ~TreeNode()
    {
        cout << "[TreeNode] 析构: " << name_ << endl;
    }
};

void test_choose_pointer()
{
    cout << "\n========== 场景 A：动物园（unique_ptr）==========\n";
    {
        vector<unique_ptr<Animal>> zoo1;
        zoo1.push_back(make_unique<Animal>("老虎"));
        zoo1.push_back(make_unique<Animal>("狮子"));
        cout << "动物园1有" << zoo1.size() << "只动物\n";

        vector<unique_ptr<Animal>> zoo2;
        zoo2.push_back(move(zoo1[0]));
        zoo1.erase(zoo1.begin());
        cout << "转移后，动物园1有" << zoo1.size() << "只，动物园2有" << zoo2.size() << "只\n";
    }
    cout << "离开作用域，所有动物自动销毁\n";

    cout << "\n========== 场景 B：共享文档（shared_ptr）==========\n";
    {
        auto doc = make_shared<Document>("Hello World");
        cout << "当前引用计数: " << doc.use_count() << endl;

        vector<shared_ptr<Document>> windows;
        windows.push_back(doc);
        windows.push_back(doc);
        cout << "打开" << windows.size() << "个窗口，引用计数: " << doc.use_count() << endl;

        windows[1]->edit("Hello C++");
        cout << "窗口2修改后，窗口1看到的: ";
        windows[0]->show();

        windows.clear();
        cout << "窗口全部关闭，引用计数: " << doc.use_count() << endl;
    }
    cout << "最后一个 shared_ptr 销毁，文档自动保存并销毁\n";

    cout << "\n========== 场景 C：树结构（weak_ptr 防循环引用）==========\n";
    {
        auto root = make_shared<TreeNode>("根");
        auto child1 = make_shared<TreeNode>("子1");
        auto child2 = make_shared<TreeNode>("子2");

        root->children_.push_back(child1);
        root->children_.push_back(child2);

        child1->parent_ = root;
        child2->parent_ = root;

        if(auto p = child1->parent_.lock())
            cout << child1->name_ << " 的父节点是: " << p->name_ << endl;
    }
    cout << "离开作用域，父子节点全部正常析构（weak_ptr 打破了循环引用）\n";
}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    test_shared_ptr_basic();
    test_circular_reference();
    test_choose_pointer();

    return 0;
}
