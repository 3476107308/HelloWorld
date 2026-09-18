#include<iostream>
#include<cstring>
using namespace std;

class Base
{
    public:
    Base()
    {
        cout << "调用Base构造\n";
    }
    virtual void speak() = 0;
    virtual~Base() = 0;
};

Base::~Base()
{
    cout << "纯虚析构调用\n";
}

class son1:public Base
{
    private:
    string* name_;
    public:
    son1(string st1)
    {
        name_ = new string(st1);
        cout << "调用son1构造\n";
    }
    ~son1()
    {
        cout << "调用son1析构\n";
        delete name_;
        name_ = nullptr;
    }
    void speak()
    {
        cout << *name_ << "儿子在说话\n";
    }
};

void test01()
{
    Base* temp = new son1("111");
    temp->speak();
    delete temp;
}

int main()
{
    test01();
    return 0;
}