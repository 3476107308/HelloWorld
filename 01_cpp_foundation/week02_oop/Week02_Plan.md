# 第 2 周学习计划：C++ 继承、多态、虚函数

时间：5.20-5.26  
对应总计划：第 2 周，C++ 面向对象强化  
本周目标：能独立写出“基类 + 派生类 + 多态调用”的小项目，并能解释虚函数、纯虚函数、抽象类、虚析构函数。

## 一、本周必须掌握

### 1. 继承

重点：
- 基类、派生类。
- `public`、`protected`、`private` 成员访问权限。
- `public` 继承、`protected` 继承、`private` 继承。
- 构造函数和析构函数的调用顺序。

你要能回答：
- 子类能不能访问父类的 `private` 成员？
- `protected` 和 `private` 的区别是什么？
- 为什么实际开发中最常用的是 `public` 继承？

### 2. 多态

重点：
- 静态多态：函数重载、运算符重载。
- 动态多态：父类指针/引用指向子类对象，通过虚函数调用子类实现。
- 向上转型：`Animal* p = new Cat;`

你要能回答：
- 什么是多态？
- 为什么父类指针可以指向子类对象？
- 没有 `virtual` 会发生什么？

### 3. 虚函数、纯虚函数、抽象类

重点：
- `virtual` 虚函数。
- `= 0` 纯虚函数。
- 包含纯虚函数的类是抽象类，不能直接实例化。
- `override` 的作用。

你要能回答：
- 虚函数和普通成员函数有什么区别？
- 纯虚函数存在的意义是什么？
- 为什么建议子类重写虚函数时加 `override`？

### 4. 虚析构函数

重点：
- 通过父类指针释放子类对象时，父类析构函数必须是 `virtual`。
- 否则可能只调用父类析构，不调用子类析构，造成资源泄漏。

你要能回答：
- 为什么基类析构函数通常要写成虚析构？
- 什么情况下析构函数可以不写成虚函数？

## 二、推荐学习资料

### 主线视频

黑马程序员 C++：
- 继承。
- 多态。
- 虚函数。
- 纯虚函数和抽象类。
- 虚析构和纯虚析构。

对应视频：
- 黑马程序员《C++教程从0到1入门编程》完整版：https://www.bilibili.com/video/BV1et411b73Z/
- 重点看选集：45-58（继承基本语法、继承方式、构造析构顺序、多态、纯虚函数和抽象类、虚析构和纯虚析构）。

建议：
- 这周继续用黑马作为主线，不要换太多课。
- 每个知识点看完后立刻写代码验证。

### 补充视频

侯捷《C++ 面向对象高级编程》：
- 继承与复合。
- 虚函数与多态。

对应视频：
- 侯捷《C++ 面向对象高级开发（上）》：https://www.bilibili.com/video/BV1r6h5zgE2i/
- 重点看选集：11《组合与继承》、12《虚函数与多态》。

建议：
- 侯捷课可以作为“理解原理”的补充。
- 如果听起来吃力，先不用硬啃，优先保证代码能写出来。

### 文档查阅

cppreference：
- `virtual function`
- `override specifier`
- `abstract class`
- `destructor`

建议：
- 不需要全文读完。
- 遇到语法不确定时查一下定义和示例。

## 三、每日安排

### Day 1：继承基础和访问权限

学习内容：
- 类与类之间的继承关系。
- `public/private/protected` 成员访问。
- 三种继承方式。

对应学习视频：
- 黑马程序员 C++：继承基本语法：https://www.bilibili.com/video/BV1et411b73Z/?p=45
- 黑马程序员 C++：继承方式：https://www.bilibili.com/video/BV1et411b73Z/?p=46

代码练习：
- 写一个 `Person` 基类。
- 派生 `Student` 和 `Teacher`。
- 分别测试 `public`、`protected`、`private` 成员在派生类中的访问情况。 

建议文件：
```text
week02_oop/
  day01_inheritance.cpp
```

今日验收：
- 能说清楚“成员访问权限”和“继承方式”是两件事。
- 能解释为什么子类不能直接访问父类 `private` 成员。

### Day 2：构造、析构和继承顺序

学习内容：
- 父类构造函数先执行，子类构造函数后执行。
- 子类析构函数先执行，父类析构函数后执行。
- 子类构造函数如何调用父类有参构造。

