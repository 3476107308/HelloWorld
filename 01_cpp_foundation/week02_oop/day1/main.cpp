#include <iostream>
#include <string>
using namespace std;

// ============================================================
// Week02 Day 1：继承基础 & 访问权限 & 三种继承方式
// ============================================================
//
// 今日目标：
//   1. 理解"继承"是什么、为什么用继承
//   2. 搞懂 public / protected / private 三种成员在继承中的行为
//   3. 区分"成员访问权限"和"继承方式"——这是两件不同的事
//   4. 能说出三种继承方式对子类的影响
//
// 推荐先看视频：
//   - 黑马 C++ P45：继承基本语法
//   - 黑马 C++ P46：继承方式
// ============================================================


// ============================================================
// 第 1 课：什么是继承？
// ============================================================
// 继承就像是"is-a"关系：
//   - Student 是一个 Person（学生是人）
//   - Teacher 是一个 Person（老师是人）
//
// Person 有的东西（name, age），Student 和 Teacher 也都有。
// 把共同的东西抽到父类 Person 里，子类就不用重复写了。
//
// 语法：
//   class 子类 : public 父类 { ... };
//   这里的 public 是"继承方式"，不是成员访问权限，后面会详细讲。
// ============================================================

class Person {
//  ======== 三个访问权限的区域 ========
//  下面用 public / protected / private 把成员分成三类，
//  目的：观察子类分别能访问哪些、不能访问哪些。

public:    // —— 公开区域：类外 + 子类 + 自己 都能访问
    string name = "张三";

    void say_hello() {
        cout << "Hello, I'm " << name << endl;
    }

protected: // —— 保护区域：只有子类 + 自己能访问，类外不能访问
    int age = 18;

    void protected_func() {
        cout << "这是基类的 protected 函数" << endl;
    }

private:   // —— 私有区域：只有自己能访问，子类和类外都不能访问
    string id_card = "310000200001011234";  // 身份证号，谁都不要看

    void private_func() {
        cout << "这是基类的 private 函数" << endl;
    }
};


// ============================================================
// 第 2 课：子类能访问基类的什么成员？
// ============================================================
// 规则很简单：
//   - public    成员 → 子类内部可以访问 ✓
//   - protected 成员 → 子类内部可以访问 ✓
//   - private   成员 → 子类内部 不能 访问 ✗
//
// 不管继承方式是 public / protected / private，
// 基类的 private 成员子类永远无法直接访问。
// ============================================================

class Student : public Person {
public:
    void test_access() {
        // ---- 尝试访问基类的三类成员 ----

        // ① public 成员：可以访问 ✓
        cout << "name = " << name << endl;
        say_hello();

        // ② protected 成员：可以访问 ✓
        //    子类内部能直接使用，就像自己的成员一样
        cout << "age = " << age << endl;
        protected_func();

        // ③ private 成员：不能访问 ✗
        //    去掉下面两行的注释，编译器会报错：
        //    error: 'id_card' is a private member of 'Person'
        //    error: 'private_func' is a private member of 'Person'
        //
        // cout << "id_card = " << id_card << endl;    // 编译错误！
        // private_func();                               // 编译错误！

        cout << "（private 成员访问不了，编译会报错）" << endl;
    }
};


// ============================================================
// 第 3 课：三种继承方式 —— 真正影响的是什么？
// ============================================================
// 继承方式决定了基类成员在子类中"降级"为什么权限。
//
//                   基类 public   基类 protected   基类 private
//  public 继承   →  子类 public   子类 protected   子类不可访问
//  protected 继承 → 子类 protected 子类 protected   子类不可访问
//  private 继承  →  子类 private   子类 private     子类不可访问
//
// 一句话：继承方式"收紧"了基类成员在子类中的可见度。
//   - public 继承：原来是什么就是什么（最常用！）
//   - protected 继承：public 降为 protected
//   - private 继承：全部降为 private
//   - 基类的 private 成员不管什么继承方式，子类都访问不到
// ============================================================


// ---- 3.1 public 继承（最常用，约 95% 的情况）----
// 基类的 public 还是 public，protected 还是 protected
// 这是最自然的继承，"is-a" 关系就用 public 继承
class StudentPublic : public Person {
    // name、say_hello  → public     （和 Person 一样）
    // age               → protected  （和 Person 一样）
    // id_card           → 不可访问   （Person 的 private）
};


// ---- 3.2 protected 继承（很少用）----
// 基类的 public 降级为 protected，类外就访问不到了
class StudentProtected : protected Person {
public:
    void show() {
        // name 和 say_hello 在这里变成了 protected
        // 所以子类内部还能用，但子类的类外就不能用了
        cout << "name(现在是protected): " << name << endl;
    }
};
// StudentProtected stu;
// stu.name;         // 编译错误！name 已经是 protected，类外不能访问
// stu.say_hello();  // 编译错误！say_hello 也变成了 protected


