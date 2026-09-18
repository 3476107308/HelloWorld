#ifndef CAT_H
#define CAT_H
#include"Animal.h"

using std::cout;

class Cat:public Animal
{
    public:
    Cat(string name,int age):Animal(name,age){}
    void speak()override
    {
        cout << name_ << "小猫在说话\n";
    }
    void move()override
    {
        cout << name_ << "小猫在走路\n";
    }
};

#endif