对应学习视频：
- 黑马程序员 C++：继承中构造和析构顺序：https://www.bilibili.com/video/BV1et411b73Z/?p=48
- 黑马程序员 C++：继承中的对象模型：https://www.bilibili.com/video/BV1et411b73Z/?p=47

代码练习：
- 写 `Base` 和 `Derived`。
- 在构造、析构函数中打印日志。
- 观察执行顺序。

建议文件：
```text
week02_oop/
  day02_constructor_destructor.cpp
```

今日验收：
- 能画出对象创建和销毁时的调用顺序。
- 能写出子类初始化父类成员的代码。

### Day 3：虚函数和动态多态

学习内容：
- 普通函数调用和虚函数调用的区别。
- 父类指针/引用调用子类重写函数。
- `override`。

对应学习视频：
- 黑马程序员 C++：多态基本概念：https://www.bilibili.com/video/BV1et411b73Z/?p=53
- 黑马程序员 C++：多态案例-计算器类：https://www.bilibili.com/video/BV1et411b73Z/?p=54
- 侯捷 C++：虚函数与多态：https://www.bilibili.com/video/BV1r6h5zgE2i/?p=12

代码练习：
- 写 `Animal`、`Cat`、`Dog`。
- 先不加 `virtual`，观察输出。
- 再加 `virtual`，观察输出变化。

建议文件：
```text
week02_oop/
  day03_virtual_function.cpp
```

今日验收：
- 能解释“静态绑定”和“动态绑定”。
- 能说明 `virtual` 改变的是运行时调用行为。

### Day 4：纯虚函数和抽象类

学习内容：
- 纯虚函数：`virtual void speak() = 0;`
- 抽象类不能实例化。
- 接口式设计。

对应学习视频：
- 黑马程序员 C++：纯虚函数和抽象类：https://www.bilibili.com/video/BV1et411b73Z/?p=55
- 黑马程序员 C++：多态案例-制作饮品：https://www.bilibili.com/video/BV1et411b73Z/?p=56

代码练习：
- 把 `Animal` 改成抽象类。
- 派生 `Cat`、`Dog`、`Bird`。
- 所有派生类必须实现 `speak()`。

建议文件：
```text
week02_oop/
  day04_abstract_class.cpp
```

今日验收：
- 能解释为什么 `Animal animal;` 不能编译。
- 能说出抽象类在项目中的作用：统一接口，隐藏具体实现。

### Day 5：虚析构函数和资源释放

学习内容：
- 父类指针释放子类对象。
- 虚析构函数。
- 内存泄漏风险。

对应学习视频：
- 黑马程序员 C++：虚析构和纯虚析构：https://www.bilibili.com/video/BV1et411b73Z/?p=57
- 黑马程序员 C++：多态案例-电脑组装：https://www.bilibili.com/video/BV1et411b73Z/?p=58

代码练习：
- 子类中申请动态数组。
- 用 `Animal* p = new Cat; delete p;` 测试析构行为。
- 分别测试基类析构函数加 `virtual` 和不加 `virtual`。

建议文件：
```text
week02_oop/
  day05_virtual_destructor.cpp
```

今日验收：
- 能解释为什么 `Animal` 的析构函数应该写成：
```cpp
virtual ~Animal() = default;
```

### Day 6：本周验收项目

项目名：
```text
week02_animal_polymorphism
```

项目要求：
- 定义抽象基类 `Animal`。
- 派生类至少包含 `Cat`、`Dog`、`Bird`。
- 每个动物都有：
  - 名字。
  - 年龄。
  - `speak()`。
  - `move()`。
- 使用 `vector<Animal*>` 或 `vector<unique_ptr<Animal>>` 保存动物对象。
- 遍历容器，多态调用 `speak()` 和 `move()`。
- 正确释放资源。

推荐做法：
- 如果你第一周对指针还不够熟，先用 `vector<Animal*>`，手动 `delete`。
- 如果你想提前挑战更现代的写法，用 `vector<unique_ptr<Animal>>`。

最低文件结构：
```text
week02_animal_polymorphism/
  main.cpp
  Animal.h
  Cat.h
  Cat.cpp
  Dog.h
  Dog.cpp
  Bird.h
  Bird.cpp
  README.md
```

