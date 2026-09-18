#ifndef DOG_H
#define DOG_H
#include"Animal.h"

using std::cout;

class Dog:public Animal
{
    public:
    Dog(string name,int age):Animal(name,age){}
    void speak()override
    {
        cout << name_ << "小狗在说话\n";
    }
    void move()override
    {
        cout << name_ << "小狗在走路\n";
    }
};

#endif