// ---- 3.3 private 继承（极少用）----
// 基类的 public 和 protected 全降为 private
// 意味着：这个子类的子类（孙子类）也什么都访问不到了
class StudentPrivate : private Person {
public:
    void show() {
        // name、say_hello、age 在这里都变成了 private
        // 所以这个子类内部还能用
        cout << "name(现在是private): " << name << endl;
    }
};
// 如果 StudentPrivate 再有子类，那孙子类什么都访问不到
// 因为到了 StudentPrivate 这里，Person 的成员全变成了 private


// ============================================================
// 第 4 课：实际应用——Person → Student / Teacher
// ============================================================
// 用 public 继承写两个真正的子类，这才是日常开发的方式。

class Teacher : public Person {
private:
    string subject_;   // 老师多一个"科目"属性

public:
    Teacher(string name, int age, string subject) {
        // 注意：name 继承自 Person 的 public，可以直接赋值
        this->name = name;       // 等价于 Person::name = name
        this->age = age;         // age 是 protected，子类内部可以用
        subject_ = subject;
    }

    void teach() {
        // 能访问 name（public）和 age（protected）
        cout << name << " 老师（" << age << "岁）在教 " << subject_ << endl;
    }
};

class GoodStudent : public Person {
private:
    string grade_;     // 学生多一个"年级"属性
    int score_;

public:
    GoodStudent(string name, int age, string grade, int score) {
        this->name = name;
        this->age = age;
        grade_ = grade;
        score_ = score;
    }

    void study() {
        cout << name << "（" << grade_ << "）在学习... 成绩：" << score_ << endl;
    }
};


// ============================================================
// 第 5 课：总结——"成员权限" vs "继承方式"是两件不同的事
// ============================================================
// 很多初学者会搞混，记住这个比喻：
//
//   成员权限（public/protected/private）：
//     → 就像一个人的"隐私等级"
//     → 身份证号是 private，年龄是 protected，名字是 public
//     → 这是你（基类）自己决定的
//
//   继承方式（public/protected/private 继承）：
//     → 就像"孩子继承了你的东西以后，怎么对外展示"
//     → public 继承：孩子原样对外展示（身份证还是保密，名字还是公开）
//     → protected 继承：孩子把你的东西都收起来一点
//     → private 继承：孩子把你的东西全藏起来，谁也不给看
//
// 最常见的写法就是：
//   class Child : public Parent { ... };
// ============================================================


// ============================================================
// 测试代码
// ============================================================
int main() {
    cout << "========== Week02 Day 1：继承基础与访问权限 ==========" << endl;
    cout << endl;

    // ---- 测试1：子类访问基类成员 ----
    cout << "--- 1. Student 测试访问基类成员 ---" << endl;
    Student s;
    s.test_access();         // 观察子类能访问哪些
    cout << endl;

    // ---- 测试2：类外访问 ----
    cout << "--- 2. 类外访问 public 成员 ---" << endl;
    cout << "s.name = " << s.name << endl;  // public，类外可以
    s.say_hello();                           // public，类外可以
    // s.age;        // 编译错误！protected，类外不能访问
    // s.id_card;    // 编译错误！private，类外不能访问
    cout << "（age 和 id_card 在类外访问不了）" << endl;
    cout << endl;

    // ---- 测试3：public 继承 ----
    cout << "--- 3. public 继承：StudentPublic ---" << endl;
    StudentPublic sp;
    cout << "sp.name = " << sp.name << endl;   // public 还是 public，类外可访问 ✓
    sp.say_hello();                              // 同上 ✓
    cout << "public 继承：基类 public 成员还是 public" << endl;
    cout << endl;

    // ---- 测试4：protected 继承 ----
    cout << "--- 4. protected 继承：StudentProtected ---" << endl;
    StudentProtected sp2;
    sp2.show();              // 子类内部还能访问
    // sp2.name;             // 编译错误！name 降为 protected，类外不能访问
    // sp2.say_hello();      // 编译错误！同上
    cout << "（name 和 say_hello 已变为 protected，类外不可见）" << endl;
    cout << endl;

    // ---- 测试5：private 继承 ----
    cout << "--- 5. private 继承：StudentPrivate ---" << endl;
    StudentPrivate sp3;
    sp3.show();              // 子类内部还能访问
    // sp3.name;             // 编译错误！都变为 private 了
    cout << "（所有基类成员都变为 private，类外不可见）" << endl;
    cout << endl;

    // ---- 测试6：实际应用 ----
    cout << "--- 6. 实际应用：Teacher 和 GoodStudent ---" << endl;
    Teacher t("李老师", 35, "C++ 编程");
    t.say_hello();           // 继承自 Person 的 public 方法
    t.teach();

    GoodStudent stu("小明", 20, "大二", 95);
    stu.say_hello();         // 同样是继承来的
    stu.study();
    cout << endl;

    // ---- 总结 ----
    cout << "==================== 今日总结 ====================" << endl;
    cout << "1. 子类能访问基类的 public 和 protected 成员" << endl;
    cout << "2. 子类不能访问基类的 private 成员" << endl;
    cout << "3. 继承方式决定基类成员在子类中的权限级别" << endl;
    cout << "4. 实际开发用 public 继承就够了" << endl;
    cout << "5. '成员权限'是基类定的，'继承方式'是子类定的" << endl;
    cout << "==================================================" << endl;

    return 0;
}