进阶要求：
- 加一个 `Zoo` 类，负责管理动物。
- 支持 `addAnimal()`。
- 支持 `showAllAnimals()`。
- 支持按名字查找动物。

验收标准：
- 程序能正常编译运行。
- 输出每个动物的叫声和移动方式。
- 基类析构函数是 `virtual`。
- 子类重写函数使用 `override`。
- 没有明显内存泄漏。

### Day 7：复盘和面试题

整理文件：
```text
week02_note.md
```

必须写清楚：
- 继承是什么？
- `public/protected/private` 成员访问区别。
- 三种继承方式的区别。
- 多态是什么？
- 虚函数是什么？
- 纯虚函数是什么？
- 抽象类是什么？
- 为什么要有虚析构函数？
- `override` 有什么用？
- 构造和析构的调用顺序是什么？

本周提交：
- 至少 3 次 Git commit。
- 至少 1 个完整 README。
- 至少 1 张程序运行截图。

## 四、VS Code 操作重点

### 1. 推荐目录结构

```text
cpp-learning/
  week02_oop/
    day01_inheritance.cpp
    day02_constructor_destructor.cpp
    day03_virtual_function.cpp
    day04_abstract_class.cpp
    day05_virtual_destructor.cpp
  week02_animal_polymorphism/
    main.cpp
    Animal.h
    Cat.h
    Cat.cpp
    Dog.h
    Dog.cpp
    Bird.h
    Bird.cpp
    README.md
```

### 2. 单文件编译

MSVC：
```powershell
cl /EHsc day03_virtual_function.cpp
.\day03_virtual_function.exe
```

MinGW：
```powershell
g++ -std=c++17 -Wall -Wextra -g day03_virtual_function.cpp -o day03_virtual_function.exe
.\day03_virtual_function.exe
```

### 3. 多文件编译

MSVC：
```powershell
cl /EHsc main.cpp Cat.cpp Dog.cpp Bird.cpp
.\main.exe
```

MinGW：
```powershell
g++ -std=c++17 -Wall -Wextra -g main.cpp Cat.cpp Dog.cpp Bird.cpp -o animal.exe
.\animal.exe
```

### 4. 调试时重点观察

打断点位置：
- `Animal` 构造函数。
- `Cat/Dog/Bird` 构造函数。
- `speak()`。
- 各个析构函数。

观察内容：
- 创建对象时先调用谁。
- 删除对象时先调用谁。
- 父类指针调用的是父类函数还是子类函数。

## 五、本周最终代码模板

可以按这个骨架写：

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Animal {
public:
    Animal(string name, int age) : name_(name), age_(age) {}

    virtual ~Animal() = default;

    virtual void speak() const = 0;
    virtual void move() const = 0;

    string name() const {
        return name_;
    }

    int age() const {
        return age_;
    }

protected:
    string name_;
    int age_;
};

class Cat : public Animal {
public:
    Cat(string name, int age) : Animal(name, age) {}

    void speak() const override {
        cout << name_ << " says: meow" << endl;
    }

    void move() const override {
        cout << name_ << " walks quietly" << endl;
    }
};

class Dog : public Animal {
public:
    Dog(string name, int age) : Animal(name, age) {}

    void speak() const override {
        cout << name_ << " says: woof" << endl;
    }

    void move() const override {
        cout << name_ << " runs happily" << endl;
    }
};

class Bird : public Animal {
public:
    Bird(string name, int age) : Animal(name, age) {}

    void speak() const override {
        cout << name_ << " says: chirp" << endl;
    }

    void move() const override {
        cout << name_ << " flies in the sky" << endl;
    }
};

int main() {
    vector<unique_ptr<Animal>> animals;

    animals.push_back(make_unique<Cat>("Mimi", 2));
    animals.push_back(make_unique<Dog>("Wangcai", 3));
    animals.push_back(make_unique<Bird>("Xiaolan", 1));

    for (const auto& animal : animals) {
        animal->speak();
        animal->move();
    }

    return 0;
}
```

## 六、本周不要踩的坑

- 不要忘记给基类析构函数加 `virtual`。
- 不要把 `override` 省掉，它能帮你发现函数签名写错。
- 不要在抽象基类里写太多具体逻辑，本周重点是统一接口。
- 不要只看视频不写代码，多态必须通过调试才能真正理解。
- 不要一上来纠结虚函数表底层细节，先保证会用、会解释、会写项目。
