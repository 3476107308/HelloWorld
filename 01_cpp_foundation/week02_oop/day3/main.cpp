#include<iostream>
#include<cstring>
using namespace std;


class Animal_NoVirtual
{
    public:
    void speak()
    {
        cout << "动物在说话\n";
    }
};
class Cat_NoVirtual:public Animal_NoVirtual
{
    public:
    void speak()
    {
        cout << "猫在说话\n";
    }
};


class Animal
{
    public:
    virtual void speak()
    {
        cout << "动物在说话\n";
    }
};

class Cat:public Animal
{
    public:
    void speak()override
    {
        cout << "猫在说话\n";
    }
};

class Dog: public Animal
{
    public:
    void speak()override
    {
        cout << "狗在说话\n";
    }
};




void doSpeak(Animal& animal)
{
    animal.speak();
}

void test01()
{
    //引用多态
    Cat cat1;
    doSpeak(cat1);
    //指针多态
    Animal* p = new Cat;
    p->speak();
    delete p;

    p = new Dog;
    p->speak();
    delete p;
    p = nullptr;
}

void test_no_virtual()
{
    Animal_NoVirtual* p = new Cat_NoVirtual;
    p->speak();
    delete p;
    p = nullptr;
}


class AbstractCalculator
{
    public:
    int num1_,num2_;
    virtual int getans()
    {
        return 0;
    }
};

class AddCalculator: public AbstractCalculator
{
    public:
    int getans()override
    {
        return num1_ + num2_;
    }
};

class SubCalculator: public AbstractCalculator
{
    public:
    int getans()override
    {
        return num1_ - num2_;
    }
};

void test02()
{
    AbstractCalculator* abc = new AddCalculator;
    abc->num1_ = 10;
    abc->num2_ = 20;
    cout << "加法计算器\n" << abc->num1_ << " + " << abc->num2_ << " = " << abc->getans() << endl;
    delete abc;
    abc = new SubCalculator;
    abc->num1_ = 110,abc->num2_ = 10;
    cout << "减法计算器\n" << abc->num1_ << " - " << abc->num2_ << " = " << abc->getans() << endl; 
    delete abc;
    abc = nullptr;
}


int main()
{
    cout << "有virtual\n";
    test01();
    cout << "无virtual\n";
    test_no_virtual();
    test02();
    return 0;
}