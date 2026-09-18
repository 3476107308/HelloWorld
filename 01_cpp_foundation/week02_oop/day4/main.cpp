#include<iostream>
using namespace std;
class Animal
{
    public:
    virtual void speak() = 0;
    void dospeak()
    {
        speak();
    }
};

class Cat:public Animal
{
    public:
    void speak()override
    {
        cout << "小猫在说话\n";
    }
};

class Dog:public Animal 
{
    public:
    void speak()override
    {
        cout << "小狗在说话\n";
    }
};

class Bird:public Animal
{
    public:
    void speak()override
    {
        cout << "小鸟在说话\n";
    }
};

void doSpeak(Animal* temp)
{
    temp->speak();
    delete temp;
    temp = nullptr;
}

void test01()
{
    //Animal temp;//编译错误 Animal是抽象类 不能实例化
    doSpeak(new Cat);
    doSpeak(new Dog);
    doSpeak(new Bird);
}

int main()
{
    test01();
    return 0;
}