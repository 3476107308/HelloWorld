#include<iostream>
using namespace std;

//继承中的构造和析构顺序
class Base
{
public:
    Base()
    {
        cout << "Base构造函数!\n";
    }
    ~Base()
    {
        cout << "Base析构函数!\n";
    }

};

class Son:public Base
{
    public:
    Son()
    {
        cout << "Son构造函数!\n";
    }
    ~Son()
    {
        cout << "Son析构函数!\n";
    }
};

class Person
{
private:
    string name_;
    int age_;
public:
    Person(string name,int age):name_(name),age_(age)
    {
        cout << "Person 构造 name_" << name_ << " age=" << age_ << endl;
    }
    ~Person()
    {
        cout << "Person 析构 name_" << name_ << endl;
    }
    string get_name()const
    {
        return name_;
    }
    int get_age()const 
    {
        return age_;
    }
};

class Student:public Person{
    private:
    int score_;
    public:
    Student(string name,int age,int score):Person(name,age),score_(score)
    {
        cout << "Student 构造 score=" << score_ << endl;
    }
    ~Student()
    {
        cout << "Student 析构" << endl;
    }

};

void test01()
{
    //Base b;
    Son s;
    //先构造父类，再构造子类 先析构子类 再析构父类
}
void test02()
{
    Student stu("小明",18,95);
    cout << "姓名： " << stu.get_name() << ",年龄： " << stu.get_age() << endl;
}

int main()
{
    test01();
    cout << endl;
    test02();
    return 0;
}