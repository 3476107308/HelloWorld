#ifndef BIRD_H
#define BIRD_H
#include"Animal.h"

using std::cout;

class Bird:public Animal
{
    public:
    Bird(string name,int age):Animal(name,age){}
    void speak()override
    {
        cout << name_ << "小鸟在说话\n";
    }
    void move()override
    {
        cout << name_ << "小鸟在飞翔\n";
    }
};

#endif